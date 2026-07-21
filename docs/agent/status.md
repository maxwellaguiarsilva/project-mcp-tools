# Status

## Current Task

### Implement `loop` mode — programmer/police orchestration engine

**Goal:** Add a fourth mode to `project-mcp-tools` that runs opencode in a loop:
a programmer agent implements the specification, then an independent police agent
(fresh session, no programmer context) evaluates the result via Playwright and
compliance rules. If not satisfied, feedback is fed back and the loop continues.

**Key concerns:**
- Hangout protection (subprocess timeouts, kill-on-timeout, max-iterations)
- Two-context isolation (programmer session continues; police session is fresh)
- Verdict parsing (structured JSON from police output)
- Session monitoring via `opencode.db` for diagnostics

**Design doc:** `docs/agent/loop-engine-design.md` (in host project)

**Status:** Completed. All files compile, imports work, dry-run works, process guard timeout/kill tested, verdict parser tested, session monitor tested against real opencode.db, python_code_verifier passes on all new files.

---

## Previous Task

### Implement missing regex rules in `cpp_code_verifier`

**Goal:** Add regex-based checks for rules confirmed to have no false positives
in the target project (see `docs/agent/style-guide/cpp.md` — Verification Matrix):

- `using namespace` — forbidden
- `this->` — forbidden
- `::std::exit` / `::std::abort` / `::std::quick_exit` — forbidden
- `#pragma once` without matching `#ifndef header_guard_` — missing guard

**Status:** Completed.

