# Performance Report

Investigation into where the latency of `cpp-analyze` (and tool startup in
general) comes from. Measurements taken on an Arch Linux workstation with
12 CPUs (`nproc`) against the `draft-point` target project (81 `.cpp`/`.hpp`
files). Use this report to decide how to address the startup and analysis costs.

---

## 1. Pipeline cost of `cpp-analyze`

The analyze pipeline runs two phases (see `cpp/analyze.py`):
1. `cpp_verifier(...).run()` — regex formatting rules over all files
2. `project_core.run_static_analysis()` — a `cppcheck` invocation

### Phase timing

| Phase | Time |
|-------|------|
| Formatting verifier (`cpp_verifier.run`) | ~0.07s |
| `cppcheck` warm (build dir cached) | ~0.15s |
| `cppcheck` cold (no `--cppcheck-build-dir` cache) | ~25s |
| `uv run` interpreter/import startup (fixed) | ~3.5s |

The verifier is negligible. The analysis cost only appears when cppcheck runs
cold; the `--cppcheck-build-dir=build` cache makes warm reruns near instant.

---

## 2. The dominant cppcheck cost is `--force`

Cold runs with `-j 12`, varying flags (measured directly against cppcheck):

| Variant | real time |
|---------|-----------|
| `exhaustive` + `--force` (previous config) | ~25s |
| `exhaustive` without `--force` | ~10.3s |
| default check-level, no `--force` | ~10.4s |

`--force` roughly **doubles** the runtime because it makes cppcheck check every
`#if` configuration branch. `--check-level=exhaustive` costs almost nothing
extra on this codebase. Mitigation applied: `static_analysis.force` was set to
`false` in `cpp/cpp_lib/cpp_project_config.py`.

---

## 3. Threads do not help cppcheck here

`max-threads` defaults to the CPU count (`get_cpu_count`, 12 here), and cppcheck
is already invoked with `-j 12`. Yet parallelization gains almost nothing:

| Config | real | user |
|--------|------|------|
| `-j 12` exhaustive + force | 24.8s | 28.1s |
| `-j 1`  exhaustive + force | 26.5s | 26.2s |

`user ≈ real` in both cases, so CPU usage is effectively serial. cppcheck's
heavy `--force`/`exhaustive` work per translation unit does not scale across the
12 threads; a single heavy file serializes the run. Adding more threads is not
the answer.

---

## 4. Fixed ~3.5s startup comes from unconditional heavy imports

Every tool invocation (not only `cpp-analyze`) pays a fixed startup cost because
of unconditional imports that happen regardless of which tool runs.

| Source | Cost |
|--------|------|
| `tools/tool_manager.py` imports `fastapi`, `uvicorn`, `fastmcp` at module top level (always, even for the `cli` path) | ~0.9s |
| `main.py _setup_manager` scans all tool folders; the `general` folder loads `general_lib.image_generator`/`image_interpreter`, which `import google.genai` | genai alone ~1.25s |
| `uv` itself (interpreter launch) | ~0.045s |

Confirmed: after `_setup_manager`, `google.genai`, `fastmcp`, `fastapi` and
`uvicorn` are all present in `sys.modules` even for a pure cppcheck invocation.

### Ideas for a later pass (not yet implemented)

- Delay `import google.genai` into the image tool functions instead of at module
  load, and/or load image tools lazily.
- Avoid importing `fastapi`/`uvicorn`/`fastmcp` at module top level in
  `tool_manager.py` when only `run_cli` is used.
- Register tool modules on demand instead of the universal folder scan in
  `_setup_manager`.

---

## 5. Context of the reported ~12s

The commonly reported figure sits between the warm (~0.2s) and cold (~25s)
extremes. The exact number depends on how warm the `--cppcheck-build-dir=build`
cache is at the moment of the run.
