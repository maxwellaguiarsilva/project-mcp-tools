# project-mcp-tools

A Python framework that exposes developer tools simultaneously through three protocols: **MCP** (Model Context Protocol), **REST API**, and **CLI** — all from a single, shared tool registry.

## Overview

`project-mcp-tools` solves the problem of maintaining separate tool backends for different consumers. Write a tool once using the `@tool()` decorator, and it becomes instantly available to:

- **AI assistants** via the MCP protocol (powered by FastMCP)
- **HTTP clients** via a REST API (powered by FastAPI + uvicorn)
- **Terminal users** via a CLI (powered by argparse)

The bundled tools cover C++ development (compile, static analysis, formatting, class/test scaffolding, include tree analysis), Python formatting verification, and git operations — all with process isolation through subprocess execution.

## Installation

**Requirements:** Python 3.14+, `uv` package manager

```bash
# Clone the repository
git clone <repository-url>
cd project-mcp-tools

# Install dependencies
uv sync
```

## Usage

### MCP Server

Starts a FastMCP server that AI assistants can connect to:

```bash
uv run mcp-server
```

Configure your MCP client to use this server. For example, in `opencode.json` at the root of **the host project** (the project you want the tools to operate on, not the `project-mcp-tools` directory itself):

```json
{
  "mcp": {
    "project-mcp-tools": {
      "type": "local",
      "command": ["uv", "--directory", "project-mcp-tools", "run", "mcp-server", "--target-project", "../my-host-project"]
    }
  }
}
```

> **Important:** `--directory` tells `uv` where to find the `project-mcp-tools` package (pyproject.toml, dependencies, venv). `--target-project` sets the working directory for the MCP process and all its subprocesses — this is the project the tools will actually operate on. The path is resolved relative to `project-mcp-tools/` (since `uv --directory` changes the working directory). Without this separation, git/cpp/python tools would operate inside `project-mcp-tools/` instead of your host project.

### REST API Server

Starts a FastAPI server on `http://0.0.0.0:8000`:

```bash
uv run api --target-project ../my-host-project
```

Each tool is exposed as `POST /tools/<tool_name>`. Query parameters from the tool's function signature become fields in the JSON request body.

Example request:

```bash
curl -X POST http://localhost:8000/tools/git_quick_upload \
  -H "Content-Type: application/json" \
  -d '{"message": "my commit"}'
```

Swagger UI is available at `http://localhost:8000/docs`.

### CLI

Invoke any tool from the terminal:

```bash
uv run cli --target-project ../my-host-project git_quick_upload --message "your commit message"
```

`--target-project` must come before the tool name. Tools that don't reference the host project (e.g., `get_random_number`) can be called without `--target-project`.

## Tool Catalog

### General

| Tool | Signature | Description |
|------|-----------|-------------|
| `create_image` | `(description: str, file_name: str \| None = None) -> str` | Generates an image using Gemini (model `gemini-3.1-flash-lite-image`) from the given text description. Saves to `resources/images/` |
| `debug` | `() -> str` | Returns environment debugging information (cwd, paths, env vars) |
| `get_random_number` | `(start: int = 1, end: int = 100) -> str` | Returns a random number between start and end |

### Git

| Tool | Signature | Description |
|------|-----------|-------------|
| `git_discard_changes` | `() -> str` | Discards all uncommitted changes and removes untracked files. Reverts to HEAD |
| `git_pull_submodules` | `() -> str` | Updates every submodule to the latest remote commit (requires clean submodules); the pointer bump is left uncommitted |
| `git_quick_upload` | `(message: str) -> str` | Performs `git pull`, submodule update, `git add .`, `git commit -m <message>`, and `git push` |

### Python

| Tool | Signature | Description |
|------|-----------|-------------|
| `python_analyze` | `() -> str` | Applies `python_code_verifier` on all `*.py` files in the tools directory |
| `python_clear` | `() -> str` | Removes all `__pycache__` directories under the current directory |
| `python_code_verifier` | `(files: list[str]) -> str` | Verifies Python formatting rules for specified files |

### C++

