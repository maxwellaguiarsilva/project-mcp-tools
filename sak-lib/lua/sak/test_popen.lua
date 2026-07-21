-- test_popen.lua
-- This script diagnoses io.popen behavior in Termux.

local function run_command(cmd_str)
    print("\n--- Testing command: " .. cmd_str .. " ---")
    local f, err = io.popen(cmd_str, "r")
    if not f then
        print("Error opening pipe: " .. (err or "unknown error"))
        return
    end
    local output = f:read("*all")
    local status = f:close()

    print("Output:")
    if output and #output > 0 then
        print("```")
        print(output)
        print("```")
    else
        print("(No output or empty string)")
    end

    if status then
        print("Command exited successfully.")
    else
        print("Command failed or terminated abnormally.")
        print("Status: " .. tostring(status))
    end
end

print("--- Lua Version ---")
print(_VERSION)

print("\n--- Environment Variables ---")
run_command("echo $PATH")
run_command("env") -- Print all environment variables

run_command("pwd")
run_command("echo Hello from io.popen")
run_command("ls -la")

print("\n--- Script finished ---")

