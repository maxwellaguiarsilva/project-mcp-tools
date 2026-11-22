# `sak/opengl/` — OpenGL Objects

Header-only OpenGL objects for shader compilation, program linking, and GPU buffer ownership. The module depends on the GLAD function table and on the C++26 reflection facilities, and it carries no filesystem dependency: every entry point takes GLSL text or a section map, so the caller decides where the source comes from.

`opengl.hpp` is the module aggregator. This document covers `shader.hpp` and `program.hpp`.

| Header | Subject |
|---|---|
| `shader.hpp` | shader type enum, section splitting, section compilation, shader storage buffer |
| `program.hpp` | program link and activation |

---

## `shader.hpp` — Shader and Storage Buffer

Declared in `include/sak/opengl/shader.hpp`. The header defines the flat-payload concept, the section helpers, the `shader` class, and the nested `storage_buffer` template.

### `shader::type`

`enum class type : GLenum` maps each stage name to its OpenGL enumerator:

| Enumerator | GL enumerator |
|---|---|
| `vertex` | `GL_VERTEX_SHADER` |
| `fragment` | `GL_FRAGMENT_SHADER` |
| `geometry` | `GL_GEOMETRY_SHADER` |
| `tess_control` | `GL_TESS_CONTROL_SHADER` |
| `tess_evaluation` | `GL_TESS_EVALUATION_SHADER` |
| `compute` | `GL_COMPUTE_SHADER` |

### `shader::table` and `shader::sections`

```cpp
using	table		=	map< type, shader >;
using	sections	=	map< string, string >;
```

`table` keys the compiled shaders by stage, so a `program` can consume the table through `std::views::values`. `sections` carries GLSL text keyed by section name, which is the input form of `compile` and the output form of `split_sections`.

### `shader::split_sections( string_view )`

Parses a combined GLSL document into its named sections.

- The section name is a word, and each section is delimited by two marker lines: `//<tab><name>-shader` opens it, and the same marker closes it.
- The body of a section is the text between the two marker lines, excluding both markers, so each section keeps its own `#version` directive.
- The marker pair is found by a multiline regular expression that captures the name and the body, and the closing marker must repeat the opening name.
- A repeated section is rejected with `duplicate shader section: <name>`, and a document without a section is rejected with `no shader section found`.
- `sections` is an ordered map keyed by name, so iterating the result walks the section names in order.

### `shader::compile( const sections& )`

Compiles a supplied section map into a table.

- Each section name is resolved through `shader::kind_of( name )`, which matches it against the enumerator identifiers of `shader::type`.
- A section whose name has no enumerator is rejected with `unknown shader section: <name>`.
- A section whose stage is already present in the table is rejected with `duplicate shader section: <name>`.
- Each accepted section constructs one `shader` from its body and its stage, and the shader is stored under that stage in the returned table.
- Compilation consumes finished section text only. Any rewriting of a section body happens between `split_sections` and `compile`.

### `shader::kind_of` and `shader::name_of`

```cpp
static auto kind_of( const string_view name ) -> type;
static auto name_of( const type kind ) -> string_view;
```

`kind_of` maps a section name to the enumerator that declares it, and rejects an unknown name with `unknown shader section: <name>`. `name_of` returns the enumerator identifier for a stage, and rejects an unknown value with `unknown shader kind`. Both walk `define_static_array( enumerators_of( ^^type ) )`, so the name and the stage remain two views of the same enumerator and the header keeps no repeated name table.

### `shader` construction

```cpp
shader( const string& source, const type shader_type = vertex );
```

The constructor creates a shader object, uploads the in-memory source, compiles it, and reads `GL_COMPILE_STATUS`. On failure it throws `std::runtime_error` carrying the info log returned by `fetch_log( gl_get_shader_info_log, id( ) )`. The shader object is owned through a private RAII destructor, and `program` is a friend of the class so it can attach the shader without exposing the handle.