| Tool | Signature | Description |
|------|-----------|-------------|
| `cpp_analyze` | `() -> str` | Applies formatting fixes on all `.cpp`/`.hpp` files, then runs cppcheck static analysis |
| `cpp_code_verifier` | `(files: list[str]) -> str` | Verifies C++ formatting rules for specified files |
| `cpp_compile` | `() -> str` | Compiles the entire C++ project in parallel using Clang |
| `cpp_create_class` | `(class_hierarchy: str, include_list: list[str] = [], using_list: list[str] = [], create_header_only: bool = False) -> str` | Scaffolds a new C++ class from a hierarchy string (e.g., `"game/player"`) |
| `cpp_create_test` | `(hierarchy: str, flg_adhoc: bool = False, include_list: list[str] = []) -> str` | Scaffolds a C++ test file |
| `cpp_analyze_include_tree` | `(file_path: str = None) -> str` | Displays the recursive include dependency tree of a C++ file. Defaults to the project main file |

## Project Structure

```
project-mcp-tools/
├── main.py                    # Entry point — builds tool_manager, starts servers
├── pyproject.toml             # Project config, dependencies, entry points
├── tools/                     # Core engine package
│   ├── __init__.py
│   ├── tool_manager.py        # Core orchestrator — shared registry, tool folder loading, subprocess dispatch, CLI/API/MCP exposure
│   ├── tool.py                # @tool() decorator, ToolInfo/ParameterInfo models, response contract helpers
│   ├── path_manager.py        # Project/target root resolution — injectable, no global state
│   └── folder_scanner.py      # Auto-discovers @tool-decorated functions in directories
├── general/                    # General-purpose tools (no host project dependency)
│   ├── create_image.py         # Gemini image generation tool
│   ├── debug.py                # Environment debugging tool
│   └── get_random_number.py    # Random number generator
├── sak/
│   ├── common.py              # Utilities (process creation, JSON, assertions)
│   └── fso/                   # File system objects
├── lib/
│   ├── base_verifier.py       # Abstract regex-based code formatter
│   ├── project_config.py      # Global project configuration
│   ├── project_file.py        # Abstract source file with license header management
│   └── template.py            # Jinja-like template engine with imports and lists
├── cpp/
│   ├── analyze.py             # C++ full analysis tool
│   ├── code_verifier.py       # C++ formatting verification tool
│   ├── compile.py             # C++ parallel compilation tool
│   ├── create_class.py        # C++ class scaffolding tool
│   ├── create_test.py         # C++ test scaffolding tool
│   ├── include_tree.py        # C++ include dependency tree tool
│   └── cpp_lib/               # C++ domain library (compiler, model, verifier, build)
├── python/
│   ├── analyze.py             # Python full analysis tool
│   ├── code_verifier.py       # Python formatting verification tool
│   └── python_lib/            # Python domain library (model, verifier, config)
├── git/
│   ├── discard_changes.py     # Git reset + clean tool
│   └── quick_upload.py        # Git pull/add/commit/push tool
├── resources/
│   └── images/               # Generated images (from create_image tool)
├── .agents/
│   └── skills/               # AI assistant skills (compliance audit, uv package manager)
└── docs/
    ├── templates/             # Template files for class/test scaffolding (user zone)
    ├── example/               # Usage examples (e.g. google-genai.py) (user zone)
    └── agent/                 # AI-managed knowledge base (architecture, guides, workflows, status)
        ├── architecture.md    # System architecture and design decisions
        ├── development/       # Tool development guide
        ├── style-guide/       # Coding style guides
        ├── workflow/          # Workflow documentation
        └── status.md          # Agent task status
```

## Architecture

The system is built around a central `tool_manager` object that holds the shared tool registry and handles all three transports (CLI, REST API, and MCP).

For a detailed breakdown of the system architecture, design decisions, and target project mechanism, see the [System Architecture](docs/agent/architecture.md) guide.

## Adding a New Tool

To add a new tool, create a Python file in an existing tool folder (or a new one) and decorate your function with `@tool()`.

For a step-by-step tutorial and guidelines on structuring the tool layer and domain libraries, see the [Tool Development Guide](docs/agent/development/adding-tools.md).

## Configuration

Global and domain-specific configurations are centralized in the codebase. For a complete list of configuration keys and values, see [System Architecture - Centralized Configuration](docs/agent/architecture.md#centralized-configuration).

## Coding Conventions

All code in this project must adhere to strict guidelines, including the exclusive use of `snake_case` for all identifiers and specific spacing rules. For the complete set of guidelines, see the [Python Style Guide](docs/agent/style-guide/python.md).

## License

GNU General Public License v3.0 — see the license headers in source files for details.
