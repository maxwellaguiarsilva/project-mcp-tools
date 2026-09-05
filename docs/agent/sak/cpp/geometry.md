# `sak/geometry/` — Geometry Primitives

## `point.hpp` — N-Dimensional Point

**This is the central class of the project.** Its purpose is to free algorithms that use numbers, coordinates, etc. from being tied to a fixed number of dimensions.

```cpp
template< is_arithmetic t_scalar, size_t num_dimensions >
	requires ( num_dimensions > 1 )
class point;
```

### Design Philosophy

`point` is built with operator overloading and tuple/range compatibility so that business rules and multidimensional formulas can be written agnostically to the number of dimensions. A formula written for 2D points works identically for 3D, 4D, or N-D points without modification.

**Examples of dimension-agnostic code:**
- `end - start` → vector displacement in any dimension
- `point | square | sum | square_root` → Euclidean norm in any dimension
- `point % other` → element-wise modulo in any dimension
- `point * scalar` → uniform scaling in any dimension
- `point | sin | to` → element-wise transform materialized back to a point

### Implementation

- **Inherits privately from `std::array< t_scalar, num_dimensions >`**, re-exporting its member types and methods ( `begin`, `end`, `size`, `operator[]`, `data`, `fill` ).
- **Default scalar type:** `int`, **default dimensions:** `2`.
- **Variadic constructor:** Accepts exactly `num_dimensions` arguments, each convertible to `t_scalar`.
- **Cross-dimension constructor:** Converts from a point of another scalar/dimension, truncating with `take( num_dimensions )`.
- **Range constructor:** A `std::from_range_t` constructor via `std::ranges::copy`, making `point` a standard-conforming range consumer ( used by `std::ranges::to< t_target >( )` ).

### Operator Overloads ( via `__352612026_operator` macro )

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

### Pipe Operators

The legacy `point | invocable` pipe overloads were removed. Points are plain ranges; piping is handled natively by the STL and the `sak::ranges::to` closure:

| Expression | Behavior |
|-----------|----------|
| `range \| to` | Converts any range to a `point< t_scalar, N >` ( target deduced at the assignment site ) |

### Element-wise Comparison

```cpp
point.is_all( other, operation )
```
Applies `operation( scalar, scalar )` to each pair of dimensions and returns `true` iff all satisfy it.

Building on this:
- `first == second` → `is_all( second, equal_to )`
- `first.is_inside( second )` → `is_all( second, less_equal )` — checks all dimensions of `first` are ≤ corresponding dimensions of `second`.

### Geometric Queries

- `get_length( )` — Euclidean norm: `length( *this )` — works in any dimension.
- `get_product( )` — product of all elements: `fold_left( *this, 1, multiplies )`.

### Concept & Type Traits

- `is_point< t_point >` — concept that detects `point` specializations ( including cvref variants ), defined in `geometry/concepts.hpp`.

---

## `geometry.hpp` — High-Level Geometry Types

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

## `line_view.hpp` — Bresenham-like Line Iterator

Generates all discrete points along a line from `start` to `end` using a DDA ( Digital Differential Analyzer ) algorithm, dimension-agnostic.

```cpp
auto points = start_point | line_to( end_point );
for( const auto& pixel : points )
	/* pixel is a point along the line */
```

### Algorithm

- Computes `difference = end - start`
- `walker_step = difference | absolute | to` — absolute step per dimension
- `step = difference | sign | to` — directional step per dimension ( -1, 0, or +1 per axis )
- `total = max( walker_step )` — total number of iterations ( the dominant axis )
- At each step: the walker accumulates `walker_step`; when a dimension's walker exceeds `total`, that dimension "steps" and the walker is corrected.

This works for any point dimension without modification.

### Implementation Details

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
