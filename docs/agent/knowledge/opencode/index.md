# opencode Knowledge

Scope: this directory holds the AI-managed knowledge base about opencode, the AI coding assistant that hosts this project's MCP tools.

## How to Use This Knowledge

1. When the user asks a question about opencode, **search the files in this folder first** and base the answer on what is found here.
2. If the subject is not covered here, **research the opencode source and docs** (https://opencode.ai/docs, https://github.com/anomalyco/opencode) before answering, and base the answer on what is found there.
3. If there is relevant opencode knowledge that is **not** present in these documents, or that is **hard to find** elsewhere, **store it in this folder** so future answers can rely on it.

## Files

- `index.md` — this file: scope, usage rules, and index of documents in this directory.

## Session Context Usage

The `session_context_usage` tool reports how much of the model context window the current opencode chat session is using. It reads the opencode SQLite database directly — no HTTP server needed.

### Data Locations

| What | Where |
|------|-------|
| Session/message database | `$XDG_DATA_HOME/opencode/opencode.db` (default `~/.local/share/opencode/opencode.db`) |
| Database override | env var `OPENCODE_DB` (absolute path, or filename inside the data dir) |
| Models cache (context limits) | `$XDG_CACHE_HOME/opencode/models.json` (default `~/.cache/opencode/models.json`), from models.dev |
| Legacy JSON storage (pre-1.18) | `$XDG_DATA_HOME/opencode/storage/session/...` |

### Relevant Database Schema

The `session` table stores per-session data with token columns:

- `directory` — host project root of the session (used to find the current session)
- `parent_id` — set for child/subagent sessions (`NULL` for the main session)
- `time_archived` — `NULL` for active sessions
- `time_updated` — last activity timestamp (ms epoch)
- `model` — JSON string, e.g. `{"id":"deepseek-v4-flash","providerID":"deepseek"}`
- `tokens_input` / `tokens_output` / `tokens_reasoning` / `tokens_cache_read` / `tokens_cache_write` — session-level aggregates
- `cost` — accumulated session cost

The `message` table stores each message's `data` as JSON. Assistant messages carry the real per-request `tokens` object:

```json
{ "total": 117215, "input": 2189, "output": 220,
  "reasoning": 246, "cache": { "read": 114560, "write": 0 } }
```

### What "context in use" Means

- `context_used` = `input + cache.read + cache.write` of the **last request with usage** — the actual prompt size sent to the model (cached tokens are still part of the context window).
- `last_request.total` = the provider-reported total (adds output + reasoning).
- `context_percent` = `context_used / context_limit * 100`.
- The last message of a turn is still being generated and reports zero tokens until it finishes; the tool scans the most recent messages to find the last one that actually has usage.

### How the Tool Resolves Things

1. Current session = most recent `time_updated`, active (`time_archived IS NULL`) session whose `directory` matches the target project, preferring `parent_id IS NULL` (main session over subagents).
2. Context limit = `models.json[providerID].models[modelID].limit.context`, overridable via the `context_limit` parameter.
