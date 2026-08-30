#ifndef CALCULATOR_TOOL_H
#define CALCULATOR_TOOL_H

#include "tool.h"

class CalculatorTool : public Tool {
public:
    std::string getName() const override;
    std::string getDescription() const override;
    std::optional<std::string> execute(const std::string& args) override;
};

#endif // CALCULATOR_TOOL_H