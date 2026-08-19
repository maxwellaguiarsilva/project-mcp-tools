# Status

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

---

## Current Task

### Add `describe_image` tool in `general/`

**Goal:** Create a tool that uses the Gemini vision model
(`gemini-flash-lite-latest`) to help models without vision capability interpret
an image located in the target project. The calling model passes the relative
image path and a description of what it wants to know about the image.

**Status:** Completed.

- `general/describe_image.py` — thin `@tool()` facade
- `general/general_lib/image_interpreter.py` — vision logic (path resolution,
  MIME detection, Gemini call)
- Verified end-to-end via CLI and against the Google API

---

## Previous Task

### Add `session_context_usage` tool in `session/`

**Goal:** Create a mechanism for AI assistants to know how much of the model
context token window is in use in the current opencode chat session, so the user
can write conditional instructions (e.g., "if the session reaches 30k tokens,
compact it"). The tool reads the opencode SQLite database directly (no HTTP
server needed).

**Status:** Completed.

- `session/context_usage.py` — thin `@tool()` facade
- `session/session_lib/context_tracker.py` — database reader: auto-detects the
  active session in the target project, extracts the last request's token usage,
  resolves the model context limit from the models.dev cache
- Registered `session` folder in `main.py`
- Verified via CLI (`session_context_usage` returns `context_used`,
  `context_percent`, and breakdown) and `python_code_verifier`
- Documented the mechanism in `docs/agent/knowledge/opencode/index.md`


