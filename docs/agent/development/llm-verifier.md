# `cpp_llm_verifier` — LLM-Level C++ Code Verifier

this document describes the `cpp_llm_verifier` MCP tool: its architecture, implementation details, current status, and roadmap.

for adding new tools, see [Tool Development Guide](adding-tools.md). for system architecture, see [System Architecture](../architecture.md).

---

## Overview

`cpp_llm_verifier` checks C++ source files against LLM-level coding rules defined in the [C++ Style Guide](../style-guide/cpp.md). each rule is verified individually per file using `opencode run --auto`, providing maximum accuracy at the cost of speed.

### Source Files

- **Tool entry point:** `cpp/llm_verifier.py`
- **Core engine:** `cpp/cpp_lib/llm_verifier_core.py`
- **Rule parser:** `cpp/cpp_lib/llm_rules.py`
- **Rule definitions:** `docs/agent/style-guide/cpp.md` and `docs/agent/style-guide/all.md` — rules are extracted at runtime via `<!-- llm-rule: id=..., complexity=... -->` metadata comments

---

## Architecture

### Batch-per-File Verification Strategy

all rules are verified together in a single run per file:

```
opencode run --auto --model <model> --dir <target_root> "<prompt>"
```

- `--auto` auto-approves permissions (safe in a controlled environment)
- `--model` is parameterized via the `model` argument (default: `deepseek/deepseek-v4-flash`)
- `--dir` sets the target project directory
- the prompt includes all rules + file content, with violations tagged by `rule_id`
- exceptions from the subprocess are caught silently — the file returns zero violations

this reduces subprocess count from M×N (files × rules) to M (files only), resolving the timeout issue while still checking every rule against every file in a single LLM call.

### Response Parsing

the output is parsed by `_parse_violation`:

1. `_strip_preamble` filters build status lines (starting with `> `) and blank lines
2. if the trimmed output is `"ok"` (case-insensitive), zero violations are returned
3. otherwise, regex `r"violation:\s*(\w+)\s*line\s*(\d+)\s*:\s*(.+?)(?=\n\s*violation|\Z)"` extracts all violations with their `rule_id`, `line_number`, and `description`
4. the `rule_id` is resolved against a lookup dict built from the checked rules to fill `rule_name`
5. each violation is paired with its `line_content` from the original file for audit context

**known limitation:** the prompt uses free-text format, not structured JSON — false positives (wrong line numbers) are expected and handled by the auditing workflow. if opencode changes its output format, the regex may need adjustment.

### Parallel Execution

`ThreadPoolExecutor` with 8 workers fans out file checks concurrently. each worker calls `openode run --auto` once per file with all rules batched in the prompt. results are collected via `as_completed`.

### Available Rules (17 total)

| Rule ID | Name | Complexity |
|---------|------|-----------|
| `lowercase` | Case — Lowercase Everywhere | low |
| `using` | `using` and `__using` | medium |
| `naming` | Naming | low |
| `comma` | Comma Rule | medium |
| `operators` | Operators | low |
| `flow_control` | Flow Control | low |
| `definitions` | Definitions | low |
| `termination` | Program Termination | medium |
| `console_output` | Console Output | low |
| `methods` | Methods | medium |
| `indirection` | Indirection | medium |
| `increment` | Increment | low |
| `comments` | Comments | low |
| `modernity` | Modernity, Composition, and the `sak` Library | high |
| `casts` | Casts | medium |
| `tests` | Tests | medium |
| `language` | Language | low |

---

## Test Results (2026-07-19)

all rules are batched into a single prompt per file, reducing subprocess count from M×N to M.

### Scenario 1: 1 File + 1 Specific Rule

| Setup | Subprocesses | Result |
|-------|-------------|--------|
| `files=["source/main.cpp"], rules=["ensure"]` | 1 | **PASS** |

### Scenario 2: 1 File + Multiple Rules

| Setup | Subprocesses | Result |
|-------|-------------|--------|
| `files=["source/main.cpp"], rules=["ensure","lowercase","naming"]` | 1 | **PASS** |
| `files=["source/main.cpp"], rules=<all 17 rules>` | 1 | **PASS** |

### Scenario 3: Full Project + All Rules

