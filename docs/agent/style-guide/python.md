# Python Style Guide

Import: [Common Style Rules](all.md)

This document extends the common style rules with Python-specific conventions and tooling.

For system architecture, see [System Architecture](../architecture.md). For instructions on adding new tools, see the [Tool Development Guide](../development/adding-tools.md).

## Verification Matrix

Rules are enforced at different levels depending on whether they can be
mechanically checked or require semantic understanding:

| Level | Tool | Scope |
|---|---|---|
| **Regex** | `python_code_verifier` | Spacing, quotes, comment formatting, `Optional` to `\| None`, helper usage, and other purely mechanical patterns. See each section's badge below. |
| **LLM** | Code generation / review | Naming quality, readability, architectural choices, design patterns, and intent — rules that require understanding the code's purpose. |

Individual sections below may be tagged with `[python_code_verifier]` (regex)
or `[llm]` (requires judgment) to indicate who owns the check.

---

## Naming (Python specifics)

The common naming rules apply.

Additionally, Python-specific naming patterns follow the same `snake_case` convention:

```python
#   correct
class project_model:
    def get_files( self, file_list ):
        max_items = 10
        return  result

#   incorrect
class ProjectModel:
    def getFiles( self, fileList ):
        MAX_ITEMS = 10
        return  result
```

---

## Strings `[python_code_verifier]`

### Use double quotes for all string literals

Always use `"double quotes"` instead of `'single quotes'`.

```python
#   correct
name = "John"
message = f"hello {name}"
pattern = r"\d+\.\d+"

#   incorrect
name = 'John'
message = f'hello {name}'
```

### Exception: strings containing double quotes

When a string contains `"` characters, use triple-double quotes (`"""`) to avoid escaping:

```python
# instead of: f"version = {config[\"key\"]}"
f"""version = {config["key"]}"""

# instead of: r"\"hello\""
r""" "hello" """
```

This applies to all string prefixes: `f`, `r`, `fr`/`rf`, and plain strings.

---

## Comments, Docstrings, and User-facing Messages

The common comment rules apply. Additionally:

### Highlighting within strings

Use `"double quotes"` (not single quotes) to highlight terms inside a string:

```python
#   correct
"""the class_hierarchy parameter defines the namespace and class name
(e.g., "game/player" creates class "player" in namespace "game")"""

#   incorrect
'game/player' creates class 'player'
```

### Docstrings and inline comments

```python
@tool( )
def my_tool( ) -> str:
    """description of what this tool does
    each line is a separate thought without connecting punctuation
    this keeps strings visually consistent with comments"""
```

```python
ensure( False, "unknown config key" )  # not: "Unknown config key."
```

---

## Spacing `[python_code_verifier]`

### Spaces inside parentheses and brackets

Always put a space after `(` and before `)` in function calls and definitions. Same for `[ ]` and `{ }`.

```python
#   correct
ensure( expression, message )
super( ).__init__( file_path )
config[ "compiler" ][ "standard" ]
[ 1, 2, 3 ]
{ "key": value }

#   incorrect
ensure(expression, message)
super().__init__(file_path)
config["compiler"]["standard"]
[1, 2, 3]
{"key": value}
```

Exceptions: slicing, decorators, empty containers.

```python
data[ 8:12 ]     # slicing: keep spaces
@tool( )         # decorator: keep space
[ ]              # empty list
{ }              # empty dict
```

### Exit keyword padding (`return`, `raise`, `yield`)

Standard indentation is 4 spaces. Keywords that are exit points of a function/method are padded with spaces so the following expression starts at column 8 (keyword length + padding = 8 characters):

- `return` — 6 letters + 2 spaces
- `raise` — 5 letters + 3 spaces
- `yield` — 5 letters + 3 spaces

```python
#   correct
return  value
return  f"result: {name}"
return  to_json( self.to_dict )
raise   Exception( "message" )
yield   item
return              # bare return: no extra space
raise               # bare re-raise: no extra space

#   incorrect
return value
return f"result: {name}"
raise Exception( "message" )
yield item
```

### Dict literal spacing

One space before `:` and one space after:

```python
#   correct
{ "key": value, "other": 42 }

#   incorrect
{"key":value, "other":42}
{'key': value, 'other': 42}
```

---

## Commas (Python specifics)

The common comma rule applies. Python-specific details:

Even with 4 or more items, a structure may stay inline when short:

```python
cmd = [ "git", "reset", "--hard", "HEAD" ]
```

### Visual alignment

