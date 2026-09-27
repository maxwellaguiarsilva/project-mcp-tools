# `sak/fso/` — File-System Objects

Header-only, domain-agnostic abstractions over file-system objects. This module is a C++26 port of the Python `sak/fso` reference ( `file.py`, `text_file.py` ) kept under `docs/agent/analyses/opengl/opengl-tag/src/sak/fso/`.

The module provides two levels of abstraction:

- `file` — a path plus cached derived components and metadata.
- `text_file` — a `file` that also carries its textual content, with read and write operations.

---

## `file.hpp` — File-System Object

Declared in `include/sak/fso/file.hpp`. `file` wraps a `std::filesystem::path` and exposes derived components and metadata.

### Type Aliases

```cpp
using	path_type		=	::std::filesystem::path;
using	time_point		=	system_clock::time_point;
using	optional_time	=	optional< time_point >;
```

### Construction and Mutation

The constructor takes a `std::filesystem::path`, stores it, and immediately computes the derived components and metadata:

```cpp
explicit file( path_type source_path );
```

`path( path_type )` is a mutator overload of the `path( )` accessor that replaces the stored path and recomputes both the derived components and the metadata.

### Derived Components

| Accessor | Meaning |
|---|---|
| `path( )` | the full stored path |
| `base( )` | the parent path (`path` minus the final component) |
| `folder( )` | the name of the parent folder |
| `name( )` | the file stem (`path` without extension) |
| `extension( )` | the extension without the leading dot |

### Metadata

| Accessor | Type |
|---|---|
| `exists( )` | `bool` |
| `modified_at( )` | `const optional_time&` |
| `created_at( )` | `const optional_time&` |

Both timestamps are `std::optional< std::chrono::system_clock::time_point >` and are empty when the file does not exist. `modified_at` and `created_at` are reset to empty at the start of every `refresh( )`.

### `refresh( )` and `path( path_type )`

`refresh( )` is virtual and recomputes the metadata. It sets `exists`, then, when the file exists, reads `modified_at` and `created_at`:

```cpp
virtual auto refresh( ) -> void;
```

The mutator overload `path( path_type )` recomputes the derived components ( `refresh_components( )` ) and then calls `refresh( )`.

### Design Decisions

- **Header-only.** The whole module is implemented in headers; there is no `.cpp` companion.
- **`modified_at`.** Computed with `std::filesystem::last_write_time` and converted from the file clock with `std::chrono::clock_cast< system_clock >`.
- **`created_at`.** The standard library exposes no portable creation time, so the port mirrors the Python `os.path.getctime` semantics on Linux by calling `::stat` and reading `st_ctim` ( the inode status-change time ), combining `from_time_t( tv_sec )` with `nanoseconds( tv_nsec )`.
- Default destructor and default copy/move constructors are enabled via `use_default_dtc` and `use_default_copy_move_ctc`.

---

## `text_file.hpp` — Text File

Declared in `include/sak/fso/text_file.hpp`. `text_file final` derives publicly from `file` and adds textual content plus read and write operations.

### Content and `read( )`

`content( )` returns `const optional< string >&`. `read( )` ( re )loads the content when the file exists and returns the same optional reference:

```cpp
auto read( ) -> const optional_content&;
```

When the file exists but cannot be opened, `read( )` reports the failure through `sak::ensure` with the message `text_file: unable to read file: <path>`.

### `write( )`

```cpp
auto write( const string_view new_content ) -> string;
```

`write( )` creates the parent directories if needed, writes the content, closes and validates the output stream, then calls `refresh( )`. It returns the message `"created file: <path>\n"`. Failures are reported through `sak::ensure` with messages prefixed by `text_file: unable to write file:` or `text_file: failed to write file:`.

### `refresh( )`

`text_file` overrides `refresh( )` to call `file::refresh( )` and then `read( )`, so metadata and content stay consistent after any mutation.

### Usage

```cpp
#include <print>
#include <sak/using.hpp>
#include <sak/fso/text_file.hpp>

auto main( ) -> int
{
	__using( ::std::, println )
	__using( ::sak::, exit_success )
	__using( ::sak::fso::, text_file )

	text_file note( "notes/hello.txt" );
	note.write( "hello, world\n" );

	if( const auto& content = note.content( ); content )
		println( "content: {}", content.value( ) );

	return	exit_success;
}
```

---

## Tests

The grouped test `tests/sak/test_sak_fso.cpp` exercises `file` and `text_file` together. It verifies that a freshly constructed `text_file` has no metadata or content, that `write( )` returns the created-file message, that the written content round-trips, that the derived components ( `name`, `extension`, `folder`, `base`, `path` ) are correct, that a plain `file` reads the same path metadata, and that a missing file reports no `exists` and no `modified_at`.
