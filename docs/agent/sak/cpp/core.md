# Core Infrastructure

## `sak/using.hpp` — Variadic `using` Macro

Provides `__using( prefix, name1, name2, ... )` — a variadic macro that expands to a series of `using prefix::name;` declarations. Supports up to 64 names per invocation. Also provides `__use_macro( macro, ... )` for invoking a macro over multiple argument groups. Reduces visual noise when importing multiple symbols from the same namespace.

## `sak/default_ctc_dtc.hpp` — Constructor/Destructor Macros

Standardized macros for common copy/move constructor/destructor patterns:
- `use_default_copy_ctc`, `use_default_move_ctc`, `use_default_copy_move_ctc`
- `use_non_default_copy_ctc`, `use_non_default_move_ctc`, `use_non_default_copy_move_ctc`
- `delete_copy_ctc`, `delete_move_ctc`, `delete_copy_move_ctc`
- `use_default_dtc`, `use_non_default_dtc`

## `sak/ensure.hpp` — Runtime Assertion

Niebloid `sak::ensure( expression, message )` that throws `std::runtime_error` with the given message when the expression is false. Also exports `sak::exit_success` / `sak::exit_failure`.

## `sak/concepts.hpp` — Core Concepts

- `is_callable< t_callable, t_args... >` — the compiler accepts calling the object via `operator( )`.
- `is_tuple< t_tuple >` — the type is tuple-like ( has `tuple_size` ).

## `sak/sak.hpp` — Main Header

Aggregates `default_ctc_dtc.hpp`, `ensure.hpp`, `concepts.hpp`. Defines `sak::byte` as `uint8_t`.
