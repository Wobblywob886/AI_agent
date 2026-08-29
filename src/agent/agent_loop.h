#ifndef AGENT_LOOP_H
#define AGENT_LOOP_H

#include "../common/Header.h"
#include "../client/llm_client.h"
#include "../tools/tool_registry.h"
#include "skill_loader.h"
#include "loop_detector.h"

struct Step {
    int stepId = 0;
    std::string thought;
    json action;
    std::optional<std::string> toolResult;
    int tokensUsed = 0;
    long latencyMs = 0;
};

class AgentLoop {
private:
    std::shared_ptr<LLMClient> client_;
    ToolRegistry& registry_;
    SkillLoader& skillLoader_;
    LoopDetector loopDetector_;
    std::vector<json> history_;
    int maxSteps_ = 15;
    std::function<void(const Step&)> stepHook_;

public:
    AgentLoop(std::shared_ptr<LLMClient> client, ToolRegistry& registry, SkillLoader& skillLoader);

    void setStepHook(std::function<void(const Step&)> hook);
    std::string run(const std::string& task);

protected:
    virtual std::string think(const std::vector<json>& history);
    virtual std::optional<Step> act(const std::string& thought);
    virtual void observe(const std::optional<std::string>& result);
};

#endif // AGENT_LOOP_H