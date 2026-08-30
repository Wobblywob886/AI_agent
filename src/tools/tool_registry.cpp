#include "tool_registry.h"
#include "file_tool.h"
#include "calculator_tool.h"
#include "exec_tool.h"
#include <iostream>

void ToolRegistry::registerTool(std::unique_ptr<Tool> tool) {
    if (tool) {
        tools_[tool->getName()] = std::move(tool);
        std::cout << "[ToolRegistry] Registered: " << tools_.rbegin()->first << std::endl;
    }
}

Tool* ToolRegistry::getTool(const std::string& name) {
    auto it = tools_.find(name);
    return (it != tools_.end()) ? it->second.get() : nullptr;
}

std::string ToolRegistry::getSchemas() const {
    std::string schemas = "Available tools:\n";
    for (const auto& [name, tool] : tools_) {
        schemas += "- " + name + ": " + tool->getDescription() + "\n";
    }
    return schemas;
}

void ToolRegistry::setPolicy(const std::vector<std::string>& allow, const std::vector<std::string>& deny) {
    allowList_ = std::set<std::string>(allow.begin(), allow.end());
    denyList_ = std::set<std::string>(deny.begin(), deny.end());
}

bool ToolRegistry::isAllowed(const std::string& name) const {
    if (!allowList_.empty() && allowList_.find(name) == allowList_.end()) return false;
    if (denyList_.find(name) != denyList_.end()) return false;
    return true;
}