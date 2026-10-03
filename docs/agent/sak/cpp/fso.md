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
| `modified_at( )` | `const time_point&` |
| `created_at( )` | `const time_point&` |

Both timestamps are `std::chrono::system_clock::time_point`. When the file does not exist they hold a default-constructed `time_point` ( the epoch ), and the consumer is responsible for checking `exists( )` before consuming them.

### `refresh( )` and `path( path_type )`

`refresh( )` is virtual and recomputes the metadata. It sets `exists` and assigns a default-constructed `time_point` to both `modified_at` and `created_at`, then, when the file exists, reads the real timestamps:

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

`content( )` returns `const string&`. `read( )` ( re )loads the content when the file exists and returns the same string reference:

```cpp
auto read( ) -> const string&;
```

When the file does not exist, `read( )` leaves `m_content` as-is, which is a default-constructed empty `string` until a write populates it.

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
	__using( ::sak::, exit_success, ensure )
	__using( ::sak::fso::, text_file )

	text_file note( "notes/hello.txt" );
	note.write( "hello, world\n" );

	ensure( note.exists( ), "note must exist after write" );
	println( "content: {}", note.content( ) );

	return	exit_success;
}
```

---

## Tests

The grouped test `tests/sak/test_sak_fso.cpp` exercises `file` and `text_file` together. It performs the timestamp checks in an `exists( )`-gated way, comparing against the default-constructed `time_point` sentinel. It verifies that a freshly constructed `text_file` does not exist and exposes that sentinel through both `modified_at` and `created_at` while its content is empty, that `write( )` returns the created-file message and then makes the file exist with round-tripping content and non-default timestamps, that the derived components ( `name`, `extension`, `folder`, `base`, `path` ) are correct, that a plain `file` reads the same path metadata, and that a missing file reports no `exists` and the default `time_point` sentinel for `modified_at`.
