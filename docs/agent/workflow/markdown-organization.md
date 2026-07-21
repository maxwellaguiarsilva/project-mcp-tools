# Markdown Reorganization Workflow

This workflow instructs AI agents on how to audit, reorganize, and maintain markdown documentation to avoid redundancy and "markdown hell", ensuring every document has a single source of truth and is properly connected via relative links.

---

## Mandatory Conventions

### Markdown Filenames and Directories: Strict Kebab-Lower-Case

All `*.md` filenames and directories inside `docs/agent/` **must** use **kebab-lower-case**: lowercase letters and hyphens only. Underscores are **prohibited** in markdown filenames and directory names under `docs/agent/`.

| Correct | Incorrect |
|---------|-----------|
| `adding-tools.md` | `adding_tools.md` |
| `markdown-organization.md` | `markdown_organization.md` |
| `python.md` | `Python.md` |

The only exceptions are root-level entry points explicitly required by tooling: `AGENTS.md` (root only) and `README.md`.

### Index Files: `index.md` Instead of `AGENTS.md`

Every directory in the `docs/agent/` tree **must** contain an `index.md` file that acts as the directory's index and table of contents.

- `AGENTS.md` is used **only** at the project root (`/AGENTS.md`) because tools (opencode, AI agents) specifically look for this file at the root.
- Every subdirectory **must** use `index.md` as its index file.
- Each `index.md` must:
  1.  Explain the scope and purpose of the directory.
  2.  Provide a clean index of relative links to all documents within that directory.
- High-level files (like the root `AGENTS.md`) must link to `index.md` (e.g., `[Workflows](docs/agent/workflow/index.md)`), not to `AGENTS.md`.

### Directory Scope and Purpose

Every directory inside `docs/agent/` must have a **well-defined scope and purpose** clearly stated in its `index.md`. A directory without a clear, bounded scope should not exist — merge its documents into a sibling directory or eliminate it.

### Markdown Import

Any markdown file may declare an `Import:` header to extend another markdown file within its own project:

```markdown
# Workflows

Import: [project-mcp-tools Workflows](../../../project-mcp-tools/docs/agent/workflow/index.md)

This directory extends the base workflows from `project-mcp-tools` with conventions specific to this project.
```

The `Import:` header tells AI agents to:

1. Read the imported document first to understand what is already defined there.
2. Treat the local document as an extension that adds content without repeating the imported topics.
3. Apply the "single source of truth" rule across file boundaries: a concept belongs in exactly one place.

Imports are only allowed within the same project. Since a submodule is part of the consumer project's working tree, a consumer may import from its submodule — but the submodule itself must never import from external projects.

---

## Objectives

1.  **Eliminate Redundancy:** Ensure each concept, rule, or piece of information exists in exactly one authoritative document.
2.  **Maintain Context:** Ensure every document has a clear scope, folder location, and header explaining its purpose.
3.  **Prevent Loose Documents:** Ensure every markdown file is referenced/linked by at least one other document.
4.  **Avoid High-Level Clutter:** Use local `index.md` files in scoped directories to act as indexes, preventing root-level files from becoming cluttered with long lists of links.
5.  **Respect Documentation Ownership:** Keep the AI-managed knowledge base inside `docs/agent/` and never create or modify documents outside it (see [Documentation Ownership](#documentation-ownership)).

---

## Documentation Ownership

The `docs/` tree is split into two ownership zones:

- **`docs/agent/` — AI/LLM zone.** The entire knowledge base used and administered by LLMs (architecture, development guides, style guides, workflows, status/planning documents) must always live inside `docs/agent/`. This is the only documentation area where AI agents are allowed to create, edit, move, or delete markdown files.
- **`docs/` root and any folder outside `docs/agent/` — programmer/user zone.** These belong to the human developer/user of the project (e.g., `docs/templates/`, `docs/example/`). AI agents must **not** create or alter documents there. If a change seems necessary in that zone, the agent must report it to the user instead of editing it.

Exception: root-level entry points (`README.md` and the root `AGENTS.md`) may be updated by agents when links into `docs/agent/` need to be kept in sync.

---

## Step-by-Step Workflow

### Step 1: Audit and Map
- Search the entire repository for all markdown (`*.md`) files.
- Read each file to understand its purpose and content.
- Identify and map out all redundancies (e.g., duplicate coding rules, identical tables, repeated architecture diagrams, or overlapping setup instructions).
- Flag any markdown filenames that violate the **kebab-lower-case** rule.
- Flag any directory inside `docs/agent/` that lacks an `index.md` or whose `index.md` does not cover all documents in that directory.

### Step 2: Define Single Sources of Truth
- For each redundant concept, decide on a single authoritative document where it belongs.
- The choice of the authoritative document must align with:
  - **File Name:** The name of the file must use **kebab-lower-case** and clearly represent the concept (e.g., `python.md` for Python style rules).
  - **Folder Scope:** The folder should match the domain (e.g., `docs/agent/style-guide/` for style guides, `docs/agent/workflow/` for workflows).
  - **Header/Purpose:** The document's header must explain its scope.

### Step 3: Consolidate and Link
- Move the redundant content to its designated authoritative document.
- In all other files where the content was duplicated, replace the duplicated text with:
  1.  A very concise summary of the concept.
  2.  A relative hyperlink pointing directly to the authoritative document (e.g., `[Python Style Guide](../style-guide/python.md)`).

### Step 4: Establish Local Contexts
- For every directory inside `docs/agent/`, ensure an `index.md` exists that:
  1.  Explains the scope and purpose of the directory.
  2.  Provides a clean index of links to all documents within that directory.
- Update high-level files (like the root `AGENTS.md`) to point only to the directory's `index.md` (e.g., `[Workflows](docs/agent/workflow/index.md)`), keeping the root-level list short and clean.

### Step 5: Verify Connectivity
- Perform a final check to ensure there are no "loose" markdown files.
- Every single markdown file in the repository must be reachable by following links starting from the root `README.md` or `AGENTS.md`.
- Verify that all relative links are correct and resolve properly.
- Verify that all markdown filenames in `docs/agent/` comply with the **kebab-lower-case** rule.
