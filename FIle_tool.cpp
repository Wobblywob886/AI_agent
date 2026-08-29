#include "File_tool.h"
#include <fstream>
#include <sstream>
#include <filesystem>

std::string FileTool::getName() const {
    return "file";
}

std::string FileTool::getDescription() const {
    return "Read or write files. Usage: 'read filename' or 'write filename content...'";
}

std::optional<std::string> FileTool::execute(const std::string& args) {
    try {
        std::istringstream iss(args);
        std::string cmd, filename;
        iss >> cmd >> filename;

        if (cmd == "read" || cmd == "read_file") {
            std::ifstream file(filename);
            if (!file.is_open()) return "ERROR: Cannot open file " + filename;

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        } 
        else if (cmd == "write" || cmd == "write_file") {
            std::string content = args.substr(args.find(filename) + filename.length() + 1);
            std::ofstream file(filename);
            if (!file.is_open()) return "ERROR: Cannot write to " + filename;

            file << content;
            return "SUCCESS: Written to " + filename;
        }

        return "ERROR: Unknown command. Use 'read filename' or 'write filename content'";
    } catch (...) {
        return std::nullopt;
    }
}