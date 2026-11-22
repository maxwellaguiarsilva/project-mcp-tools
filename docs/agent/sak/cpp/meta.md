# `sak/meta/` — Compile-Time Reflection

Header-only, domain-agnostic helpers built on `std::meta`. The folder answers two reflection questions that consumers would otherwise encode as hand-written tables: which virtual methods an interface declares and an implementation overrides, and which enumerator a name denotes. Both headers are pure reflection plus splices, carry no hardware or business dependency, and leave the meaning of the reflected data to the consumer. The folder has no aggregator header; a translation unit includes the individual header it needs.

| Header | Subject |
|---|---|
| `override.hpp` | virtual method reflection: method enumeration, override detection, the `override_model` mask, and method pointers |
| `enum.hpp` | enumerator reflection: `kind_of` maps a name to an enumerator, `name_of` maps an enumerator to its name |

---

## `override.hpp` — Virtual Method Reflection

Declared in `include/sak/meta/override.hpp`. Every entry point is `consteval`, so each answer is produced during compilation. Membership is read with `std::meta::members_of( ^^type, access_context::current( ) )`, which means the answer respects the access context of the point of use and never reads a member the consumer could not name.

### `virtual_methods< t_base >( )`

Returns `std::vector< std::meta::info >` holding every virtual method declared by `t_base`, in declaration order.

- A member qualifies when it is a function, is not a special member function, and is virtual.
- Constructors, destructors, and assignment operators are excluded by the special-member check.
- Non-virtual functions, data members, and inherited members are excluded; the scan covers the declarations of `t_base` itself.
- The returned order is the declaration order of `t_base`, which is the order every downstream position refers to.

### `overridden_methods< t_base, t_derived >( )`

Returns `std::vector< std::meta::info >` holding the **base-side** methods that `t_derived` overrides.

- The outer walk visits `virtual_methods< t_base >( )`, so the result follows the base declaration order.
- For each base method, the derived members are scanned for a function marked `std::meta::is_override` whose identifier equals the base method identifier.
- Both sides must have an identifier; an unnamed member is skipped.
- The scan stops at the first matching derived declaration for that base method, and the base method is the value that is recorded.
- Matching is by identifier only, so it reports the pairing and says nothing about signatures or exception specifications.

### `is_overridden< t_base, t_derived >( const string_view name ) -> bool`

Answers whether a base method with that identifier is overridden, by comparing `name` against the identifiers from `overridden_methods< t_base, t_derived >( )`. It is the lookup form used when the consumer holds the name as data rather than as an `info`.

A second overload takes the method at compile time instead of by name:

```cpp
template< typename t_base, typename t_derived, info t_method >
consteval auto is_overridden( ) { return is_overridden< t_base, t_derived >( identifier_of( t_method ) ); }
```

### `overridden_names< t_base, t_derived >( )`

Returns a static array of `const char*`, one identifier per overridden base method, in base declaration order. Each identifier is materialized through `define_static_string` and the array through `define_static_array`, so the names are usable as ordinary static data.

### `override_model< t_interface, t_implementation = t_interface >`

A reflective model that pairs an interface with an implementation and exposes the pairing as a mask. The default argument makes the single-argument form describe the interface against itself.

```cpp
using	override_mask	=	bitset< virtual_methods< t_interface >( ).size( ) >;
```

| Member | Result |
|---|---|
| `methods( )` | `virtual_methods< t_interface >( )` |
| `position( const info t_method )` | the index of that method in `methods( )`, obtained by dereferencing `sak::ranges::index_of` |
| `overridden( )` | `overridden_methods< t_interface, t_implementation >( )` |
| `mask( )` | an `override_mask` with one bit set at the `position` of every overridden method |

The bit order is the interface declaration order, so bit `n` always denotes the same method for a given interface. A mask is therefore comparable across implementations, and a difference between two masks names the methods one implementation overrides and the other does not.

### `method_pointer< info t_method >( )`

Returns `&[: t_method :]` — a real pointer to the method the `info` designates, produced by splicing. The reflected method becomes callable without transcribing its name, so a name-level decision and a call site can be driven by the same `info`.

---

## `enum.hpp` — Enumerator Names

Declared in `include/sak/meta/enum.hpp`. The header provides the two directions of one reflection over any enumeration type:

```cpp
template< typename t_enum >
	requires is_enum_v< t_enum >
auto kind_of( const string_view name ) -> t_enum;

template< typename t_enum >
	requires is_enum_v< t_enum >
auto name_of( const t_enum kind ) -> string_view;
```

### `kind_of( const string_view name )`

Maps an enumerator name to the enumerator it declares.

- The walk is `template for` over `define_static_array( enumerators_of( ^^t_enum ) )`, so each enumerator is visited as a compile-time value.
- An enumerator matches when `identifier_of( enumerator )` equals `name`; the match assigns `[: enumerator :]` to the result.
- The `found` flag keeps the first match; later enumerators do not overwrite it.
- An unknown name is rejected through `sak::ensure` with `unknown enumerator: <name>`.

### `name_of( const t_enum kind )`

Returns the enumerator identifier for a value.

- The same `define_static_array( enumerators_of( ^^t_enum ) )` walk compares `[: enumerator :]` against `kind` and records `identifier_of( enumerator )`.
- An unknown value is rejected through `sak::ensure` with `unknown enumerator value`.

### One Source of Truth

Both directions walk the same `define_static_array( enumerators_of( ^^t_enum ) )`. The name and the value remain two views of the same enumerator, so no header, table, or switch repeats an enumeration that already exists in the type, and adding an enumerator needs no edit outside the enumeration itself.

The two error messages are deliberately domain-neutral: they name the failure and the offending text, and nothing about the consumer's vocabulary. A consumer that owns a user-facing vocabulary is expected to add its own context when the message reaches the user, so the same reflection serves every domain without embedding any of them.

---

## Tests

No structured test exists for this folder: `find -L /home/maxwell/code/draft-point/tests -name "test_sak_meta*"` returns nothing, and `tests/` holds only the `adhoc/` tree. The headers reach execution through their consumers; `sak::opengl::shader::compile` exercises `kind_of` on `shader::type`, and the demo resolves a stage name through `name_of`.
