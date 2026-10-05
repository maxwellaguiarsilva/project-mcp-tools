# `sak/ranges/` — Range Utilities

## `to.hpp` — Universal Materializer

**Active consumption core.** `sak::ranges::to` replaces the retired `to_point`. It is a native `range_adaptor_closure` that produces a proxy holding the range; the proxy converts to *any* target type at the assignment site via a templated implicit conversion operator.

```cpp
point | sin | to      // -> sak::point ( dimension-agnostic )
range | to            // -> any target deduced from the assignment context
```

- The general path delegates to `std::ranges::to< t_target >`, gated by `requires( ( is_class< t_target > or is_union< t_target > ) and is_toable< t_target, t_range > )` ( `is_class` / `is_union` from `<sak/concepts.hpp>` ) to mirror `std::ranges::to`'s own requirement and keep scalar and other non-class targets out of its hard `static_assert`. `is_toable` is recursive and mirrors `std::ranges::to`: it rejects views outright ( `not view< t_target >` ), accepts a direct path when the target is constructible from the range ( `constructible_from< t_target, from_range_t, t_range >` or `constructible_from< t_target, t_range >` ) or supports `insert( end( ), value )`, and accepts a recursive path when the target is an `input_range` and the source element reference is not convertible to the target's `range_value_t`, requiring each element to materialize into the value type ( `is_toable< range_value_t< t_target >, range_reference_t< t_range > >` ). That recursive path covers ranges of ranges, e.g. materializing `line | split( ' ' ) | filter( ... )` into `std::vector< std::string >` where each element is a `subrange`. Targets `std::ranges::to` cannot actually produce ( `std::allocator`, `std::initializer_list`, `std::basic_string_view` ) are thus kept out of its hard errors when `to` is used inline as a constructor argument.
- `std::array` has no `from_range_t` constructor, so `std::array` gets a direct copy specialization.
- A cv-qualified destination deduces `t_target` as `const T`, so `__to_impl< const t_target >` delegates to `__to_impl< t_target >` instead of reaching the generic path.
- The conversion is `&&`-qualified and intentionally rejects `auto` deduction so the target type is always explicit at the assignment site.
- The conversion operator is constrained by `materializes< t_target, t_range >`, which requires `__to_impl< t_target >::apply( range )` to be a valid expression.

The proxy deduces its target from the left-hand side of the assignment ( `point p = expr | to;`, and future scalar targets ). Without the constraint, a class whose constructor accepts `convertible_to` scalars ( e.g. `point` ) would offer two equally-ranked user-defined conversions — the destination constructor and the proxy operator — and the assignment would be rejected as ambiguous. Constraining the operator by materializability makes `convertible_to< proxy, scalar >` false, so the destination constructor stops competing, while targets with a `__to_impl` specialization ( e.g. `point`, `array` ) still materialize.

## `operators.hpp` — Element-wise Operators

Provides `+`, `-`, `*`, `/`, `%` ( and their compound `+=`, `-=`, etc. ) for ranges, with the behavioral intent that containers produce eager results, views produce lazy results, and strings stay standard. The constraints are now expressed through `is_container` / `any_is_view` / `is_same_decayed`:
- **Containers** ( `is_container`: non-view, non-string-like ): eager result via `std::ranges::transform` into a new container. The container-container overloads additionally require `is_same_decayed< t_left, t_right >`.
- **Views** ( the lazy overloads use `viewable_range` plus `any_is_view`, i.e. at least one operand is a view; the view-scalar and scalar-view forms require `is_view` ): lazy result via `std::views::zip_transform`.
- Also provides unary negation for containers, constrained by `is_container`.

## `concepts.hpp` — Range Concepts

- `is_view` — alias for `std::ranges::view` on the cvref-unwrapped type
- `is_resizable` — has `resize( size_t )`
- `is_string_like` — has `traits_type` ( string-like containers excluded from element-wise operators )
- `is_container< t_container >` — an `input_range` that is neither a view nor string-like: `input_range and not is_view and not is_string_like`
- `any_is_view< t_ranges... >` — variadic; true when at least one passed range is a view: `( is_view< t_ranges > or ... )`
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
| `regex_matches.hpp` | `regex_matches` | `std::regex_iterator` ( wrapped as a view ) | Lazy view of the `std::smatch` results of applying a `std::regex` to a range's characters |

**`rotated` details:** Implements circular rotation via `concat( range, range ) | drop( offset % length ) | take( length )`. The default `| rotated` ( no args ) rotates by 1. Exposes a closure for `| rotated( offset )`.

**`regex_matches` details:** A niebloid modeled on `rotated`/`enumerate` that stores the pattern by `const regex&` in its nested closure. `some_range | regex_matches( pattern )` yields a lazy `std::ranges::subrange` over `std::regex_iterator`, i.e. the `std::smatch` results of applying `pattern` to the range's characters; the direct form `regex_matches( range, pattern )` is also provided. It is not `constexpr` because `std::regex` is runtime, and it returns the same match type as `std::sregex_iterator`.
