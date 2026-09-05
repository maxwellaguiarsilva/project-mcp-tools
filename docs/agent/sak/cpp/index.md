# The `sak` Library

The core of this project is the `sak` library ( Swiss Army Knife ). It is designed as a collection of generic, domain-independent utilities covering mathematics, geometry, ranges, and design patterns.

- **Domain Agnostic:** It contains no business logic or hardware dependencies.
- **Modern Paradigms:** It leverages C++26 features such as `ranges`, `views`, concepts, and custom `Niebloids` to reduce visual noise and promote **functional composition**.
- **Native STL Integration:** All pipeable utilities are native `std::ranges::range_adaptor_closure` objects; the STL's own `operator|` routes every pipeline. There are no custom global `operator|` overloads.

---

## Documentation Freshness Check

To validate whether this documentation is outdated, compare the mtime of this file ( `docs/agent/sak/cpp/index.md` ) against the mtime of the most recently modified file under `include/sak/` ( recursive ):

```bash
# 1. Find the most recent file under include/sak and get its datetime
date -r "$( find -L include/sak -type f -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d' ' )" '+%Y-%m-%d %H:%M:%S'

# 2. Get this file's datetime in the same format
date -r docs/agent/sak/cpp/index.md '+%Y-%m-%d %H:%M:%S'
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
    bitmask.hpp               # Scoped-enum bit flag wrapper

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

## Modules

The documentation is distributed across one file per module directory under `include/sak/`:

- [Core Infrastructure](core.md) — `using`, `default_ctc_dtc`, `ensure`, `concepts`, `sak.hpp`.
- [`sak/math/`](math.md) — arithmetic operations, math functions, math concepts, vector math.
- [`sak/pattern/`](pattern.md) — design patterns: dispatcher, tupled, value_or, to_number, bitmask.
- [`sak/ranges/`](ranges.md) — range utilities and views.
- [`sak/geometry/`](geometry.md) — geometry primitives and the `point` core class.
