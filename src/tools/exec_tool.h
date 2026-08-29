#ifndef EXEC_TOOL_H
#define EXEC_TOOL_H

#include "Tool.h"

class ExecTool : public Tool {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    std::optional<std::string> execute(const std::string& args) override;
};

#endif // EXEC_TOOL_H