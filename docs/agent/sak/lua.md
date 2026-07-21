# Lua SAK

The Lua `sak` library is a custom OOP + utility framework that extends Lua's standard library. It injects **globals** (no `local` needed): `stack`, `class`, `table`, `string` extensions, and all `is_*` / `ensure_*` / `coalesce_*` type helpers.

## Components

### `stack` (`lua/sak/stack.lua`) — Module Path Stack

A global table-with-metatable that builds dotted module paths for `require`.

```lua
stack:require[[profile]]       -- requires "sak.profile" (pushes "profile" onto stack)
stack:require{ "safe", "table" } -- requires multiple modules, returns results list
stack:local_require[[class]]   -- requires "class" directly (no "sak." prefix)
```

The stack tracks the current namespace prefix. `tostring(stack)` returns the joined path (e.g., `"sak.table"`).

### `safe` (`lua/sak/safe.lua`) — Type Helpers & Safe Calls

Generates global type functions for each Lua type:

| Pattern            | Example              | Description                          |
|--------------------|----------------------|--------------------------------------|
| `is_<type>(x)`     | `is_string(x)`       | Type check: `type(x) == "string"`    |
| `ensure_<type>(x, alt)` | `ensure_table(x, {})` | Returns `x` if correct type, else `alt`, else type default |
| `coalesce_<type>(...)`  | `coalesce_string(a, b)` | Returns first argument matching type |

Additional globals:

- `is_nil(x)`, `is_callable(x)` (function or table with `__call`)
- `safe_call(fn, ...)` — calls `fn(...)` only if callable, else returns nil
- `safe_index(t, k)` — returns `t[k]` only if `t` is a table
- `safe_table_call(t, k, ...)` — `safe_call(safe_index(t, k), ...)`
- `coalesce(...)` — returns first non-nil argument
- `safe_caller(fn, callback)` — wraps `fn` in `pcall` (or `xpcall` if callback given), returns a new function

### `table` (`lua/sak/table/`) — OOP Table Extensions

Transforms Lua's `table` into a full OOP base class. `table( )` creates a new table instance with class behavior.

**Core mechanics:**

- `__index`: First searches the class hierarchy for a method named `key`. If not found and `key` is a string, tries `get_<key>` (getter convention).
- `__newindex`: If a `set_<key>` method exists, calls it. If only a `get_<key>` exists, raises an error (private key protection). Otherwise `rawset`.
- `set_class(class, ...)`: Assigns a class to a table instance. Sets up metatable, `__index`/`__newindex`, and wires up metamethods (`__add`, `__concat`, `__tostring`, `__call`, etc.).

**Key methods** (across `add.lua`, `each.lua`, `getters.lua`, `index.lua`, `pairs.lua`, `set_class.lua`, `tostring.lua`):

```lua
table( )                          -- new table instance
table{ 1, 2, 3 }                 -- new instance from literal
t:update( other )                 -- merge other into self
t:copy( other )                   -- copy other into self
t + item                          -- append item (or merge if table)
t .. other                        -- concat (string delimiter or table merge)
t:find( item )                    -- returns key of item, or nil
t:keys / t:values / t:first / t:last / t:length  -- getters
t.no_empty                        -- getter: removes "" entries, returns self
```

**Iteration methods** (dynamically generated in `each.lua`):

The `each` family is generated from closures with suffixes. Base names: `each`, `apply`, `map`, each with `_if` / `_if_not` variants. Suffixes determine how the callback receives arguments:

| Suffix       | Callback signature           |
|--------------|-------------------------------|
| (none)       | `action(value, ...)`          |
| `_list`      | `action(unpack(value))`       |
| `_pair`      | `action(key, value, ...)`     |
| `_list_pair` | `action(key, unpack(value))`  |
| `_self_pair` | `action(self, key, value)`    |
| `_self_list_pair` | `action(self, key, unpack(value))` |

Examples: `t:each_pair(fn)`, `t:each_if(is_string)`, `t:map(fn)`, `t:apply(fn)`.

- `each*` — collects results into a **new list**
- `apply*` — writes results back into **same table** (in-place)
- `map*` — collects results into a **new dict** keyed by original value

### `class` (`lua/sak/class/class.lua`) — OOP Class System

```lua
local	my_class	=	class( "my_class" )               -- create class inheriting from table
local	my_class	=	class( "my_class", some_parent )  -- inherit from some_parent

my_class.default_getters	=	{ "name", "id" }           -- auto-generate get_name/get_id

function my_class:my_class( name )               -- constructor (named after class)
	self.private.name	=	name
end

function my_class:greet( )                       -- method
	print( "hello " .. self.name )               -- self.name calls get_name
end

local	obj	=	my_class( "world" )                 -- instantiate (calls __call)
obj:greet( )
```

**Class hierarchy:** `class` -> `table` (table is the root class).

- `class( "name" )` creates a new class with `super = table`
- `class( "name", parent )` creates a class with `super = parent`
- `class.private.name` — class name (for `__tostring`)
- `class.private.super` — parent class
- `class:set_default_getters( keys )` — auto-generates `get_<key>` reading from `self.private`
- `class:set_default_setters( keys )` — auto-generates `set_<key>` writing to `self.private`
- `self.private` — instance-private storage (backing fields for getters/setters)

**Inheritance & method resolution:** `find_method` (in `index.lua`) walks the class chain via `class.private.super` until it finds a matching function.

### `string` (`lua/sak/string.lua`) — String Extensions

Extends Lua's string metatable:

```lua
s:starts_with( prefix ) / s:ends_with( suffix )
s:convert( )                          -- "true"->true, "false"->false, "123"->123, else string
s:enclose( before, after )            -- wrap in quotes (alias: s:quote)
s:split( delimiter )                  -- returns table of parts
s:lines( )                            -- split on \r\n, drop empties
s:words( )                            -- split on whitespace/periods, drop empties
s:csv( delim ) / s:tsv( delim )       -- parse multi-line CSV/TSV into table of rows
s:file_content( )                     -- io.open + read *all
s:process_open( )                     -- vim.fn.system (or io.popen fallback)
s:path_list( )                        -- find files under path (Linux: `find`, Windows: `pwsh`)
s:path_content( )                     -- list + read all files
s:unescape( )                         -- unescape \n \t \r \" \' \\
```
