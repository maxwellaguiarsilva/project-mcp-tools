# System Architecture

This document outlines the system architecture, design decisions, target project mechanism, and global configuration of the `project-mcp-tools` framework.

For style guidelines, see the [Python Style Guide](style-guide/python.md). For instructions on adding new tools, see the [Tool Development Guide](development/adding-tools.md).

---

## Architecture Overview

The system is built around a central `tool_manager` object (in `tools/tool_manager.py`) that holds the shared tool registry and handles all three transports:

```
                          main.py
                             |
                     tools/tool_manager
                     /     |      \
               run_cli()  run_api()  run_mcp()
              (argparse) (FastAPI)  (FastMCP)
                     \     |      /
               run_in_subprocess()
                      |
                (subprocess)
                      |
              actual tool function
```

All three transports (CLI, API, MCP) share the same tool registry, guaranteeing identical behavior across protocols.

### Loop Mode (Fourth Mode)

The loop mode is an **orchestrator**, not a transport. It does not expose the tool registry — instead, it drives opencode programmatically via `opencode run --auto` subprocesses. It has its own package (`loop/`) and entry point (`main_loop`), but reuses `path_manager` and `sak.common` helpers.

```
main.py (main_loop)
    |
    v
loop/loop_engine.py  (argparse, config)
    |
    v
loop/loop_lib/engine_core.py  (run_loop)
    |
    +---> opencode_runner.py  (opencode run --auto, --format json)
    |         |
    |         v
    |     process_guard.py  (subprocess timeout + kill)
    |
    +---> verdict.py  (parse police JSON verdict)
    |
    +---> session_monitor.py  (query opencode.db for diagnostics)
```

The loop alternates between a **programmer** agent (build mode, continuing session) and a **police** agent (custom read-only agent, fresh session each time). The police evaluates the project against the specification and returns a structured JSON verdict. If not satisfied, feedback is fed back to the programmer and the loop repeats.

See `docs/agent/development/loop-engine.md` (in host project: `docs/agent/loop-engine-design.md`) for the full design.

---

## Key Design Decisions

- **Process Isolation:** All three transports (CLI, API, MCP) call `_run_in_subprocess()` which spawns a clean Python subprocess for each tool invocation. This prevents shared state corruption, memory leaks, and ensures a clean environment for each run.
- **Single Manager, Three Transports:** `tool_manager` handles CLI, API, and MCP exposure in one class. Each transport method reads from the same shared registry of `tool_info` objects, which are built once at tool registration time by inspecting the function signature.
- **Auto-Discovery:** Tools are discovered by scanning directories for `@tool`-decorated functions. No manual registration is needed — just place a `.py` file in the right folder.
- **Response Contract:** All tools return a standardized dict: `{"status": "success", "data": ...}` or `{"status": "error", "error": ...}`.

---

## Target Project Mechanism

The `--target-project` argument (available on all entry points: `mcp-server`, `api`, `cli`) specifies the host project the tools should operate on — a separate codebase that is not the `project-mcp-tools` directory itself.

### Key Components

- **`tools/path_manager.py`:** Injectable `path_manager` class — `set_target_root(path)` updates the target, `to_env()` serializes paths for subprocess propagation, `ensure_path()` adds project root to `sys.path`.
- **`main.py`:** Parses `--target-project` via `parse_known_args()` and calls `set_target_root()`. Remaining args are forwarded (critical for `run_cli` which needs the tool name and its args).
- **`tools/tool.py` `run_in_subprocess()`:** Reads the target root via `get_target_root()`, passes it to the child process via `PROJECT_MCP_TARGET_ROOT` env var and as `cwd`.
- **Subprocess Entry:** `python -m main tool_name` — `main.py` runs without `--target-project` in argv, but `get_target_root()` reads the env var. `execute_tool()` runs the tool in-process.

This design keeps all tool code (`cpp/`, `git/`, `lib/`, `python/`) agnostic to where the host project lives — they simply call `get_target_root()` to resolve paths.

---

## Centralized Configuration

Global configuration is centralized in `lib/project_config.py`:

| Key | Value |
|-----|-------|
| `max_threads` | Auto-detected CPU count |
| `author.name` | From `git config user.name` |
| `author.email` | From `git config user.email` |
| `paths.tools` | `"."` (project root) |
| `paths.docs` | `"docs"` |
| `paths.templates` | `"docs/templates"` |
| `locale.datetime-format` | `"%Y-%m-%d %H:%M:%S"` |
| `system.pattern.line_break` | `"\n"` |

C++ configuration (`cpp/cpp_lib/cpp_project_config.py`) adds:
- Compiler: Clang++ with C++23, 64-bit, balanced optimization
- Warnings: `-Wall -Wextra -Werror -Wfatal-errors`
- Static analysis: cppcheck (exhaustive mode)
- Build paths: `source/`, `include/`, `tests/`, `tests/adhoc/`, `build/`, `dist/`
