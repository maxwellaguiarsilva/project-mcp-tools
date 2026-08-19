# Common Style Rules

This document contains coding style rules that apply to all languages in this
project. Language-specific guides import this file and extend it with their own
tooling and syntax conventions.

---

## LLM Rule Registration

Every `[llm]` section must include a metadata comment immediately after the
heading with the format:

```markdown
<!-- llm-rule: id=<snake_case_id>, complexity=<low|medium|high> -->
```

This makes the rules machine-parseable so LLM verifier tools can extract them
automatically without hardcoded strings. Adding a new LLM rule requires only
editing this markdown file — no code changes.

---

## Naming `[llm]`

<!-- llm-rule: id=naming, complexity=low -->

Use `snake_case` for all identifiers: variables, functions, methods, classes,
constants, and filenames. No PascalCase, no camelCase, no UPPER_CASE.

Exceptions:
- External libraries and APIs that require different conventions.
- Scenarios where the language or tooling requires a different case.

```text
#   correct
my_variable
user_manager
get_file_path( )

#   incorrect
myVariable
UserManager
getFilePath( )
```

---

## Comments `[llm]`

<!-- llm-rule: id=comments, complexity=low -->

All comments follow the same visual pattern: lowercase, no trailing period,
one idea per line.

```text
#   correct: lowercase, no period
#   another thought goes on its own line

#   incorrect: capitalized and with a period at the end
```

---

## Comma Rule (Rule of 3) `[llm]`

<!-- llm-rule: id=commas, complexity=medium -->

The rule of 3 is a starting point for common sense, not a hard limit. The real
goal is to avoid exceptionally long lines — more than **100 characters** — that
force soft wrapping to see the whole code.

### Inline

On the same line, the space comes after the comma.

When a list, enum, argument list, or aggregate initializer has **3 or fewer**
items, keep it on one line. **4 or more** items may also stay inline when the
line remains short (under 100 characters) and readable.

### Multiline (leading commas)

Any list that is split across multiple lines must use the **leading comma**
style — the comma starts the line.

Split across lines in two cases:
1. When there are **4 or more** items and inlining would exceed **100
   characters** or hurt readability.
2. When there are **3 or fewer** items, but inlining would exceed **100
   characters** or make it hard to read (e.g., nested structures).

```text
#   leading comma style
data = {
     "name": "john"
    ,"age": 31
    ,"flg_developer": true
}
```

---

## Single-use Variables `[llm]`

<!-- llm-rule: id=single_use_vars, complexity=medium -->

A single-use variable is justified when it **captures a value at a specific
point in time** — preserving the semantic meaning of what passed through that
line. It is **not** justified when it merely renames something at the call
site.

```text
#   justified: freezes the value at this moment in the pipeline
filtered = data | filter( is_valid ) | to_list( )
clean = normalize( filtered )

#   unjustified: just a new name for the same thing, used once
serialized = to_json( data )
save( serialized )
#   better
save( to_json( data ) )
```

JSON serialization is the canonical example. `to_json( data )` is already
expressive — wrapping it in a variable adds no timeline information and wastes
a line.

---

## Language `[llm]`

<!-- llm-rule: id=language, complexity=low -->

All code, comments, and user-facing messages must be in English (US).
