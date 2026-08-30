#ifndef FILE_TOOL_H
#define FILE_TOOL_H

#include "tool.h"

class FileTool : public Tool {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    std::optional<std::string> execute(const std::string& args) override;
};

#endif // FILE_TOOL_H