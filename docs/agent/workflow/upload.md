# Simple Upload Workflow

This document outlines the steps for quickly pushing code changes to GitHub without a pull request.

1.  **Fetch Latest Code:**
    Execute `git pull` to retrieve the most recent code from the remote repository.
    ```bash
    git pull
    ```

2.  **Analyze Changes:**
    Review the `git diff` to understand what modifications have been made in the project's code. Also check for untracked files that may need to be added.
    ```bash
    git status
    git diff
    ```
    If a submodule (e.g., `project-mcp-tools`) shows as `modified`, inspect changes inside it and follow the submodule workflow — see [Consumer Submodule Sync](consumer-submodule-sync.md).

3.  **Handle Untracked Files:**
    Before committing, ensure all relevant untracked files are staged. Use `.gitignore` to exclude build artifacts (`__pycache__/`, `.venv/`, etc.) so they don't pollute the repository.
    ```bash
    git status
    ```

4.  **Commit Changes (Grouped by Topic):**
    You must analyze the modifications and group them by distinct topics. **It is strictly forbidden to commit unrelated changes in the same commit.** 
    
    To ensure proper separation, follow this step-by-step analysis:
    *   **Step 4.1 - Categorize:** Review the `git diff` and categorize the changes into themes. Common themes are: *Feature* (new logic), *Fix* (bug correction), *Refactor* (code cleanup), *Style* (formatting/spacing), *Docs* (markdown/comments), and *Chore* (configs/dependencies).
    *   **Step 4.2 - Group Files:** Assign each modified file to its corresponding theme. Files changed for the exact same reason (e.g., a class and its unit test) belong in the same group.
     *   **Step 4.3 - Stage and Commit:** For each group, stage ONLY those specific files and commit them. **Never use `git add .` or `git commit -a` if there are multiple themes.**
     *   **Submodule changes:** Commit inside the submodule first, then stage the pointer bump in the host project (see [Consumer Submodule Sync](consumer-submodule-sync.md)).
     
     *Example of Separation:*
    If you modified `auth.py` (logic fix) and `README.md` (typo fix), you MUST make two separate commits:
    `git add auth.py && git commit -m "fix token validation logic"`
    `git add README.md && git commit -m "fix typo in setup instructions"`

    The agent should not ask for approval for the commit message and should proceed with the commit automatically.
    **Imperative:** If your commit message contains backticks (``` ` ```), you MUST escape them with a backslash (```\```) to avoid command substitution errors.
    ```bash
    # For each distinct group of changes:
    git add <specific_files_or_directories>
    git commit -m "commit message describing only these changes"
    ```

5.  **Push:**
    Push the changes to the remote repository.
    ```bash
    git push
    ```

6.  **Final Validation:**
    Perform a final check to ensure everything is in order. **The goal is for `git status` to be 100% clean — no modified files, no staged changes, no untracked files.**
    ```bash
    git status
    git diff
    ```

7.  **Communication:**
    Inform the user that the procedure was completed successfully.
    In this final communication, show exactly which commit message was used, enclosed in `backticks`.

