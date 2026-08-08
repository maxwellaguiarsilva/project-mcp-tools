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

