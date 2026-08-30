#ifndef TOOL_REGISTRY_H
#define TOOL_REGISTRY_H

#include "../common/Header.h"
#include "tool.h"  

#include <set>      

class ToolRegistry {
private:
    std::map<std::string, std::unique_ptr<Tool>> tools_;
    std::set<std::string> allowList_;
    std::set<std::string> denyList_;

public:
    void registerTool(std::unique_ptr<Tool> tool);
    Tool* getTool(const std::string& name);
    std::string getSchemas() const;
    void setPolicy(const std::vector<std::string>& allow, const std::vector<std::string>& deny);
    bool isAllowed(const std::string& name) const;
};

#endif 