# The `sak` Library

The core of this project is the `sak` library ( Swiss Army Knife ). It is designed as a collection of generic, domain-independent utilities covering mathematics, geometry, ranges, and design patterns.

- **Domain Agnostic:** It contains no business logic or hardware dependencies.
- **Modern Paradigms:** It leverages C++26 features such as `ranges`, `views`, concepts, and custom `Niebloids` to reduce visual noise and promote **functional composition**.
- **Native STL Integration:** All pipeable utilities are native `std::ranges::range_adaptor_closure` objects; the STL's own `operator|` routes every pipeline. There are no custom global `operator|` overloads.

---

## Documentation Freshness Check

To validate whether this documentation is outdated, compare the mtime of this file ( `docs/agent/sak/cpp.md` ) against the mtime of the most recently modified file under `include/sak/` ( recursive ):

```bash
# 1. Find the most recent file under include/sak and get its datetime
date -r "$( find -L include/sak -type f -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d' ' )" '+%Y-%m-%d %H:%M:%S'

# 2. Get this file's datetime in the same format
date -r docs/agent/sak/cpp.md '+%Y-%m-%d %H:%M:%S'
```

If the most recent header mtime is newer than this file's mtime, the documentation is outdated. In that case, **request the user's permission before updating this documentation**.

---

## Range Pipeline Modernization ( C++26 )

Historically, the `sak` library contained a parallel range-pipeline infrastructure built during the Termux ( Android ) era, when the NDK libstdc++ lacked `std::ranges::to`, range adaptor closures, and `std::views::enumerate`. That infrastructure relied on a greedy `sak::operator|` and a `to_point` materialization proxy.

That era is **fully retired**. The project now runs on Arch Linux with GCC 16 and complete C++23/C++26 standard library support. The migration ( see `docs/agent/architecture/range-pipeline-cpp26.md` ) replaced the legacy pipe operators with native C++23 `range_adaptor_closure` objects, and `to_point` was deleted in favor of the universal materializer `sak::ranges::to`.

- **`sak::ranges::to`** replaces `to_point`. It is a native range adaptor closure that defers the target type to the assignment site via a templated implicit conversion operator, keeping pipelines dimension-agnostic ( `point | sin | to` instead of `point | sin | to_point` ).
- **Niebloids inherit from `std::ranges::range_adaptor_closure`**, so `range | sak::math::sin` resolves natively through the STL `operator|`.
- **`sak::point` is a first-class range consumer** via a `std::from_range_t` constructor, so `std::ranges::to< t_target >( )` works natively.
- **No `sak::operator|` free overloads remain.** ADL pollution and the historical `ambiguous overload` errors are eliminated by construction.

---

## Directory Structure

```
include/sak/
  sak.hpp                     # Main header aggregator
  using.hpp                   # __using / __use_macro variadic macros
  default_ctc_dtc.hpp         # Copy/move/dtor macro utilities
  ensure.hpp                  # Runtime assertion utility
  concepts.hpp                # is_callable / is_tuple concepts
  string.hpp                  # String case conversion

  math/
    math.hpp                  # Arithmetic functors, math niebloids
    concepts.hpp              # is_value / is_arithmetic / is_integral / is_number / is_crossable concepts
    error.hpp                 # Math error codes and exception type
    vector.hpp                # Vector math ( length, normalize, cross, rotate )

  pattern/
    dispatcher.hpp            # Observer pattern ( event dispatcher )
    tupled.hpp                # Tuple adaptor for multi-arg functions
    value_or.hpp              # Safe map/sequence lookup with default fallback
    to_number.hpp             # String-to-number parsing with fallback

  ranges/
    to.hpp                    # Universal materializer closure ( replaces to_point )
    operators.hpp             # Element-wise +,-,*,/,% for containers and views
    concepts.hpp              # is_view / is_resizable / is_string_like / is_indirectly_binary_left_foldable
    contains.hpp              # Range containment with braced-list overload
    count_to.hpp              # Integer range [ 0, bound )
    transform.hpp             # eager_transform / lazy_transform aliases
    chunk.hpp                 # Split range into fixed-size subranges
    fold_left_first.hpp       # Fold-left using first element as initial
    views/
      enumerate.hpp           # Zip range with index
      cartesian_product.hpp   # Cartesian product of two ranges
      rotated.hpp             # Circular range rotation

  geometry/
    concepts.hpp              # is_point concept
    point.hpp                 # N-dimensional point ( core class )
    geometry.hpp              # Line/Rectangle composite types
    line_view.hpp             # Bresenham line iterator view
```

---

## Core Infrastructure

### `sak/using.hpp` — Variadic `using` Macro

Provides `__using( prefix, name1, name2, ... )` — a variadic macro that expands to a series of `using prefix::name;` declarations. Supports up to 64 names per invocation. Also provides `__use_macro( macro, ... )` for invoking a macro over multiple argument groups. Reduces visual noise when importing multiple symbols from the same namespace.

