# `sak/math/` — Mathematical Utilities

## `math.hpp` — Arithmetic Operations & Math Functions

Provides Niebloid wrappers for arithmetic and mathematical operations. These are function objects ( Niebloids ) rather than raw functions, making them pipeable and composable in range pipelines.

Every unary/binary/ternary Niebloid offers two forms:
- **Scalar overload** — applied to a plain value.
- **Range overload** — applied via `operator|` to a range, transforming it element-wise through `std::views::transform`. Unary Niebloids inherit from `std::ranges::range_adaptor_closure`; n-ary Niebloids expose a nested closure object that captures the bound arguments ( e.g., `range | is_multiple( 3 )`, `range | clamp( 0, 10 )`, `range | between( 1, 6 )` ).

**Arithmetic functors** ( thin wrappers re-exported from `std` ):
- `plus`, `minus`, `multiplies`, `divides`, `modulus`
- `bit_and`, `bit_or`, `bit_xor`, `bit_not`
- `equal_to`, `not_equal_to`, `less`, `less_equal`, `greater`, `greater_equal`

**Binary Niebloids:**

| Name | Purpose |
|------|---------|
| `atan2( first, second )` / `arctangent2` | Two-argument arctangent |
| `pow( first, second )` / `power` | Exponentiation |
| `is_multiple( first, second )` | `first % second == 0` ( integral only ) |
| `logical_and( first, second )` / `logical_or` | Non-short-circuit logical combination |
| `shift_left( first, second )` / `shift_right` | Bit shifts ( integral only ) |
| `bit_of( first, second )` | Bit number `second` of `first`: `( first >> second ) & 1` ( integral only ) |

**Unary Niebloids:**

| Name | Purpose |
|------|---------|
| `abs( value )` / `absolute` | Absolute value |
| `negate( value )` | Negation |
| `square( value )` | `value * value` |
| `decrement( value )` / `increment( value )` | Pre-decrement and pre-increment of the value |
| `identity( value )` | Returns the value unchanged |
| `logical_not( value )` | Logical negation |
| `is_even( value )` / `is_odd( value )` | Multiples of 2 ( integral only ) |
| `sign( value )` | Signum: returns -1, 0, or 1 |

**Ternary Niebloids:**

| Name | Purpose |
|------|---------|
| `between( value, low, high )` | Checks `low ≤ value ≤ high` |
| `clamp( value, low, high )` | Clamp between bounds |

**Reductions:**

| Name | Purpose |
|------|---------|
| `sum( range )` | `fold_left( range, 0, plus )` |
| `product( range )` | `fold_left( range, 1, multiplies )` |
| `min( first, second )` / `min( range )` | Minimum, binary or over a range |
| `max( first, second )` / `max( range )` | Maximum, binary or over a range |

**Approximate equality:** `close( first, second, epsilon )` is true when the two values differ by less than `epsilon`, computed through `absolute` so no ordering assumption is needed.

**Math function Niebloids** ( macro-generated wrappers over `<cmath>` ):
- Unary: `acos`, `asin`, `atan`, `ceil`, `cos`, `exp`, `floor`, `log`, `log2`, `round`, `sin`, `sqrt`, `tan`, `tanh`, `trunc`
- Binary: `atan2`, `pow`

These exist so they can be used in range pipelines ( e.g., `range | square_root` ).

**Descriptive aliases:** Each custom and std wrapper also exports a descriptive alias alongside the `sak_< name >` symbol: `absolute` ( abs ), `arccosine`, `arcsine`, `arctangent`, `ceiling`, `cosine`, `exponential`, `round_down`, `natural_logarithm`, `logarithm_base_two`, `round_to_nearest`, `sine`, `square_root` ( sqrt ), `tangent`, `hyperbolic_tangent`, `truncate`, `arctangent2` ( atan2 ), `power` ( pow ).

**Constraint narrowing:** The concept in each macro invocation narrows the scalar overload. `abs`, `negate`, `square` accept any non-range value ( `is_value` ); `sign`, `square_root` and the `<cmath>` wrappers accept any arithmetic type; `is_even`, `is_odd`, `is_multiple`, `bit_of` are restricted to integral types. This keeps range operands routed to the element-wise overload instead of the scalar one.

## `concepts.hpp` — Math Concepts

Defines the concepts used to constrain the Niebloids:
- `is_arithmetic< t_arithmetic >` — alias for `std::is_arithmetic_v`
- `is_integral< t_integral >` — alias for `std::is_integral_v`
- `is_value< t_value >` — `not std::ranges::input_range< t_value >`; routes ranges to the element-wise overload instead of the scalar one
- `is_number< t_number >` — arithmetic and not `bool`
- `is_doublet< t_vector >` — a non-view range with exactly 2 elements ( compile-time check )
- `is_triplet< t_vector >` — a non-view range with exactly 3 elements ( compile-time check )
- `is_crossable< t_vector >` — a doublet, a triplet, or a lazy view ( trusted for size )

## `error.hpp` — Math Error Codes

- `enum class error` — `division_by_zero`, `arithmetic_overflow`, `sqrt_of_negative`
- `struct exception` — derives from `std::exception`, carries an `error` code, and returns a descriptive message from `what( )`.

## `vector.hpp` — Vector Math

Dimension-agnostic vector operations built on top of `math.hpp` and `ranges/operators.hpp`:
- `dot( first, second )` — element-wise product summed; `dot( vector )` is the squared norm
- `length( vector )` — Euclidean norm: `square_root( dot( vector ) )`
- `vector | normalize` — unit vector in the same direction ( direct-application closure )
- `cross( left, right )` / `left | cross( right )` — cross product via cyclic permutation over `rotated`, for `is_crossable` operands
- `vector | rotate( axis, angle )` — Rodrigues rotation around an axis by an angle
