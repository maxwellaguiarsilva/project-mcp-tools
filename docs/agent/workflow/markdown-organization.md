# Markdown Reorganization
This workflow instructs AI agents on how to audit, reorganize, and maintain markdown documentation to avoid redundancy and "markdown hell", ensuring every document has a single source of truth and is properly connected via relative links.

---
### Markdown Filenames and Directories: Strict kebab-lower-case
All `*.md` filenames and directories inside `docs/agent/` **must** use **kebab-lower-case**: lowercase letters and hyphens only. Underscores are **prohibited** in markdown filenames and directory names under `docs/agent/`. The only exceptions are root-level entry points explicitly required by tooling: `AGENTS.md` and `README.md`.

### Index Files: `index.md`
Every directory in the `docs/agent/` tree **must** contain an `index.md` file that acts as the directory's index and table of contents.
- Every subdirectory **must** use `index.md` as its index file.
- Each `index.md` must:
  1.  Explain the scope and purpose of the directory.
  2.  Provide a clean index of relative links to all documents within that directory.

---

## Objectives
1.  **Eliminate Redundancy:** Ensure each concept, rule, or piece of information exists in exactly one authoritative document.
2.  **Maintain Context:** Ensure every document has a clear scope, folder location, and header explaining its purpose.
3.  **Prevent Loose Documents:** Ensure every markdown file is referenced/linked by at least one other document.
4.  **Avoid High-Level Clutter:** Use local `index.md` files in scoped directories to act as indexes, preventing root-level files from becoming cluttered with long lists of links.

---

## Documentation Ownership
The `docs/` tree is split into two ownership zones:
- **`docs/agent/` — AI/LLM zone.** The entire knowledge base used and administered by LLMs (architecture, development guides, style guides, workflows, status/planning documents) must always live inside `docs/agent/`. This is the only documentation area where AI agents are allowed to create, edit, move, or delete markdown files.
- **`docs/` root and any folder outside `docs/agent/` — programmer/user zone.** These belong to the human developer/user of the project (e.g., `docs/templates/`, `docs/example/`). AI agents must **not** create or alter documents there. If a change seems necessary in that zone, the agent must report it to the user instead of editing it.
Exception: root-level entry points (`README.md` and the root `AGENTS.md`) may be updated by agents when links into `docs/agent/` need to be kept in sync.

