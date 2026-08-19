# Tool Development Guide

This document explains how to add new tools, structure the tool layer, and work with domain libraries in the `project-mcp-tools` framework.

For system architecture, see [System Architecture](../architecture.md). For style guidelines, see the [Python Style Guide](../style-guide/python.md). For standard workflows (e.g., committing/pushing changes), see [Workflows](../workflow/index.md).

---

## Tool Layer Rules

To maintain a clean separation of concerns, all tools must adhere to the following rules:

- **Thin Facades:** Tool files (in `cpp/`, `python/`, `git/`, `general/`) must be **thin facades**. They should only contain the `@tool()` decorated function, its imports, and a return statement. No domain logic, no model classes, and no complex helper functions should live in the tool file.
- **No Cross-Tool Imports:** Tools must **never import other tools** directly. If a tool needs functionality from another domain, import from the corresponding domain library (`*_lib/`) instead.
- **Return Type:** Tool functions must return `str`. The response contract (`contract_success` / `contract_error`) is applied by the runner, not by the tool itself.

---

## Step-by-Step: Adding a New Tool

### 1. Create the Tool File

Create a file in an existing tool folder (e.g., `git/`) or create a new folder. Use the `@tool()` decorator imported from `tools.tool`:

```python
from tools.tool import tool
from my_domain.my_domain_lib.my_logic import perform_logic

@tool( )
def my_new_tool( name: str, verbose: bool = False ) -> str:
    """description of what this tool does
    each line is a separate thought without connecting punctuation"""
    return  perform_logic( name, verbose )
```

### 2. Register the Folder (If New)

If you created a new folder, register it in `main.py` so the scanner can auto-discover it:

```python
m.add_tool_folder( "my_folder" )                     # tools prefixed with "my_folder_"
m.add_tool_folder( "my_folder", flg_use_prefix=False )  # tools use their bare names

# Multiple folders at once:
m.add_tool_folder( [ "git", "python", "cpp" ] )        # each gets its own prefix
```

The tool is now instantly available via MCP, REST API, and CLI — no further configuration needed.

---

## Domain Libraries (`*_lib/`)

All actual business and domain logic must live in domain-specific subdirectories (e.g., `cpp/cpp_lib/`, `python/python_lib/`).

### Verifier Classes

Verifier classes must extend `base_verifier` from `lib/base_verifier.py`. Override `string_comment_ignore_pattern` and `rules`. Do not redefine module-level constants already defined in `base_verifier` (e.g., `re_line_break`).

### Project Models

Project models (classes representing project files) must extend `project_file` from `lib/project_file.py`. Implement `shebang`, `comment_string`, and override `refresh()` to populate domain-specific attributes.

### Config Merge Pattern

To define domain-specific configurations, define a domain-specific dict and merge it with the base config using `deep_update`:

```python
import copy
from sak.common import deep_update
from lib.project_config import project_config

domain_config = {
    "language": {
         "extension": "py"
        ,"comment_string": "#   "
    }
}

domain_config = deep_update( copy.deepcopy( project_config ), domain_config )
```
