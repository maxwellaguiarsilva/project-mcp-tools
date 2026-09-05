# `sak/pattern/` — Design Patterns

## `dispatcher.hpp` — Observer Pattern

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

## `tupled.hpp` — Tuple Adaptor

Converts a multi-argument callable into one that accepts a single tuple argument via `std::apply`. Essential for element-wise operations on zipped ranges:

```cpp
auto sum_of_pairs = zip( first, second ) | transform( tupled( plus ) );
```

## `value_or.hpp` — Safe Lookup

Niebloid that performs a lookup with a default fallback. Supports both associative containers ( `find` ) and sequences ( bounded `operator[]` ):

```cpp
auto val = value_or( my_map, key, default_value );
auto item = value_or( my_vector, index, default_value );
```

## `to_number.hpp` — String Parsing

Niebloid that parses a `std::string` into a numeric type using `std::from_chars`, returning a caller-supplied default when parsing fails:

```cpp
auto val = to_number( text, 42 );
```

---

# `sak::pattern::bitmask` Design Specification

This document records the design decisions and architectural specifications established for `sak::pattern::bitmask`, a generic template wrapper for scoped enums (`enum class`) representing bit flags.

## Overview and Purpose

`sak::pattern::bitmask< t_enum >` is a domain-agnostic utility designed to manage bitmask operations on `enum class` types safely, declaratively, and without macro or global operator pollution.

- **Scope:** Placed under namespace `sak::pattern` in `include/sak/pattern/bitmask.hpp`.
- **Target type:** Constrained to enumeration types (`std::is_enum_v< t_enum >`).
- **Bit representation responsibility:** The consuming `enum class` is responsible for defining its enumerators with the appropriate power-of-two bit patterns (e.g., `0x01`, `0x02`, `0x04`, etc.). The bitmask operates directly on these underlying values.

## Storage and Representation

- **Storage type:** Automatically deduced from the underlying type of the enum using `std::underlying_type_t< t_enum >`.
- **Internal member:** Stored as an integer of the deduced underlying type (e.g., `m_value`), initialized to `0` by default.
- **Bitwise mechanics:**
  - `use` performs bitwise OR (`m_value |= value`).
  - `remove` clears bits via bitwise AND NOT (`m_value &= ~value`).
  - `toggle` inverts bits via bitwise XOR (`m_value ^= value`).
  - `clear` resets the state to `0`.
  - `all` evaluates whether all tested bits are set (AND logic).
  - `any` evaluates whether at least one tested bit is set (OR logic).
  - `value` (and the conversion operator) read the current raw `m_value`.

The mutators and predicates are generated from shared templates (macros): each provides a variadic overload accepting one or more `t_enum` flags that forwards to a single-`initializer_list` core. Mutator cores fold a ranges pipeline over `m_value` (OR, AND of negated flags for `remove`, XOR), while predicate cores delegate to `std::ranges::all_of`/`any_of` over the private `is_set` predicate.

## Method Specifications

All mutator methods return `void`. A fluent interface was explicitly considered and rejected; neither `use`, `remove`, `toggle`, nor `clear` return `*this`.

### `use`
- **Purpose:** Activates one or more flags.
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`. Single-flag calls naturally match this overload.
  - Initializer list: accepts `std::initializer_list< t_enum >` for braced syntax (`mask.use( { flag::a, flag::b } )`).
- **Return type:** `void`.

### `remove`
- **Purpose:** Deactivates one or more flags.
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `void`.

### `toggle`
- **Purpose:** Alternates the state of one or more flags (flips active bits).
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `void` (a `bool` return for the new state was evaluated and discarded to prevent ambiguity when toggling multiple flags).

### `all`
- **Purpose:** Queries whether **all** specified flags are active (AND logic).
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `bool`.
- **Condition:** Returns `true` if and only if **every** supplied flag is set in the mask.

### `any`
- **Purpose:** Queries whether **at least one** of the specified flags is active (OR logic).
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `bool`.
- **Condition:** Returns `true` if **any** (one or more) of the supplied flags is set in the mask.

### `clear`
- **Purpose:** Clears all active flags, resetting the mask to zero.
- **Signatures:** Takes no arguments (`clear( )`).
- **Return type:** `void`.

### `value`
- **Purpose:** Exposes the current raw underlying integer state.
- **Signatures:** Takes no arguments (`value( )`), `const` qualified.
- **Return type:** `underlying_type`.

### Conversion operator
- **Purpose:** Read-only view of the current raw state.
- **Signatures:** `explicit operator underlying_type( )`, `const` qualified.
- **Return type:** `underlying_type`.
- **Condition:** Must be explicitly requested (no implicit conversions to integer).

## Constructors

- **Default constructor:** Initializes `m_value` to `0` (empty mask).
- **Variadic constructor:** Mirrors the `use` method; allows initializing the bitmask with one or more initial flags (e.g., `bitmask< flag > mask{ flag::read, flag::write }`).
- **Initializer list constructor:** Accepts `std::initializer_list< t_enum >` to permit braced initialization lists.

## Summary Matrix of Decisions

| Aspect | Decision | Notes |
|---|---|---|
| **Underlying Type** | Auto-deduced | Deduced via `std::underlying_type_t< t_enum >` |
| **Bit Values** | Consumer responsibility | Enum values must be powers of two |
| **Storage Member** | Deduce underlying, init `0` | E.g., `underlying_type m_value{ 0 }` |
| **Fluent Interface** | No | `use`, `remove`, `toggle`, and `clear` return `void` |
| **`use`** | `void` | Overloads for variadic `1+` and `initializer_list` |
| **`remove`** | `void` | Clears bits; variadic `1+` and `initializer_list` |
| **`toggle`** | `void` | Flips bits; variadic `1+` and `initializer_list` |
| **`all`** | `bool` | Returns `true` only if **all** tested flags are active (AND) |
| **`any`** | `bool` | Returns `true` if **any** tested flag is active (OR) |
| **`clear`** | `void` | No arguments; resets mask to `0` |
| **`value`** | `underlying_type` | No arguments; reads raw state |
| **Conversion** | `explicit operator underlying_type` | No arguments; reads raw state |
| **Constructors** | Default + matching `use` | Default empty, variadic `1+`, and `initializer_list` |
