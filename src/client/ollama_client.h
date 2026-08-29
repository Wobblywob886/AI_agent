#ifndef OLLAMA_CLIENT_H
#define OLLAMA_CLIENT_H

#include "llm_client.h"

class OllamaClient : public LLMClient {
private:
    std::string baseUrl_;
    std::string model_;
    float temperature_ = 0.7f;
    int maxTokens_ = 512;

public:
    explicit OllamaClient(const std::string& baseUrl = "http://localhost:11434");
    ~OllamaClient() override = default;

    std::expected<json, APIError> chat(const json& messages) override;
    void setModel(const std::string& m) override;
    void setTemperature(float t) override;
    void setMaxTokens(int n) override;
};

#endif // OLLAMA_CLIENT_H