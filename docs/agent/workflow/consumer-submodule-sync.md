# Consumer Submodule Synchronization Workflow

When `project-mcp-tools` is consumed as a git submodule by a host project, it acts as a version pin: the host project records a pointer (commit hash) and only adopts newer versions of the tool deliberately. 

The golden rule for developers modifying the tools while working inside a host project is always: **commit from the inside out** (commit inside the submodule first, then commit the pointer bump in the host project).

---

## Bootstrap (required once per fresh clone)

Git configuration is not versioned, so every new clone of the host project needs the following setup to ensure smooth submodule handling:

```bash
git config core.hooksPath .githooks
git config submodule.recurse true
git config push.recurseSubmodules on-demand
```

* `core.hooksPath`: enables versioned hooks (if the host project provides them, such as a pre-push guard).
* `submodule.recurse`: `pull`/`checkout` update the submodule working tree to the recorded pointer automatically.
* `push.recurseSubmodules=on-demand`: pushing the host project first pushes any pending submodule commits, ensuring a pointer to an unpublished commit can never reach the remote.

---

## Safety Nets (Provided by the Tools)

The `project-mcp-tools` framework provides built-in tools to safely manage submodule updates without manual git wrangling:

1. **`git_pull_submodules` (MCP/CLI tool):** updates every submodule to the latest remote commit of its tracked branch. It requires a clean submodule status and uses `switch` + `pull --ff-only`, so it never creates detached heads or silent merges. The pointer bump is left uncommitted in the host project for the owner to review.
2. **`git_quick_upload` (MCP/CLI tool):** calls the same submodule update logic before staging; any submodule failure aborts the upload before anything is committed. The successful flow is `pull` → update submodules → `add .` (captures the bump) → `commit` → `push`.

*(Note: Host projects are also encouraged to implement a `.githooks/pre-push` script to abort pushes when the recorded pointer is behind the submodule head).*

---

## Known Caveats

* After parent-level `pull`/`checkout`/`reset` operations, the submodule may end up in a detached head state. Run `git -C project-mcp-tools switch main` before committing inside it.
* If the submodule advances directly on the remote (e.g., edited on GitHub by another contributor), nothing breaks: the host project keeps the pinned version until `git_pull_submodules` (or a manual pull inside the submodule) plus a bump commit adopt the new version.