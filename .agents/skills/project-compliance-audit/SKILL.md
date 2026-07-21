---
name: project-compliance-audit
description: Audit a project for architectural compliance — identify unused existing helpers, redundancy, improper coupling, inconsistency with documented conventions, and obsolete documentation. Use when asked to "audit", "review compliance", "find inconsistencies", "check code quality", or ensure a project is architecturally "in unison".
---

# Project Compliance Audit

Systematic methodology to verify that a codebase is architecturally consistent and fully leverages its own abstractions.

## When to Use This Skill

- User asks for an architecture audit, compliance review, or code quality assessment
- User suspects redundancy or inconsistency in the codebase
- After major refactoring — verify no patterns were broken
- Before accepting contributions — ensure PRs follow project conventions
- Periodically, as a hygiene check

## Audit Methodology

### Phase 1: Absorb Documentation

Read every piece of project documentation first:

- `README.md` — architecture, conventions, tool catalog
- `CONTRIBUTING.md` or `AGENTS.md` — coding standards, workflows
- `docs/` directory — planning docs, status docs, design decisions
- Any `.editorconfig`, `pyproject.toml`, linter configs

Extract every **explicit convention** into a checklist. Examples:
- Naming rules (snake_case, PascalCase, etc.)
- Import rules (absolute vs relative, ordering)
- Library/framework preferences (which ORM, which test runner, etc.)
- File organization rules (where domain logic lives vs. facades)
- Error handling patterns
- Serialization patterns

### Phase 2: Inventory the Toolbox

Identify all shared/utility/helper modules. For each, list every public function, class, and constant it exports. This becomes your "available toolbox" — any raw equivalent found elsewhere is a potential violation.

Example sources of helpers:
- `sak/common.py` or `utils/` — general utilities
- `lib/base_*.py` or abstract base classes
- `lib/fso/` or I/O wrappers
- Project-specific config modules
- Template engines or scaffolding utilities
- Domain library packages (`*_lib/`)

### Phase 3: Full Codebase Scan

Read **every source file** in the project. For each file, check:

#### 3a. Non-Utilization of Existing Helpers

For every raw/low-level operation, ask: does the project already provide a wrapper or helper for this?

| Raw usage | Should it use a project helper? |
|---|---|
| `json.dumps(x, indent=4, default=str)` | Does a `to_json()` wrapper exist? |
| `subprocess.run(cmd, ...)` | Does a `create_process()` or `run_cmd()` exist? |
| `assert x, "msg"` | Does an `ensure()` or `check()` exist? |
| Manual string formatting for output | Does a `print_line()` or formatter exist? |
| Manual type checking + list normalization | Does an `ensure_list()` exist? |
| `os.cpu_count()` | Does a platform-safe `get_cpu_count()` exist? |
| Manual dict merge logic | Does a `deep_update()` exist? |

#### 3b. Redundancy

- Are module-level constants redefined in subclasses when the base class already defines them?
- Are the same patterns duplicated across files instead of extracted into shared libs?
- Are domain model classes defined in tool/facade files instead of `*_lib/` directories?
- Do two different files perform the same config merge (`deep_update(copy.deepcopy(base), override)`) — could this be a single helper?

#### 3c. Improper Coupling

- Does a tool/facade file import another tool/facade file directly? (tools should depend on domain libs, not each other)
- Does the constructor eagerly initialize resources that are only needed by one code path? (e.g., server instance created even when CLI mode is used)
- Are transport/protocol concerns leaking into domain logic?

#### 3d. Inconsistency with Project Conventions

Compare every file against the conventions extracted in Phase 1:

- Import style: does every file follow the same convention (absolute vs relative)?
- Are imports used? (no unused imports)
- Do all files in the same layer follow the same structural pattern?
- Is error handling consistent? (same exception types, same wrapping style)
- Is serialization consistent? (same JSON wrapper, same indent style)
- Are class hierarchies respected? (verifiers extend base verifier, models extend project file, etc.)

#### 3e. Documentation Incongruence

- Do docs reference files, classes, or functions that no longer exist?
- Are architecture diagrams up to date?
- Do status docs describe completed work as if it were still in progress?
- Are there documents with duplicate or contradictory information?
- Do naming conventions in docs match the actual code?

### Phase 4: Report Findings

Structure the report in exactly these 5 categories:

1. **Non-utilization of existing helpers** — raw operations that should use project wrappers
2. **Redundancy in architecture** — duplicated constants, classes in wrong layers, repeated patterns
3. **Improper coupling** — layering violations, eager initialization, tool-to-tool imports
4. **Inconsistency with best practices** — import style mismatches, error handling divergence, structural asymmetries
5. **Incongruent or obsolete documentation** — stale references, outdated diagrams, completed work presented as current

For each finding, provide: file path, line number, what was found, and what the expected behavior should be.

### Phase 5: Document Implicit Conventions

After the audit, any strong pattern observed across the codebase that is NOT documented should be proposed for documentation. These are "implicit conventions" — rules the project follows in practice but hasn't written down. Common examples:

- "All domain configs merge into project_config via deep_update"
- "Verifier classes must extend base_verifier, never implement from scratch"
- "Error messages use sentence case, no trailing punctuation"
- "Always use to_json, never json.dumps directly"

### Phase 6: Update Project Documentation

When the audit generates new knowledge about the project's conventions:
- Update `docs/agent/general-planning.md` (or equivalent) with newly documented conventions
- Update `docs/agent/status.md` (or equivalent) to reflect current state
- Remove or archive obsolete planning documents
- Fix numbering errors, stale references, outdated diagrams

## Output Format

```
## 1. Non-utilization of existing helpers
| File | Line | Issue | Expected |
|------|------|-------|----------|

## 2. Redundancy in architecture
| File | Line | Issue | Expected |
|------|------|-------|----------|

## 3. Improper coupling
| File | Line | Issue | Expected |
|------|------|-------|----------|

## 4. Inconsistency with best practices
| File | Line | Issue | Expected |
|------|------|-------|----------|

## 5. Incongruent or obsolete documentation
| File | Line | Issue | Expected |
|------|------|-------|----------|
```

## Key Principles

- **The project's own helpers are always preferred** over raw language/stdlib equivalents. If a wrapper exists, it exists for a reason (error context, platform safety, consistent formatting).
- **Conventions are discovered, not invented.** If the codebase consistently does something one way, that is the convention — even if undocumented.
- **Documentation is code.** Stale docs are bugs. They mislead future contributors and AI assistants.
- **Thin facades, fat libs.** Tool/endpoint/controller files should be wiring only. Domain logic belongs in dedicated library directories.
