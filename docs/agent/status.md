# Status

## Current Task

### Implement missing regex rules in `cpp_code_verifier`

**Goal:** Add regex-based checks for rules confirmed to have no false positives
in the target project (see `docs/agent/style-guide/cpp.md` — Verification Matrix):

- `using namespace` — forbidden
- `this->` — forbidden
- `::std::exit` / `::std::abort` / `::std::quick_exit` — forbidden
- `#pragma once` without matching `#ifndef header_guard_` — missing guard

**Status:** Completed.

