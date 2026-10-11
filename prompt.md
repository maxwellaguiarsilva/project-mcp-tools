# Misleading `[autofix-done]` label in verifier output

## Problem

The verifier engine labels every non-warning violation as `[autofix-done]`, regardless of whether the auto-fix flag is enabled or whether the file was actually written. In verify-only mode, the label claims a fix that was never applied to disk.

## Observed behavior

- With `flg_auto_fix=False` (the constructor default), `run()` returns violations labeled `[autofix-done]`, and nothing is written to disk. Reproduced with a stub model whose `get_files` returns one item with body `f(1)`: the output contains `line N: [autofix-done] missing space after '('`, and the item's `write` is never called.
- With `flg_auto_fix=True`, the label is printed for each violation, and the file is written when the final content differs from the original. This is the only mode used by the entry points found in the repository.
- Rules whose replacement is `\g<0>` are labeled `[warning-only]`. That label is correct, because it is decided by the rule definition and not by an outcome.

## Root cause

In `lib/base_verifier.py`, the inner function `sub_func` inside `base_verifier.run` chooses the label with:

`kind = "warning-only" if r.replacement == r"\g<0>" else "autofix-done"`

The label depends only on the rule's replacement. It never checks `self.flg_auto_fix`. The file write happens later, once per file, in the block guarded by `if self.flg_auto_fix:` after the `new_content` comparison. The label is therefore decided before the outcome it names, and without knowing whether that outcome happens.

## What needs to be corrected

- The label must describe what actually happens. In verify-only mode, a violation must not be reported as fixed.
- A violation reported as fixed must correspond to content that was written to disk.
- Keep the distinction between warning-only rules and auto-fixable rules. It is a property of the rule and is correct.
- Define the wording for the verify-only case and implement it in the verifier engine, so every consumer receives the same wording.
- The file write is decided once per file after all rules have run, while violations are recorded per match. The implementation must account for this ordering.
- Preserve the current output structure (the `file:` header, the indented `line N:` entries, and the "no formatting violations found" message) unless the change explicitly requires otherwise.

## Files involved

- `lib/base_verifier.py`: `base_verifier.__init__` (stores `flg_auto_fix`, default `False`) and `base_verifier.run` (`sub_func`, the `kind` assignment, the `violations` list, and the write block).
- `python/python_lib/python_verifier.py` and `cpp/cpp_lib/cpp_verifier.py`: subclasses of `base_verifier`. They define rules but do not produce the labels; they inherit the behavior.
- `python/code_verifier.py` (line 23), `cpp/code_verifier.py` (line 26), `python/analyze.py` (line 26), and `cpp/analyze.py` (line 29): construct the verifiers with `flg_auto_fix = True`. These are the only callers found in the repository.

## Verification

- The repository has no automated test suite.
- Validate by running a stub-model check in verify-only mode, confirming the output label and that no file is written.
- Validate formatting of the changed Python files with `python_code_verifier`. Note that the MCP verifier tools run in fix mode and may rewrite the files they check.

## Constraints

- Follow the repository `AGENTS.md`.
- File content must be written in English (US).
- Identifiers use snake_case.
- Markdown must not be artificially line-wrapped.
