# The `sak` Library

The core of this project is the `sak` library ( Swiss Army Knife ). It is designed as a collection of generic, domain-independent utilities covering mathematics, geometry, ranges, and design patterns.

- **Domain Agnostic:** It contains no business logic or hardware dependencies.
- **Modern Paradigms:** It leverages C++26 features such as `ranges`, `views`, concepts, and custom `Niebloids` to reduce visual noise and promote **functional composition**.
- **Native STL Integration:** All pipeable utilities are native `std::ranges::range_adaptor_closure` objects; the STL's own `operator|` routes every pipeline. There are no custom global `operator|` overloads.

---

## Directory Structure

- Root headers: `sak.hpp` (aggregator), `using.hpp` (the `__using` macro families), `ensure.hpp`, `concepts.hpp`, `default_ctc_dtc.hpp`, `format.hpp`, `string.hpp`.
- `expand64/` — generated expansion families consumed by `using.hpp`.
- `meta/` — `override.hpp`.
- `fso/` — `file.hpp`, `text_file.hpp`.
- `math/` — `math.hpp`, `vector.hpp`, `concepts.hpp`, `error.hpp`.
- `pattern/` — `dispatcher.hpp`, `tupled.hpp`, `value_or.hpp`, `parse.hpp`, `bitmask.hpp`, `cast.hpp`, `enum.hpp`.
- `ranges/` — `to.hpp`, `operators.hpp`, `concepts.hpp`, `contains.hpp`, `count_to.hpp`, `fold_left_first.hpp`, `index_of.hpp`, `transform.hpp`, `chunk.hpp`.
- `ranges/views/` — `enumerate.hpp`, `cartesian_product.hpp`, `reversed.hpp`, `rotated.hpp`, `regex_matches.hpp`.
- `geometry/` — `point.hpp`, `geometry.hpp`, `line_view.hpp`, `concepts.hpp`.
- `opengl/` — `opengl.hpp`, `shader.hpp`, `program.hpp`, `vertex_array.hpp`, `fetch_log.hpp`, `glad/`.
- `sdl3/` — `application.hpp`, `window.hpp`, `display.hpp`, `audio.hpp`, `opengl/`.

---

## Modules

- [Core Infrastructure](core.md) — `using`, `default_ctc_dtc`, `ensure`, `concepts`, `sak.hpp`.
- [`sak/fso/`](fso.md) — file-system objects: `file` and `text_file`.
- [`sak/math/`](math.md) — arithmetic operations, math functions, math concepts, vector math.
- [`sak/pattern/`](pattern.md) — design patterns: dispatcher, tupled, value_or, parse, bitmask.
- [`sak/ranges/`](ranges.md) — range utilities and views.
- [`sak/geometry/`](geometry.md) — geometry primitives and the `point` core class.

- **Pending:** `format.hpp`, `string.hpp`, `opengl/`, `sdl3/`, `meta/` and `expand64/` have no dedicated document. The OpenGL and SDL3 coverage lives in the analysis tree at `docs/agent/analyses/opengl/` and `docs/agent/analyses/sdl3/`.
