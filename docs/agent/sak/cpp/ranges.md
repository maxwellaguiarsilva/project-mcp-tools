# `sak/ranges/` — Range Utilities

## `to.hpp` — Universal Materializer

**Active consumption core.** `sak::ranges::to` replaces the retired `to_point`. It is a native `range_adaptor_closure` that produces a proxy holding the range; the proxy converts to *any* target type at the assignment site via a templated implicit conversion operator.

```cpp
point | sin | to      // -> sak::point ( dimension-agnostic )
range | to            // -> any target deduced from the assignment context
```

- The general path delegates to `std::ranges::to< t_target >`.
- `std::array` has no `from_range_t` constructor, so `std::array` gets a direct copy specialization.
- The conversion is `&&`-qualified and intentionally rejects `auto` deduction so the target type is always explicit at the assignment site.

## `operators.hpp` — Element-wise Operators

Provides `+`, `-`, `*`, `/`, `%` ( and their compound `+=`, `-=`, etc. ) for ranges:
- **Containers** ( non-view, non-string-like ): eager result via `std::ranges::transform` into a new container.
- **Views** ( at least one operand is a view ): lazy result via `std::views::zip_transform`.
- Also provides unary negation for containers.

## `concepts.hpp` — Range Concepts

- `is_view` — alias for `std::ranges::view` on the cvref-unwrapped type
- `is_resizable` — has `resize( size_t )`
- `is_string_like` — has `traits_type` ( string-like containers excluded from element-wise operators )
- `is_indirectly_binary_left_foldable` — fold constraint used by `fold_left_first`

## `contains.hpp` — Range Containment

Niebloid `contains( range, value )` mirroring `std::ranges::contains`, plus a braced-list overload `contains( range, { "foo", "bar" } )` that returns true if any listed value is present.

## `count_to.hpp` — Integer Range

Niebloid `count_to( bound )` producing the integer range `[ 0, bound )` via `std::views::iota`, casting the zero to the bound's type so endpoints share an integer type.

## `transform.hpp` — Transform Aliases

Re-exports `eager_transform` ( `std::ranges::transform` ) and `lazy_transform` ( `std::views::transform` ) under the `sak::ranges` namespace.

## `chunk.hpp` — Fixed-Size Chunks

Splits a range into subranges of a given size via a closure object. Returns a view of `subrange`s. Supports pipe syntax via `range_adaptor_closure`.

## `fold_left_first.hpp` — Fold with First Element

Folds a range using the first element as the initial value. Returns `std::optional< t_value >` ( empty if range is empty ). Provides both iterator/sentinel and range overloads.

## Views ( `views/` directory )

| File | Name | STL Equivalent | Purpose |
|------|------|---------------|---------|
| `enumerate.hpp` | `enumerate` | `std::views::enumerate` | Zips a range with an index starting from 0 ( or custom start ) |
| `cartesian_product.hpp` | `cartesian_product` | `std::views::cartesian_product` | Cartesian product of two ranges, returns pairs |
| `rotated.hpp` | `rotated` | *( no direct STL equivalent )* | Circular rotation of a range by an offset; uses concat + drop/take |

**`rotated` details:** Implements circular rotation via `concat( range, range ) | drop( offset % length ) | take( length )`. The default `| rotated` ( no args ) rotates by 1. Exposes a closure for `| rotated( offset )`.
