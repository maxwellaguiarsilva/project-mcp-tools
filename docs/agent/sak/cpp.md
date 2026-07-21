# The `sak` Library

The core of this project is the `sak` library (Swiss Army Knife). It is designed as a collection of generic, domain-independent utilities covering mathematics, geometry, and design patterns.

- **Domain Agnostic:** It contains no business logic or hardware dependencies.
- **Modern Paradigms:** It leverages C++ features such as `ranges`, `views`, and custom `Niebloids` to reduce visual noise and promote **functional composition**.

---

## Codebase Modernization (C++23 STL)

Historically, the `sak` library contained several custom implementations of C++23 ranges and views. These were necessary when the project was developed on Termux (Android) due to the limited standard library support of the NDK/Clang environment at the time.

### Custom STL Reproductions in `sak`:
1. `sak::ranges::views::enumerate` (reproduces `std::views::enumerate`)
2. `sak::ranges::views::join_with` (reproduces `std::views::join_with`)
3. `sak::ranges::chunk` (reproduces `std::views::chunk`)
4. `sak::ranges::fold_left_first` (reproduces `std::ranges::fold_left_first`)
5. `sak::ranges::to_array` (reproduces `std::ranges::to<std::array>`)
6. `sak::ranges::views::cartesian_product` (reproduces `std::views::cartesian_product`)

### Optimization Strategy