The first item in a multiline list gets an extra space so keys/values align vertically with items that have a leading comma:

```python
data = {
     "name": "john"       # 5 spaces (4 + 1 extra)
    ,"age": 31            # 4 spaces + 1 comma
    ,"flg_developer": True
}

return  tool_info(
     name        = name
    ,description = description
    ,function    = func
    ,parameters  = params
)
```

### Nested structures

Always split nested structures across lines to avoid excessive line length:

```python
python_project_config = {
    "language": {
         "extension": "py"
        ,"comment_string": "#   "
    }
}
```

---

## Comprehensions and Ternaries `[llm]`

<!-- llm-rule: id=comprehensions, complexity=medium -->

Prefer expressive single-expression constructs over imperative blocks.

### Comprehensions over manual loops

Use list/dict/set comprehensions instead of `for`-loops that build collections:

```python
#   correct
files = [ c.object.path for c in self.dependencies_list ]
futures = [ executor.submit( core.build, c ) for c in all_cpps.values( ) ]
self.includes = [ match.group( "path" ) for match in regex.finditer( content ) ] if content else [ ]
deps = { dep for dep in visited if isinstance( dep, hpp ) and dep is not self }

#   incorrect
files = [ ]
for c in self.dependencies_list:
    files.append( c.object.path )
```

### Ternaries over if/else blocks

Use `value if condition else other` instead of multi-line `if`/`else` blocks for simple value selection:

```python
#   correct
return  value if isinstance( value, list ) else [ value ]

#   incorrect
if isinstance( value, list ):
    return  value
else:
    return  [ value ]
```

Ternaries can be nested or combined with comprehensions:

```python
#   ternary inside comprehension
ext_list = [ ext if ext.startswith( "." ) else f".{ext}" for ext in ensure_list( extensions, str ) ] if extensions else None

#   ternary expression inside generator for any()/all()
all( isinstance( item, cls ) for item in items )
```

### extend() with generator/list

Use `extend( ... for ... in ... )` instead of a loop with `append`:

```python
#   correct
params.extend( config[ "compiler" ].get( "extra_compile_flags", [ ] ) )

#   incorrect
for flag in config[ "compiler" ].get( "extra_compile_flags", [ ] ):
    params.append( flag )
```

### any() / all() with generator

Use `any( expr for x in items )` or `all( expr for x in items )` instead of manual loops with early returns:

```python
#   correct
if not any( file_path.endswith( ext ) for ext in ext_list ):
    continue

#   incorrect
found = False
for ext in ext_list:
    if file_path.endswith( ext ):
        found = True
        break
if not found:
    continue
```

---

## Properties Over get_/set_ Methods `[llm]`

<!-- llm-rule: id=properties, complexity=medium -->

Prefer `@property` getters and setters instead of `get_*` / `set_*` methods whenever an attribute is accessed or mutated directly. This keeps the public interface consistent — callers use attribute syntax both for reading and writing — and makes future refactoring (e.g., adding validation, caching, or lazy loading) transparent to consumers.

```python
#   correct
@classmethod
@property
def instance( cls ) -> "singleton":
    ...

@classmethod
@instance.setter
def instance( cls, value: "singleton" ):
    ...

#   incorrect
@classmethod
def get_instance( cls ) -> "singleton":
    ...

@classmethod
def set_instance( cls, value: "singleton" ):
    ...
```

```python
#   correct
@property
def target_root( self ) -> Path:
    return  self._target_root

@target_root.setter
def target_root( self, path: str | Path ):
    self._target_root = Path( path ).resolve( )

#   incorrect
@property
def target_root( self ) -> Path:
    return  self._target_root

def set_target_root( self, path: str | Path ):
    self._target_root = Path( path ).resolve( )
```

Exceptions: methods that perform side effects beyond simple attribute mutation (e.g., `ensure_path`, `connect`, `commit`) should remain plain methods.

---

## Dict Construction with `|` (Pipe) Operator `[llm]`

<!-- llm-rule: id=dict_pipe, complexity=low -->

Prefer the `|` operator (Python 3.9+) for combining dictionaries inline instead of multi-statement patterns with `.copy()` and `.update()`.

```python
#   correct
env = os.environ | self._path_manager.to_env( ) | { "TIMEOUT": "30" }

#   incorrect
env = os.environ.copy( )
env.update( self._path_manager.to_env( ) )
env[ "TIMEOUT" ] = "30"
```

This also works directly inside function call arguments, avoiding a temporary variable:

