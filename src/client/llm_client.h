#ifndef LLM_CLIENT_H
#define LLM_CLIENT_H

#include "../common/Header.h"

struct APIError {
    int status_code;
    std::string message;
};

class LLMClient {
public:
    virtual ~LLMClient() = default;

    virtual std::expected<json, APIError> chat(const json& messages) = 0;

    // Config methods
    virtual void setModel(const std::string& m) = 0;
    virtual void setTemperature(float t) = 0;
    virtual void setMaxTokens(int n) = 0;
};

#endif // LLM_CLIENT_H