### `shader::storage_buffer< t_payload >`

An RAII owner of one OpenGL buffer plus one heap array of payloads.

- `static_assert( is_flat< t_payload > )` constrains the payload at compile time.
- `ssbo_header { GLuint m_count; GLuint m_stride; }` is prefixed to every payload region. `m_stride` is `sizeof( t_payload ) / sizeof( float )`, so the shader addresses elements in float units.
- The constructor takes only the element count. It allocates `unique_ptr< t_payload[ ] >`, creates the buffer, reserves `sizeof( ssbo_header ) + count * sizeof( t_payload )` bytes of dynamic storage, and writes the header immediately.
- `operator[ ]( index )`, `data( )`, and `count( )` expose the host-side payload region. Mutating an element uploads nothing.
- `flush( )` uploads the payload region with one `glNamedBufferSubData` at offset `sizeof( ssbo_header )`.
- `bind( index )` performs `glBindBufferBase` on the shader storage target. The binding index is current-context state, so binding stays a separate explicit step and the class never chooses an index on its own.

### `is_flat` and `has_no_padding`

```cpp
consteval auto has_no_padding( const info type ) -> bool;
template< typename t_type > concept is_flat = has_no_padding( ^^t_type ) and is_trivially_copyable_v< t_type >;
```

`has_no_padding` reflects the non-static data members with `access_context::current( )`, accumulates `size_of( type_of( member ) )` in declaration order, and compares each accumulated offset against `offset_of( member ).bytes`. The check passes only when the members are contiguous and the accumulated total equals `size_of( type )`.

---

## `program.hpp` — Program

Declared in `include/sak/opengl/program.hpp`.

```cpp
template< is_shader_range t_range >
class program;
```

- `is_shader_range` accepts any `std::ranges::input_range` whose `range_reference_t` converts to `const shader&`, so `shader::table | std::views::values` satisfies it directly.
- The constructor creates a program object, attaches every shader in the range, links, and reads `GL_LINK_STATUS`. On failure it throws `std::runtime_error` carrying the info log returned by `fetch_log( gl_get_program_info_log, id( ) )`.
- `use( )` activates the program with `glUseProgram`.
- The program object is owned through a private RAII destructor that calls `glDeleteProgram`.

---

## Source Boundary

`sak/opengl` consumes GLSL text and section maps, and produces compiled objects:

- `shader` takes a `std::string` source and a stage.
- `shader::split_sections` takes a `std::string_view` document.
- `shader::compile` takes a `sections` map.
- `program` takes a range of shaders.

Any data source produces those inputs: a file, a database, a REST endpoint, or in-memory generation. `sak::fso::text_file` is one consumer-side option, which the demo uses to read the combined document and the per-figure blocks.

---

## Usage

The consumer pipeline is: read the document, split it into sections, edit or inject section bodies, compile the map, and link the program.

```cpp
#include <ranges>
#include <sak/using.hpp>
#include <sak/fso/text_file.hpp>
#include <sak/opengl/program.hpp>

auto main( ) -> int
{
	__using( ::std::, string )
	__using( ::std::views::, values )
	__using( ::sak::fso::, text_file )
	__using( ::sak::opengl::, program, shader )

	const text_file document( "data/shader/figures.glsl" );
	auto sections = shader::split_sections( document.content( ) );

	const string fragment_name( shader::name_of( shader::type::fragment ) );
	sections.at( fragment_name ) = sections.at( fragment_name ) + "\n//\tgenerated prelude";

	const shader::table shaders = shader::compile( sections );
	program< decltype( shaders | values ) > linked( shaders | values );
	linked.use( );

	return	0;
}
```

---

## Tests

The module has no structured test. It is exercised by `source/figures_demo.cpp`, which reads the combined document, splits it into sections, assembles the figure dispatch into the fragment section, compiles the section map, links the program, and binds the three storage buffers at indices `0`, `1`, and `2`. The three payloads pass `is_flat` at compile time.