Now that the project is developed on Arch Linux with full C++23 standard library support (GCC 16), actively consuming these custom implementations violates **DRY** (Don't Repeat Yourself) and adds unnecessary visual noise, violating **DDD** (Domain-Driven Design).

- **Rule:** Keep the custom `sak` implementations in the library as **educational examples** (they must not be removed), but **migrate all active consumption** in the codebase to use the native C++23 standard library equivalents (`std::views::*` and `std::ranges::*`).
- **Exception:** `sak::ranges::to_array` is kept as-is and actively consumed. `std::ranges::to` does not support `std::array` (fixed-size aggregate), so `sak::ranges::to_array` remains the single consumption exception.
- **Testing:** Unit tests that validate the `sak` helpers keep consuming them by design to ensure they still work as educational references.

---

## Directory Structure

```
include/sak/
  sak.hpp                     # Main header aggregator
  using.hpp                   # __using variadic macro
  default_ctc_dtc.hpp         # Copy/move/dtor macro utilities
  ensure.hpp                  # Runtime assertion utility
  string.hpp                  # String case conversion

  math/
    math.hpp                  # Arithmetic functors, math niebloids, concepts

  pattern/
    dispatcher.hpp            # Observer pattern (event dispatcher)
    tupled.hpp                # Tuple adaptor for multi-arg functions
    value_or.hpp              # Safe map lookup with default fallback

  ranges/
    chunk.hpp                 # Split range into fixed-size subranges
    fold_left_first.hpp       # Fold-left using first element as initial
    to_array.hpp              # Pipeable range-to-array conversion
    views/
      enumerate.hpp           # Zip range with index
      join_with.hpp           # Join elements with delimiter
      cartesian_product.hpp   # Cartesian product of two ranges
      rotated.hpp             # Circular range rotation

  geometry/
    point.hpp                 # N-dimensional point (core class)
    geometry.hpp              # Line/Rectangle composite types
    line_view.hpp             # Bresenham line iterator view
```

---

## Core Infrastructure

### `sak/using.hpp` — Variadic `using` Macro

Provides `__using(prefix, name1, name2, ...)` — a variadic macro that expands to a series of `using prefix::name;` declarations. Supports up to 64 names per invocation. Reduces visual noise when importing multiple symbols from the same namespace.

### `sak/default_ctc_dtc.hpp` — Constructor/Destructor Macros

Standardized macros for common copy/move constructor/destructor patterns:
- `use_default_copy_ctc`, `use_default_move_ctc`, `use_default_copy_move_ctc`
- `delete_copy_ctc`, `delete_move_ctc`, `delete_copy_move_ctc`
- `use_default_dtc`, `use_non_default_dtc`

### `sak/ensure.hpp` — Runtime Assertion

Niebloid `sak::ensure(expression, message)` that throws `std::runtime_error` with the given message when the expression is false. Also exports `sak::exit_success` / `sak::exit_failure`.

### `sak/sak.hpp` — Main Header

Aggregates `using.hpp`, `default_ctc_dtc.hpp`, `ensure.hpp`. Defines `sak::byte` as `uint8_t`.

---

## `sak/math/` — Mathematical Utilities

### `math.hpp` — Arithmetic Operations & Math Functions

Provides niebloid wrappers for arithmetic and mathematical operations. These are function objects (niebloids) rather than raw functions, making them pipeable and composable in range pipelines.

**Arithmetic functors** (thin wrappers over `std` equivalents):
- `plus`, `minus`, `negate`, `multiplies`, `divides`, `modulus`
- `equal_to`, `less_equal`, `greater_equal`

**Error handling:**
- `enum class error` — `division_by_zero`, `arithmetic_overflow`, `sqrt_of_negative`
- `class exception` — derives from `std::exception`, carries an `error` code

**Concepts:**
- `is_arithmetic<T>` — alias for `std::is_arithmetic_v`

**Custom niebloids:**
| Name | Purpose |
|------|---------|
| `between(value, low, high)` | Checks `low ≤ value ≤ high` |
| `sum(range)` | `fold_left(range, 0, plus)` |
| `square(value)` | `value * value` |
| `abs(value)` | Absolute value |
| `sign(value)` | Signum: returns -1, 0, or 1 |
| `min(a, b)` / `min(range)` | Minimum (binary or over range via `std::ranges::min`) |
| `max(a, b)` / `max(range)` | Maximum (binary or over range) |
| `clamp(value, low, high)` | Clamp between bounds |
| `is_multiple(a, b)` | `a % b == 0` |
| `is_even(value)` | Multiple of 2 |
| `is_odd(value)` | Not even |

**Math function niebloids** (macro-generated wrappers over `<cmath>`):
- Unary: `acos`, `asin`, `atan`, `ceil`, `cos`, `exp`, `floor`, `log`, `log2`, `round`, `sin`, `sqrt`, `tan`, `trunc`
- Binary: `atan2`, `pow`

These exist so they can be used in range pipelines (e.g., `range | transform(sak::math::sqrt)`).

---

## `sak/pattern/` — Design Patterns

### `dispatcher.hpp` — Observer Pattern

Thread-safe event dispatcher. Holds a list of `weak_ptr<t_listener>` and broadcasts a call to a member function pointer to all listeners.

```cpp
sak::pattern::dispatcher<EventListener> events;
events += listener_shared_ptr;
auto result = events(&EventListener::onEvent, arg1, arg2);
```

- **Thread-safe:** Uses `std::mutex` for list access, `std::atomic` version counter for the cleanup algorithm.
- **Lifetime-aware:** Expired listeners (weak_ptr lock fails) are automatically cleaned up on the next dispatch.
- **Error handling:** Returns `expected<void, vector<failed_info>>` — collects exceptions from individual listeners without aborting the broadcast.
- Non-copyable, non-movable.

### `tupled.hpp` — Tuple Adaptor

Converts a multi-argument callable into one that accepts a single tuple argument via `std::apply`. Essential for element-wise operations on zipped ranges:

```cpp
auto sum_of_pairs = zip(a, b) | transform(tupled(plus));
```

Also defines `invocable` and `is_tuple` concepts.

### `value_or.hpp` — Safe Map Lookup

Niebloid that performs a map lookup with a default fallback:

```cpp
auto val = value_or(my_map, key, default_value);
```

Equivalent to `my_map.contains(key) ? my_map[key] : default_value`.

---

## `sak/ranges/` — Range Utilities

These are custom reproductions of C++23 STL features. They are kept as educational references; active consumption should use `std::ranges::` / `std::views::` equivalents.

### `chunk.hpp` — Fixed-Size Chunks

Reproduces `std::views::chunk`. Splits a range into subranges of a given size. Returns a view of `subrange`s. Supports pipe syntax via `range_adaptor_closure`.

### `fold_left_first.hpp` — Fold with First Element

Reproduces `std::ranges::fold_left_first`. Folds a range using the first element as the initial value. Returns `std::optional<T>` (empty if range is empty).

### `to_array.hpp` — Range-to-Array Conversion

**Active consumption exception.** Converts a range to a `std::array` via pipe operator: `range | to_array`. Unlike `std::ranges::to`, this supports fixed-size `std::array`. The array size is deduced from the target type at the conversion site.

### Views (`views/` directory)

| File | Name | STL Equivalent | Purpose |
|------|------|---------------|---------|
| `enumerate.hpp` | `enumerate` | `std::views::enumerate` | Zips a range with an index starting from 0 (or custom start) |
| `join_with.hpp` | `join_with` | `std::views::join_with` | Joins elements of a range with a delimiter between them |
| `cartesian_product.hpp` | `cartesian_product` | `std::views::cartesian_product` | Cartesian product of two ranges, returns pairs |
| `rotated.hpp` | `rotated` | *(no direct STL equivalent)* | Circular rotation of a range by an offset; uses double-via-join technique |

**`rotated` details:** Implements circular rotation by concatenating the range with itself via an `array` of views + `join`, then `drop(offset) | take(length)`. The default `| rotated` (no args) rotates by 1. Also has a generic `bitor` operator for range closures.

---

## `sak/geometry/` — Geometry Primitives

### `point.hpp` — N-Dimensional Point

**This is the central class of the project.** Its purpose is to free algorithms that use numbers, coordinates, etc. from being tied to a fixed number of dimensions.

```cpp
template<is_arithmetic t_scalar, size_t num_dimensions>
requires (num_dimensions > 1)
class point;
```

#### Design Philosophy

`point` is built with operator overloading and tuple/range compatibility so that business rules and multidimensional formulas can be written agnostically to the number of dimensions. A formula written for 2D points works identically for 3D, 4D, or N-D points without modification.

**Examples of dimension-agnostic code:**
- `end - start` → vector displacement in any dimension
- `point | square | sum | sqrt` → Euclidean norm in any dimension
- `point % other` → element-wise modulo in any dimension
- `point * scalar` → uniform scaling in any dimension
- `point | transform(f)` → apply f to each dimension

#### Implementation

- **Inherits privately from `std::array<t_scalar, num_dimensions>`**, re-exporting its member types and methods (`begin`, `end`, `size`, `operator[]`, `data`, `fill`).
- **Default scalar type:** `int`, **default dimensions:** `2`.
- **Variadic constructor:** Accepts exactly `num_dimensions` arguments, each convertible to `t_scalar`.

#### Operator Overloads (via `__352612026_operator` macro)

Each arithmetic operator generates 5 overloads:

| Expression | Behavior |
|-----------|----------|
| `a += b` | Element-wise compound assignment |
| `a += scalar` | Compound assignment with scalar (broadcast) |
| `a + b` | Element-wise addition |
| `a + scalar` | Add scalar to each dimension |
| `scalar + a` | Scalar broadcast then element-wise add |

Same pattern for `-=`, `*=`, `/=`, `%=`.

**Unary negation** returns a view transformed by `negate`, then converted back to point via `to_point`.

#### Pipe Operators

| Expression | Behavior |
|-----------|----------|
| `range \| to_point` | Converts any `input_range` to a `point<t_scalar, N>`. The target type is deduced at the conversion site (implicit conversion operator). |
| `point \| invocable` | Returns a `transform_view` over the point's elements (lazy). |
| `range \| invocable` | Same, but for non-point ranges (disabled for points to avoid ambiguity). |

#### Element-wise Comparison

```cpp
point.is_all(other, operation)
```
Applies `operation(scalar, scalar)` to each pair of dimensions and returns `true` iff all satisfy it.

Building on this:
- `a == b` → `is_all(b, equal_to)`
- `a.is_inside(b)` → `is_all(b, less_equal)` — checks all dimensions of `a` are ≤ corresponding dimensions of `b`.

#### Geometric Queries

- `get_length()` — Euclidean norm: `sqrt(sum(point | square))` — works in any dimension.
- `get_product()` — product of all elements: `fold_left(point, 1, multiplies)`.

#### Concept & Type Traits

- `is_point<T>` — concept that detects `point` specializations (including cvref variants).
- `__point_from<range>` — proxy type that converts a range into a point via `std::ranges::copy`.

---

### `geometry.hpp` — High-Level Geometry Types

Parameterized composite geometry built on top of `point`:

```cpp
template<is_point t_point = point<int, 2>>
struct geometry {
    struct line       { point start, end; point get_size(); };
    struct rectangle  { point start, end; point get_size();
                        bool contains(point); bool is_inside(rectangle); };
};
```

**Predefined aliases:**
| Alias | Type |
|-------|------|
| `g2i` | `geometry<point<int, 2>>` |
| `g3i` | `geometry<point<int, 3>>` |
| `g2f` | `geometry<point<float, 2>>` |
| `g3f` | `geometry<point<float, 3>>` |

- `line::get_size()` → `end - start` (displacement vector)
- `rectangle::contains(point)` → `start.is_inside(point) && point.is_inside(end)` (AABB containment)
- `rectangle::is_inside(rect)` → containment test of one AABB inside another

All methods are dimension-agnostic.

---

### `line_view.hpp` — Bresenham-like Line Iterator

Generates all discrete points along a line from `start` to `end` using a DDA (Digital Differential Analyzer) algorithm, dimension-agnostic.

```cpp
auto points = start_point | line_to(end_point);
for (auto p : points) { /* p is a point along the line */ }
```

#### Algorithm

- Computes `difference = end - start`
- `walker_step = difference | abs | to_point` — absolute step per dimension
- `step = difference | sign | to_point` — directional step per dimension (-1, 0, or +1 per axis)
- `total = max(walker_step)` — total number of iterations (the dominant axis)
- At each step: the walker accumulates `walker_step`; when a dimension's walker exceeds `total`, that dimension "steps" and the walker is corrected.

This works for any point dimension without modification.

#### Implementation Details

- Inherits from `std::ranges::view_interface`
- Uses `default_sentinel` for the end iterator (infinite sentinel, checked against `m_index >= m_total`)
- Iterator is a `forward_iterator`

---

## Summary of Dimension-Agnostic Design

The entire geometry module is built on the principle that **no algorithm should care how many dimensions a point has**. Every operation — arithmetic, comparison, geometric queries, line rasterization — is expressed in terms of element-wise operations on the underlying array, which naturally scales to any dimensionality.

This is achieved through:
1. **Private inheritance from `std::array`** — the storage is a fixed-size array parameterized by `num_dimensions`.
2. **Operator overloading powered by `std::ranges::transform`** — all arithmetic operators apply element-wise operations via range algorithms.
3. **Pipeable niebloids** — transforms like `square`, `negate`, `abs`, `sign` compose into pipelines: `point | abs | to_point`.
4. **`to_point` conversion** — bridges the gap between lazy range views and eager point construction.
5. **Template parameterization** — both scalar type and dimension count are template parameters with a minimum of 2.
