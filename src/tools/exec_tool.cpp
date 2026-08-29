#include "Exec_tool.h"
#include <cstdio>
#include <array>

std::string ExecTool::getName() const {
    return "exec";
}

std::string ExecTool::getDescription() const {
    return "Execute shell commands and return output (e.g. 'ls', 'echo hello', 'dir'). Use with caution.";
}

std::optional<std::string> ExecTool::execute(const std::string& args) {
    try {
        std::array<char, 128> buffer;
        std::string result;
        std::string cmd = args + " 2>&1";  // capture stderr too

#ifdef _WIN32
        FILE* pipe = _popen(cmd.c_str(), "r");
#else
        FILE* pipe = popen(cmd.c_str(), "r");
#endif

        if (!pipe) return "ERROR: Failed to execute command";

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

#ifdef _WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif

        return result.empty() ? "Command executed (no output)" : result;
    } catch (...) {
        return std::nullopt;
    }
}