### `sak/default_ctc_dtc.hpp` — Constructor/Destructor Macros

Standardized macros for common copy/move constructor/destructor patterns:
- `use_default_copy_ctc`, `use_default_move_ctc`, `use_default_copy_move_ctc`
- `use_non_default_copy_ctc`, `use_non_default_move_ctc`, `use_non_default_copy_move_ctc`
- `delete_copy_ctc`, `delete_move_ctc`, `delete_copy_move_ctc`
- `use_default_dtc`, `use_non_default_dtc`

### `sak/ensure.hpp` — Runtime Assertion

Niebloid `sak::ensure( expression, message )` that throws `std::runtime_error` with the given message when the expression is false. Also exports `sak::exit_success` / `sak::exit_failure`.

### `sak/concepts.hpp` — Core Concepts

- `is_callable< t_callable, t_args... >` — the compiler accepts calling the object via `operator( )`.
- `is_tuple< t_tuple >` — the type is tuple-like ( has `tuple_size` ).

### `sak/sak.hpp` — Main Header

Aggregates `default_ctc_dtc.hpp`, `ensure.hpp`, `concepts.hpp`. Defines `sak::byte` as `uint8_t`.

---

## `sak/math/` — Mathematical Utilities

### `math.hpp` — Arithmetic Operations & Math Functions

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

### `concepts.hpp` — Math Concepts

Defines the concepts used to constrain the Niebloids:
- `is_arithmetic< t_arithmetic >` — alias for `std::is_arithmetic_v`
- `is_integral< t_integral >` — alias for `std::is_integral_v`
- `is_value< t_value >` — `not std::ranges::input_range< t_value >`; routes ranges to the element-wise overload instead of the scalar one
- `is_number< t_number >` — arithmetic and not `bool`
- `is_triplet< t_vector >` — a fixed-size range with exactly 3 elements ( compile-time check )
- `is_crossable< t_vector >` — `is_triplet` or a lazy view ( trusted for size )

### `error.hpp` — Math Error Codes

- `enum class error` — `division_by_zero`, `arithmetic_overflow`, `sqrt_of_negative`
- `struct exception` — derives from `std::exception`, carries an `error` code, and returns a descriptive message from `what( )`.

### `vector.hpp` — Vector Math

Dimension-agnostic vector operations built on top of `math.hpp` and `ranges/operators.hpp`:
- `length( vector )` — Euclidean norm: `square_root( sum( vector * vector ) )`
- `vector | normalize` — unit vector in the same direction ( direct-application closure )
- `cross( left, right )` / `left | cross( right )` — cross product via cyclic permutation, 3-element vectors only ( `is_crossable` )
- `vector | rotate( axis, angle )` — Rodrigues rotation around an axis by an angle

---

## `sak/pattern/` — Design Patterns

### `dispatcher.hpp` — Observer Pattern

Thread-safe event dispatcher. Holds a list of `weak_ptr< t_listener >` and broadcasts a call to a member function pointer to all listeners.

```cpp
sak::pattern::dispatcher< EventListener > events;
events += listener_shared_ptr;
auto result = events( &EventListener::onEvent, arg1, arg2 );
```

- **Thread-safe:** Uses `std::mutex` for list access, `std::atomic` version counter for the cleanup algorithm.
- **Lifetime-aware:** Expired listeners ( weak_ptr lock fails ) are automatically cleaned up on the next dispatch.
- **Error handling:** Returns `expected< void, vector< failed_info > >` — collects exceptions from individual listeners without aborting the broadcast.
- Non-copyable, non-movable.

### `tupled.hpp` — Tuple Adaptor

Converts a multi-argument callable into one that accepts a single tuple argument via `std::apply`. Essential for element-wise operations on zipped ranges:

```cpp
auto sum_of_pairs = zip( first, second ) | transform( tupled( plus ) );
```

### `value_or.hpp` — Safe Lookup

Niebloid that performs a lookup with a default fallback. Supports both associative containers ( `find` ) and sequences ( bounded `operator[]` ):

```cpp
auto val = value_or( my_map, key, default_value );
auto item = value_or( my_vector, index, default_value );
```

### `to_number.hpp` — String Parsing

Niebloid that parses a `std::string` into a numeric type using `std::from_chars`, returning a caller-supplied default when parsing fails:

```cpp
auto val = to_number( text, 42 );
```

---

## `sak/ranges/` — Range Utilities

### `to.hpp` — Universal Materializer

**Active consumption core.** `sak::ranges::to` replaces the retired `to_point`. It is a native `range_adaptor_closure` that produces a proxy holding the range; the proxy converts to *any* target type at the assignment site via a templated implicit conversion operator.