```python
#   correct
create_process( cmd, env = os.environ | extra_env | { "KEY": "value" } )

#   incorrect
env = os.environ.copy( )
env.update( extra_env )
env[ "KEY" ] = "value"
create_process( cmd, env = env )
```

---

## Imports `[llm]`

<!-- llm-rule: id=imports, complexity=medium -->

### Location — top of file only

All `import` and `from ... import` statements must appear at the top of the file, before any function, class, or other definition. Imports inside functions, methods, or classes are prohibited.

```python
#   correct
import os
from sak.common import ensure

def process( ):
    ...

#   incorrect
def process( ):
    import os
    ...
```

### Ordering

1. Standard library
2. (blank line)
3. Third-party libraries
4. (blank line)
5. Project modules

### Style

Absolute imports for cross-domain references. Relative imports only within the same `*_lib/` package.

```python
import os
import re

from fastapi import FastAPI

from sak.common import ensure, create_process
from tools.tool import tool
from cpp.cpp_lib.cpp_verifier import cpp_verifier
```

---

## Type Hints `[llm]`

<!-- llm-rule: id=type_hints, complexity=low -->

Use `type | None` instead of `Optional[type]`. Keep type hints minimal but present for public interfaces.

```python
#   correct
def find( name: str ) -> str | None:

#   incorrect
def find( name: str ) -> Optional[ str ]:
```

---

## Using `sak/common.py` Helpers `[llm]`

<!-- llm-rule: id=helpers, complexity=low -->

Always prefer project helpers over raw Python equivalents:

| Instead of | Use |
|---|---|
| `assert expr, msg` | `ensure( expr, msg )` |
| `subprocess.run( cmd, ... )` | `create_process( cmd, ... )` |
| `json.dumps( data, ... )` | `to_json( data )` |
| `obj.__dict__` or manual dict building | `to_dict( obj, members )` |
| `os.cpu_count( )` | `get_cpu_count( )` |
| `print( "=" * 50 )` | `print_line( strong = True )` |
| `print( "-" * 50 )` | `print_line( strong = False )` |
| Manual dict merge with copy | `deep_update( copy.deepcopy( base ), override )` |
| `value if isinstance( value, list ) else [ value ]` | `ensure_list( value, cls )` |
| `git config --global <key>` subprocess | `get_git_config( key )` |
| `text.removeprefix( prefix )` | `remove_string_prefix( text, prefix )` |

---

## Regex Verifier Philosophy (Pareto Principle) `[python_code_verifier]`

The regex-based code verifiers (Python and C++) must adhere to a strict Pareto Principle (80/20 rule) to avoid "regex hell" and maintain developer trust:

1. **Zero False Positives (Critical):** Flagging a false violation is a severe issue. It disrupts developers, breaks valid code, and forces ugly workarounds. Rules must be designed to *never* produce false positives. If a scenario is ambiguous or too complex for a simple regex, the rule must ignore it.
2. **Acceptable False Negatives (Low Impact):** Failing to detect a legitimate violation is acceptable if the context is complex (e.g., the pattern is inside a string literal, a comment, or a complex syntactic structure). It is far better to miss a violation than to flag a false positive.
3. **Rule Design Strategy:**
   - **Line Filters (`line_filter`):** The `rule` class supports an optional `line_filter` callable. When a regex match is found, the verifier extracts the line containing the match and runs the filter. If the filter returns `False`, the match is ignored. This keeps rules declarative while allowing precise, context-aware filtering.
   - **Indent Context (Python):** Python code structures (like `return`, `raise`, `yield`) must start with an indentation that is a multiple of 4 spaces. Checking `leading_spaces % 4 == 0` on the line filters out plain text occurrences (e.g., "please, raise your hand") while correctly formatting code examples inside docstrings/comments.
   - **Single Quote Filtering:** To safely detect single quote violations without false positives, the rule uses a line filter that skips lines containing both single and double quotes (mixed quotes), lines containing comments (`#`), or docstring markers (`"""`, `'''`).
   - **String Externalization:** To minimize string-related regex exceptions, projects should externalize user-facing messages and strings into translation files (e.g., `resource/language/en-us.json`). This keeps the source code clean of complex, hardcoded text literals, leaving only simple translation keys (e.g., `"error_user_not_found"`) that are easily validated by double-quote rules without false positives.
   - Avoid overly complex regexes that try to handle 100% of edge cases; aim for the 80% of clear, unambiguous violations with 20% of the complexity.

---


