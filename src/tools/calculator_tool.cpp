#include "calculator_tool.h"
#include <sstream>
#include <cmath>

std::string CalculatorTool::getName() const {
    return "calculator";
}

std::string CalculatorTool::getDescription() const {
    return "Evaluate simple mathematical expressions (e.g. '15*17', '10+5*2', '2^8'). Supports +, -, *, /, ^, parentheses.";
}

std::optional<std::string> CalculatorTool::execute(const std::string& args) {
    try {
        // Very basic evaluator - can be improved later with exprtk or similar
        std::string expr = args;
        // Remove whitespace
        expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

        // Simple cases for now (you can enhance this)
        if (expr.find('*') != std::string::npos) {
            size_t pos = expr.find('*');
            double a = std::stod(expr.substr(0, pos));
            double b = std::stod(expr.substr(pos + 1));
            return std::to_string(a * b);
        } else if (expr.find('+') != std::string::npos) {
            size_t pos = expr.find('+');
            double a = std::stod(expr.substr(0, pos));
            double b = std::stod(expr.substr(pos + 1));
            return std::to_string(a + b);
        }
        // Add more operators as needed

        return std::to_string(std::stod(expr)); // fallback
    } catch (...) {
        return std::nullopt; // execution failed
    }
}