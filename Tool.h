#ifndef TOOL_H
#define TOOL_H

#include "common/Header.h"

class Tool {
public:
    virtual ~Tool() = default;

    virtual std::string getName() const = 0;
    
    virtual std::string getDescription() const = 0;

    virtual std::optional<std::string> execute(const std::string& args) = 0;
};

#endif // TOOL_H