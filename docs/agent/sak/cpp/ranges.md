# `sak/ranges/` — Range Utilities

## `to.hpp` — Universal Materializer

**Active consumption core.** `sak::ranges::to` replaces the retired `to_point`. It is a native `range_adaptor_closure` that produces a proxy holding the range; the proxy converts to *any* target type at the assignment site via a templated implicit conversion operator.

```cpp
point | sin | to      // -> sak::point ( dimension-agnostic )
range | to            // -> any target deduced from the assignment context
```

- The general path delegates to `std::ranges::to< t_target >`, gated by `requires( is_class< t_target > or is_union< t_target > )` ( `is_class` / `is_union` from `<sak/concepts.hpp>` ) to mirror `std::ranges::to`'s own requirement and keep scalar and other non-class targets out of its hard `static_assert`.
- `std::array` has no `from_range_t` constructor, so `std::array` gets a direct copy specialization.
- A cv-qualified destination deduces `t_target` as `const T`, so `__to_impl< const t_target >` delegates to `__to_impl< t_target >` instead of reaching the generic path.
- The conversion is `&&`-qualified and intentionally rejects `auto` deduction so the target type is always explicit at the assignment site.
- The conversion operator is constrained by `materializes< t_target, t_range >`, which requires `__to_impl< t_target >::apply( range )` to be a valid expression.

The proxy deduces its target from the left-hand side of the assignment ( `point p = expr | to;`, and future scalar targets ). Without the constraint, a class whose constructor accepts `convertible_to` scalars ( e.g. `point` ) would offer two equally-ranked user-defined conversions — the destination constructor and the proxy operator — and the assignment would be rejected as ambiguous. Constraining the operator by materializability makes `convertible_to< proxy, scalar >` false, so the destination constructor stops competing, while targets with a `__to_impl` specialization ( e.g. `point`, `array` ) still materialize.

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