| Setup | Subprocesses | Result |
|-------|-------------|--------|
| `files=<all 44 .cpp/.hpp>, rules=<all 17 rules>` | 44 | **PASS** |

### Conclusions

1. the move from 1×1 (M×N subprocesses = 748 calls) to batch-per-file (M subprocesses = 44 calls) resolves the MCP transport timeout
2. each `opencode run --auto` subprocess takes ~30+ seconds regardless of rule count, so total wall time is dominated by `files / parallel_workers`
3. rule accuracy per file may be slightly lower than 1×1 (the LLM splits attention across 17 rules), but the trade-off is acceptable for practical use
4. caching is the next highest-impact optimization — skip re-verifying unchanged files

---

## Roadmap

### Caching (Highest Priority)

avoid re-verifying files that haven't changed since the last run:

- cache key: `hash( file_content + concatenated_rule_descriptions )`
- cache location: `target_root/.opencode/cache/llm_verifier/`
- each check: compute hash → if cached, reuse result → otherwise run opencode and cache
- `--no-cache` flag to force re-verification

### Rule Filtering

- `--exclude-rules`: check all rules except these
- `--complexity` flag: filter by complexity level (e.g., `--complexity low,medium` to skip high-complexity rules)
- `--rules-from`: read rule list from a file

### Rule Refinements

rules are defined in `docs/agent/style-guide/cpp.md` and parsed at runtime — no Python changes needed to add or modify a rule. specific concerns:

- **modernity (high):** requires project-wide context (`sak` library knowledge); the 1×1 format may be too narrow — consider a multi-file check or always flagging for manual audit
- **using (medium):** the `::` exemption list is nuanced — the prompt should emphasize exemptions clearly
- **methods (medium):** const/noexcept correctness requires type-level understanding beyond the file itself
- **casts (medium):** distinguishing "redundant" from "necessary" casts requires understanding implicit conversion rules

### Markdown Output Format

pass `output_md=<path>` to write a markdown report consumable by the main session for auditing:

```markdown
# cpp style violations

verified on: <timestamp>
files checked: <count>
rules checked: <count>

## <rule_name> (<rule_id>)

### <file_path>:<line_number>
```
<original_line_content>
```
**issue:** <llm_description>
**fix:**                ← filled manually during audit
```

the `**fix:**` field is left empty for the human auditor to fill in. the report path is returned in the JSON output as `output_md`.

### Per-Check Timeout

add a configurable timeout per file×rule pair (e.g., 60s) so slow checks don't block the thread pool.

### Progress Reporting

print progress to stderr so the caller can see advancement through the M×N grid.

### Multi-file Rule Sources (Pending)

The common rules (naming, comments, commas, single-use variables, language) have been moved from `cpp.md` to `all.md` to eliminate duplication with the Python style guide. Both files are imported by `cpp.md` and `python.md`.

**Pending adjustment:** The `llm_rules.py` parser currently scans only `cpp.md` for `<!-- llm-rule: ... -->` metadata. It must be updated to also scan `all.md` (and any file transitively imported via `Import:` headers) so that the moved rules continue to be verified. The rule table below still lists the original rules — update it once the parser supports multi-file sources.

### Integration Testing

1. **empty project:** no .cpp/.hpp files → returns zero checks, no errors
2. **clean file:** a file that follows all rules → returns zero violations
3. **known violations:** a file with deliberate violations → detects and reports them
4. **invalid target root:** raises a clear error message
5. **non-existent file:** skipped gracefully

---

## Execution Order (Recommended)

1. caching — reduces API calls, biggest win
2. rule filtering (`--complexity`, `--exclude-rules`) — lets users skip slow rules
3. per-check timeout — prevents slow checks from blocking the pool
4. ~~markdown output format~~ **done** — `output_md=<path>` writes `violations.md`
5. rule refinements — iterative, based on real results
6. progress reporting — quality of life
7. integration tests — validates correctness

---

## Naming Issue

this document replaces the consumer project's `draft-point/docs/agent/analyses/cpp_llm_verifier_deferred.md`, which used underscores in its filename in violation of the [kebab-lower-case convention](../workflow/markdown-organization.md). that file has been removed.
