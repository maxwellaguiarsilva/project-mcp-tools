# Simple Upload Workflow

This document outlines the steps for quickly pushing code changes to GitHub without a pull request.

1.  **Fetch Latest Code and Analyze Changes:**
```bash
git pull
git status
git diff
```

2.  **Handle Untracked Files:**
Before committing, ensure all relevant untracked files are staged. Use `.gitignore` to exclude build artifacts (`__pycache__/`, `.venv/`, etc.) so they don't pollute the repository.

3.  **Commit Changes (Grouped by Topic):**
You must analyze the modifications and group them by distinct topics. **It is strictly forbidden to commit unrelated changes in the same commit.** 
To ensure proper separation, follow this step-by-step analysis:
    *   **Categorize:** Review the `git diff` and categorize the changes into themes. Common themes are: feature, fix, refactor, style, docs, chore (configs/dependencies).
    *   **Group Files:** Assign each modified file to its corresponding theme. Files changed for the exact same reason belong in the same group.
    *   **Stage and Commit:** For each group, stage ONLY those specific files and commit them. **Never use `git add .` or `git commit -a` if there are multiple themes.**
The agent should not ask for approval for the commit message and should proceed with the commit automatically. If your commit message contains backticks, you MUST escape them with a backslash to avoid command substitution errors.

5.  **Push and perform a final validation:**
Perform a final check to ensure everything is in order. **The goal is for `git status` to be 100% clean — no modified files, no staged changes, no untracked files.**
```bash
git push
git status
```
Inform the user that the procedure was completed successfully. In this final communication, show exactly which commit message was used, enclosed in `backticks`.

