# `sak/math/` — Mathematical Utilities

## `math.hpp` — Arithmetic Operations & Math Functions

Provides Niebloid wrappers for arithmetic and mathematical operations. These are function objects ( Niebloids ) rather than raw functions, making them pipeable and composable in range pipelines.

Every unary/binary/ternary Niebloid offers two forms:
- **Scalar overload** — applied to a plain value.
- **Range overload** — applied via `operator|` to a range, transforming it element-wise through `std::views::transform`. Unary Niebloids inherit from `std::ranges::range_adaptor_closure`; n-ary Niebloids expose a nested closure object that captures the bound arguments ( e.g., `range | is_multiple( 3 )`, `range | clamp( 0, 10 )`, `range | between( 1, 6 )` ).

**Arithmetic functors** ( thin wrappers re-exported from `std` ):
- `plus`, `minus`, `multiplies`, `divides`, `modulus`
- `equal_to`, `less_equal`, `greater_equal`

**Custom Niebloids:**
| Name | Purpose |
|------|---------|
| `abs( value )` | Absolute value |
| `negate( value )` | Negation |
| `square( value )` | `value * value` |
| `is_even( value )` | Multiple of 2 ( integral only ) |
| `is_odd( value )` | Not even ( integral only ) |
| `sign( value )` | Signum: returns -1, 0, or 1 |
 | `is_multiple( first, second )` | `first % second == 0` ( integral only ) |
 | `between( value, low, high )` | Checks `low ≤ value ≤ high` |
 | `clamp( value, low, high )` | Clamp between bounds |
 | `sum( range )` | `fold_left( range, 0, plus )` |
 | `dot( first, second )` | `sum( first * second )` element-wise dot product; `dot( vector )` = `sum( vector * vector )` |
 | `min( first, second )` / `min( range )` | Minimum ( binary or over range via `std::ranges::min` ) |
 | `max( first, second )` / `max( range )` | Maximum ( binary or over range via `std::ranges::max` ) |

**Math function Niebloids** ( macro-generated wrappers over `<cmath>` ):
- Unary: `acos`, `asin`, `atan`, `ceil`, `cos`, `exp`, `floor`, `log`, `log2`, `round`, `sin`, `sqrt`, `tan`, `tanh`, `trunc`
- Binary: `atan2`, `pow`

These exist so they can be used in range pipelines ( e.g., `range | square_root` ).

**Descriptive aliases:** Each custom and std wrapper also exports a descriptive alias alongside the `sak_< name >` symbol: `absolute` ( abs ), `arccosine`, `arcsine`, `arctangent`, `ceiling`, `cosine`, `exponential`, `round_down`, `natural_logarithm`, `logarithm_base_two`, `round_to_nearest`, `sine`, `square_root` ( sqrt ), `tangent`, `hyperbolic_tangent`, `truncate`, `arctangent2` ( atan2 ), `power` ( pow ).

**Constraint narrowing:** The concept in each macro invocation narrows the scalar overload. `abs`, `negate`, `square` accept any non-range value ( `is_value` ); `sign`, `square_root` and the `<cmath>` wrappers accept any arithmetic type; `is_even`, `is_odd`, `is_multiple` are restricted to integral types. This keeps range operands routed to the element-wise overload instead of the scalar one.

## `concepts.hpp` — Math Concepts

Defines the concepts used to constrain the Niebloids:
- `is_arithmetic< t_arithmetic >` — alias for `std::is_arithmetic_v`
- `is_integral< t_integral >` — alias for `std::is_integral_v`
- `is_value< t_value >` — `not std::ranges::input_range< t_value >`; routes ranges to the element-wise overload instead of the scalar one
- `is_number< t_number >` — arithmetic and not `bool`
- `is_triplet< t_vector >` — a fixed-size range with exactly 3 elements ( compile-time check )
- `is_crossable< t_vector >` — `is_triplet` or a lazy view ( trusted for size )

## `error.hpp` — Math Error Codes

- `enum class error` — `division_by_zero`, `arithmetic_overflow`, `sqrt_of_negative`
- `struct exception` — derives from `std::exception`, carries an `error` code, and returns a descriptive message from `what( )`.

## `vector.hpp` — Vector Math

Dimension-agnostic vector operations built on top of `math.hpp` and `ranges/operators.hpp`:
- `length( vector )` — Euclidean norm: `square_root( sum( vector * vector ) )`
- `vector | normalize` — unit vector in the same direction ( direct-application closure )
- `cross( left, right )` / `left | cross( right )` — cross product via cyclic permutation, 3-element vectors only ( `is_crossable` )
- `vector | rotate( axis, angle )` — Rodrigues rotation around an axis by an angle