```cpp
point | sin | to      // -> sak::point ( dimension-agnostic )
range | to            // -> any target deduced from the assignment context
```

- The general path delegates to `std::ranges::to< t_target >`.
- `std::array` has no `from_range_t` constructor, so `std::array` gets a direct copy specialization.
- The conversion is `&&`-qualified and intentionally rejects `auto` deduction so the target type is always explicit at the assignment site.

### `operators.hpp` — Element-wise Operators

Provides `+`, `-`, `*`, `/`, `%` ( and their compound `+=`, `-=`, etc. ) for ranges:
- **Containers** ( non-view, non-string-like ): eager result via `std::ranges::transform` into a new container.
- **Views** ( at least one operand is a view ): lazy result via `std::views::zip_transform`.
- Also provides unary negation for containers.

### `concepts.hpp` — Range Concepts

- `is_view` — alias for `std::ranges::view` on the cvref-unwrapped type
- `is_resizable` — has `resize( size_t )`
- `is_string_like` — has `traits_type` ( string-like containers excluded from element-wise operators )
- `is_indirectly_binary_left_foldable` — fold constraint used by `fold_left_first`

### `contains.hpp` — Range Containment

Niebloid `contains( range, value )` mirroring `std::ranges::contains`, plus a braced-list overload `contains( range, { "foo", "bar" } )` that returns true if any listed value is present.

### `count_to.hpp` — Integer Range

Niebloid `count_to( bound )` producing the integer range `[ 0, bound )` via `std::views::iota`, casting the zero to the bound's type so endpoints share an integer type.

### `transform.hpp` — Transform Aliases

Re-exports `eager_transform` ( `std::ranges::transform` ) and `lazy_transform` ( `std::views::transform` ) under the `sak::ranges` namespace.

### `chunk.hpp` — Fixed-Size Chunks

Splits a range into subranges of a given size via a closure object. Returns a view of `subrange`s. Supports pipe syntax via `range_adaptor_closure`.

### `fold_left_first.hpp` — Fold with First Element

Folds a range using the first element as the initial value. Returns `std::optional< t_value >` ( empty if range is empty ). Provides both iterator/sentinel and range overloads.

### Views ( `views/` directory )

| File | Name | STL Equivalent | Purpose |
|------|------|---------------|---------|
| `enumerate.hpp` | `enumerate` | `std::views::enumerate` | Zips a range with an index starting from 0 ( or custom start ) |
| `cartesian_product.hpp` | `cartesian_product` | `std::views::cartesian_product` | Cartesian product of two ranges, returns pairs |
| `rotated.hpp` | `rotated` | *( no direct STL equivalent )* | Circular rotation of a range by an offset; uses concat + drop/take |

**`rotated` details:** Implements circular rotation via `concat( range, range ) | drop( offset % length ) | take( length )`. The default `| rotated` ( no args ) rotates by 1. Exposes a closure for `| rotated( offset )`.

---

## `sak/geometry/` — Geometry Primitives

### `point.hpp` — N-Dimensional Point

**This is the central class of the project.** Its purpose is to free algorithms that use numbers, coordinates, etc. from being tied to a fixed number of dimensions.

```cpp
template< is_arithmetic t_scalar, size_t num_dimensions >
	requires ( num_dimensions > 1 )
class point;
```

#### Design Philosophy

`point` is built with operator overloading and tuple/range compatibility so that business rules and multidimensional formulas can be written agnostically to the number of dimensions. A formula written for 2D points works identically for 3D, 4D, or N-D points without modification.

**Examples of dimension-agnostic code:**
- `end - start` → vector displacement in any dimension
- `point | square | sum | square_root` → Euclidean norm in any dimension
- `point % other` → element-wise modulo in any dimension
- `point * scalar` → uniform scaling in any dimension
- `point | sin | to` → element-wise transform materialized back to a point

#### Implementation

- **Inherits privately from `std::array< t_scalar, num_dimensions >`**, re-exporting its member types and methods ( `begin`, `end`, `size`, `operator[]`, `data`, `fill` ).
- **Default scalar type:** `int`, **default dimensions:** `2`.
- **Variadic constructor:** Accepts exactly `num_dimensions` arguments, each convertible to `t_scalar`.
- **Cross-dimension constructor:** Converts from a point of another scalar/dimension, truncating with `take( num_dimensions )`.
- **Range constructor:** A `std::from_range_t` constructor via `std::ranges::copy`, making `point` a standard-conforming range consumer ( used by `std::ranges::to< t_target >( )` ).

#### Operator Overloads ( via `__352612026_operator` macro )

Each arithmetic operator generates 5 overloads ( compound and binary, range and scalar ):

