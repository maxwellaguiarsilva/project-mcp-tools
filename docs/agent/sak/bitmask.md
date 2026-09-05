# `sak::pattern::bitmask` Design Specification

This document records the design decisions and architectural specifications established for `sak::pattern::bitmask`, a generic template wrapper for scoped enums (`enum class`) representing bit flags.

---

## 1. Overview and Purpose

`sak::pattern::bitmask< t_enum >` is a domain-agnostic utility designed to manage bitmask operations on `enum class` types safely, declaratively, and without macro or global operator pollution.

- **Scope:** Placed under namespace `sak::pattern` in `include/sak/pattern/bitmask.hpp`.
- **Target type:** Constrained to enumeration types (`std::is_enum_v< t_enum >`).
- **Bit representation responsibility:** The consuming `enum class` is responsible for defining its enumerators with the appropriate power-of-two bit patterns (e.g., `0x01`, `0x02`, `0x04`, etc.). The bitmask operates directly on these underlying values.

---

## 2. Storage and Representation

- **Storage type:** Automatically deduced from the underlying type of the enum using `std::underlying_type_t< t_enum >`.
- **Internal member:** Stored as an integer of the deduced underlying type (e.g., `m_value`), initialized to `0` by default.
- **Bitwise mechanics:**
  - `use` performs bitwise OR (`m_value |= value`).
  - `remove` clears bits via bitwise AND NOT (`m_value &= ~value`).
  - `toggle` inverts bits via bitwise XOR (`m_value ^= value`).
  - `clear` resets the state to `0`.
  - `all` evaluates whether all tested bits are set (AND logic).
  - `any` evaluates whether at least one tested bit is set (OR logic).

---

## 3. Method Specifications

All mutator methods return `void`. A fluent interface was explicitly considered and rejected; neither `use`, `remove`, `toggle`, nor `clear` return `*this`.

### 3.1 `use`
- **Purpose:** Activates one or more flags.
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`. Single-flag calls naturally match this overload.
  - Initializer list: accepts `std::initializer_list< t_enum >` for braced syntax (`mask.use( { flag::a, flag::b } )`).
- **Return type:** `void`.

### 3.2 `remove`
- **Purpose:** Deactivates one or more flags.
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `void`.

### 3.3 `toggle`
- **Purpose:** Alternates the state of one or more flags (flips active bits).
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `void` (a `bool` return for the new state was evaluated and discarded to prevent ambiguity when toggling multiple flags).

### 3.4 `all`
- **Purpose:** Queries whether **all** specified flags are active (AND logic).
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `bool`.
- **Condition:** Returns `true` if and only if **every** supplied flag is set in the mask.

### 3.5 `any`
- **Purpose:** Queries whether **at least one** of the specified flags is active (OR logic).
- **Signatures:**
  - Variadic: accepts one or more arguments of type `t_enum`.
  - Initializer list: accepts `std::initializer_list< t_enum >`.
- **Return type:** `bool`.
- **Condition:** Returns `true` if **any** (one or more) of the supplied flags is set in the mask.

### 3.6 `clear`
- **Purpose:** Clears all active flags, resetting the mask to zero.
- **Signatures:** Takes no arguments (`clear( )`).
- **Return type:** `void`.

---

## 4. Constructors

- **Default constructor:** Initializes `m_value` to `0` (empty mask).
- **Variadic constructor:** Mirrors the `use` method; allows initializing the bitmask with one or more initial flags (e.g., `bitmask< flag > mask{ flag::read, flag::write }`).
- **Initializer list constructor:** Accepts `std::initializer_list< t_enum >` to permit braced initialization lists.

---

## 5. Summary Matrix of Decisions

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
| **Constructors** | Default + matching `use` | Default empty, variadic `1+`, and `initializer_list` |