| Expression | Behavior |
|-----------|----------|
| `first += second` | Element-wise compound assignment |
| `first += scalar` | Compound assignment with scalar ( broadcast via `repeat` ) |
| `first + second` | Element-wise addition |
| `first + scalar` | Add scalar to each dimension |
| `scalar + first` | Scalar broadcast then element-wise add |

Same pattern for `-=`, `*=`, `/=`, `%=`. These delegate to `sak::ranges::eager_transform`.

**Unary negation** returns `*this | negate | to`.

#### Pipe Operators

The legacy `point | invocable` pipe overloads were removed. Points are plain ranges; piping is handled natively by the STL and the `sak::ranges::to` closure:

| Expression | Behavior |
|-----------|----------|
| `range \| to` | Converts any range to a `point< t_scalar, N >` ( target deduced at the assignment site ) |

#### Element-wise Comparison

```cpp
point.is_all( other, operation )
```
Applies `operation( scalar, scalar )` to each pair of dimensions and returns `true` iff all satisfy it.

Building on this:
- `first == second` → `is_all( second, equal_to )`
- `first.is_inside( second )` → `is_all( second, less_equal )` — checks all dimensions of `first` are ≤ corresponding dimensions of `second`.

#### Geometric Queries

- `get_length( )` — Euclidean norm: `length( *this )` — works in any dimension.
- `get_product( )` — product of all elements: `fold_left( *this, 1, multiplies )`.

#### Concept & Type Traits

- `is_point< t_point >` — concept that detects `point` specializations ( including cvref variants ), defined in `geometry/concepts.hpp`.

---

### `geometry.hpp` — High-Level Geometry Types

Parameterized composite geometry built on top of `point`:

```cpp
template< is_point t_point = point< int, 2 > >
struct geometry
{
	using	point	=	t_point;

	struct line
	{
		point start, end;
		constexpr auto get_size( ) const noexcept -> point { return end - start; }
	};

	struct rectangle
	{
		point start, end;
		constexpr auto get_size( ) const noexcept -> point { return end - start; }
		constexpr auto contains( const point& point ) const noexcept -> bool
		{
			return	start.is_inside( point ) and point.is_inside( end );
		}
		constexpr auto is_inside( const rectangle& other ) const noexcept -> bool
		{
			return	other.start.is_inside( start ) and end.is_inside( other.end );
		}
	};
};
```

**Predefined aliases:**
| Alias | Type |
|-------|------|
| `g2i` | `geometry< point< int, 2 > >` |
| `g3i` | `geometry< point< int, 3 > >` |
| `g2f` | `geometry< point< float, 2 > >` |
| `g3f` | `geometry< point< float, 3 > >` |

- `line::get_size( )` → `end - start` ( displacement vector )
- `rectangle::contains( point )` → `start.is_inside( point ) && point.is_inside( end )` ( AABB containment )
- `rectangle::is_inside( rect )` → containment test of one AABB inside another

All methods are dimension-agnostic.

---

### `line_view.hpp` — Bresenham-like Line Iterator

Generates all discrete points along a line from `start` to `end` using a DDA ( Digital Differential Analyzer ) algorithm, dimension-agnostic.

```cpp
auto points = start_point | line_to( end_point );
for( const auto& pixel : points )
	/* pixel is a point along the line */
```

#### Algorithm

- Computes `difference = end - start`
- `walker_step = difference | absolute | to` — absolute step per dimension
- `step = difference | sign | to` — directional step per dimension ( -1, 0, or +1 per axis )
- `total = max( walker_step )` — total number of iterations ( the dominant axis )
- At each step: the walker accumulates `walker_step`; when a dimension's walker exceeds `total`, that dimension "steps" and the walker is corrected.

This works for any point dimension without modification.

#### Implementation Details

- Inherits from `std::ranges::view_interface`
- Uses `default_sentinel` for the end iterator ( infinite sentinel, checked against `m_index >= m_total` )
- Iterator is a `forward_iterator`
- `line_to` is a Niebloid returning a `range_adaptor_closure` bound to the end point

---

## Summary of Dimension-Agnostic Design

The entire geometry module is built on the principle that **no algorithm should care how many dimensions a point has**. Every operation — arithmetic, comparison, geometric queries, line rasterization — is expressed in terms of element-wise operations on the underlying array, which naturally scales to any dimensionality.

This is achieved through:
1. **Private inheritance from `std::array`** — the storage is a fixed-size array parameterized by `num_dimensions`.
2. **Native range adaptor closures** — all pipeable utilities are `std::ranges::range_adaptor_closure` objects routed by the STL `operator|`.
3. **Pipeable Niebloids** — transforms like `square`, `negate`, `abs`, `sign` compose into pipelines: `point | abs | to`.
4. **`sak::ranges::to` materializer** — bridges the gap between lazy range views and eager point construction, deducing the target from the assignment context.
5. **Template parameterization** — both scalar type and dimension count are template parameters with a minimum of 2.
