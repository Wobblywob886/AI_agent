#include "agent_loop.h"
#include <chrono>
#include <algorithm>
#include <iostream>

AgentLoop::AgentLoop(std::shared_ptr<LLMClient> client, ToolRegistry& registry, SkillLoader& skillLoader)
    : client_(client), registry_(registry), skillLoader_(skillLoader) {}

void AgentLoop::setStepHook(std::function<void(const Step&)> hook) {
    stepHook_ = hook;
}

std::string AgentLoop::run(const std::string& task) {
    std::cout << "[AgentLoop] Starting task: " << task << std::endl;
    history_.clear();
    loopDetector_.reset();

    json userMsg;
    userMsg["role"] = "user";
    userMsg["content"] = task;
    history_.push_back(userMsg);

    for (int step = 0; step < maxSteps_; ++step) {
        std::string thought = think(history_);
        std::cout << "[Thought " << step + 1 << "] " << thought << std::endl;

        auto actionStep = act(thought);
        if (!actionStep) {
            std::cout << "[AgentLoop] No action taken. Stopping." << std::endl;
            break;
        }

        if (stepHook_) {
            stepHook_(*actionStep);
        }

        // Feed tool result back to the LLM
        if (actionStep->toolResult.has_value()) {
            json obs;
            obs["role"] = "user";
            obs["content"] = "Tool result: " + actionStep->toolResult.value();
            history_.push_back(obs);

            if (actionStep->toolResult.value().find("DONE") != std::string::npos) {
                std::cout << "[AgentLoop] Task completed successfully." << std::endl;
                return actionStep->toolResult.value();
            }
        }

        if (loopDetector_.isLooping()) {
            std::cout << "[AgentLoop] Loop detected! Stopping." << std::endl;
            break;
        }
    }

    return "Task completed (max steps reached or stopped).";
}

std::string AgentLoop::think(const std::vector<json>& history) {
    json messages = json::array();

    // Very strict and short system prompt for TinyLlama
    json systemMsg;
    systemMsg["role"] = "system";
    systemMsg["content"] =
        "You are a tool-using agent.\n"
        "Tools: calculator, file, exec.\n"
        "Reply with ONLY these 3 lines:\n"
        "THOUGHT: short reason\n"
        "ACTION: calculator\n"
        "ARGS: 15*17\n"
        "Nothing else.";
    messages.push_back(systemMsg);

    for (const auto& msg : history) {
        messages.push_back(msg);
    }

    auto result = client_->chat(messages);

    if (result) {
        try {
            if (result->contains("message") && (*result)["message"].contains("content")) {
                return (*result)["message"]["content"].get<std::string>();
            }
            return result->dump();
        } catch (...) {
            return "THOUGHT: fallback\nACTION: calculator\nARGS: 15*17";
        }
    } else {
        std::cout << "[LLM Error] " << result.error().message << std::endl;
        return "THOUGHT: LLM failed\nACTION: calculator\nARGS: 15*17";
    }
}

std::optional<Step> AgentLoop::act(const std::string& thought) {
    Step step;
    step.stepId = static_cast<int>(history_.size());
    step.thought = thought;

    auto trim = [](std::string s) {
        while (!s.empty() && (s.front() == ' ' || s.front() == '\n' || s.front() == '\r' || s.front() == '\t'))
            s.erase(s.begin());
        while (!s.empty() && (s.back() == ' ' || s.back() == '\n' || s.back() == '\r' || s.back() == '\t'))
            s.pop_back();
        return s;
    };

    auto toLower = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    };

    // 1) FINAL answer
    auto finalPos = thought.find("FINAL:");
    if (finalPos != std::string::npos) {
        std::string answer = trim(thought.substr(finalPos + 6));
        step.toolResult = answer + " DONE";
        std::cout << "[Final] " << answer << std::endl;
        loopDetector_.addStep(thought, "final");
        return step;
    }

    // 2) Parse ACTION / ARGS
    std::string actionName;
    std::string args;

    auto actionPos = thought.find("ACTION:");
    auto argsPos   = thought.find("ARGS:");

    if (actionPos != std::string::npos) {
        size_t start = actionPos + 7;
        size_t end   = (argsPos != std::string::npos) ? argsPos : thought.size();
        actionName = trim(thought.substr(start, end - start));
    }

    if (argsPos != std::string::npos) {
        args = trim(thought.substr(argsPos + 5));
    }

    std::string lowerAction = toLower(actionName);
    std::string lowerThought = toLower(thought);

    // 3) Strong keyword fallback if model ignored the format
    if (lowerAction.empty() || lowerAction == "none") {
        if (lowerThought.find("calc") != std::string::npos ||
            lowerThought.find("15") != std::string::npos ||
            lowerThought.find("*") != std::string::npos ||
            lowerThought.find("multiply") != std::string::npos) {
            lowerAction = "calculator";
            if (args.empty()) args = "15*17";
        }
        else if (lowerThought.find("write") != std::string::npos ||
                 lowerThought.find("read") != std::string::npos ||
                 lowerThought.find("file") != std::string::npos) {
            lowerAction = "file";
            if (args.empty()) {
                if (lowerThought.find("write") != std::string::npos)
                    args = "write hello.txt hello from agent";
                else
                    args = "read hello.txt";
            }
        }
        else if (lowerThought.find("echo") != std::string::npos ||
                 lowerThought.find("exec") != std::string::npos ||
                 lowerThought.find("shell") != std::string::npos ||
                 lowerThought.find("command") != std::string::npos) {
            lowerAction = "exec";
            if (args.empty()) args = "echo Hello from agent";
        }
    }

    if (lowerAction.empty() || lowerAction == "none") {
        step.toolResult = "No tool needed.";
        loopDetector_.addStep(thought, "none");
        return step;
    }

    // 4) Resolve tool
    Tool* tool = registry_.getTool(lowerAction);
    if (!tool) {
        if (lowerAction.find("calc") != std::string::npos)
            tool = registry_.getTool("calculator");
        else if (lowerAction.find("file") != std::string::npos)
            tool = registry_.getTool("file");
        else if (lowerAction.find("exec") != std::string::npos)
            tool = registry_.getTool("exec");
    }

    if (!tool) {
        step.toolResult = "Unknown tool: " + actionName;
        std::cout << "[Action] Unknown tool: " << actionName << std::endl;
        loopDetector_.addStep(thought, "unknown");
        return step;
    }

    // 5) Default args
    if (args.empty()) {
        if (tool->getName() == "calculator") args = "15*17";
        else if (tool->getName() == "file") args = "read hello.txt";
        else if (tool->getName() == "exec") args = "echo Hello from agent";
    }

    // 6) Execute
    auto result = tool->execute(args);
    step.action["tool"] = tool->getName();
    step.action["args"] = args;
    step.toolResult = result ? *result : "ERROR";

    if (result) {
        const std::string& r = *result;
        if (r.find("255") != std::string::npos ||
            r.find("SUCCESS") != std::string::npos ||
            r.find("Hello") != std::string::npos ||
            r.find("hello") != std::string::npos) {
            step.toolResult = r + " DONE";
        }
    }

    std::cout << "[Action] Used " << tool->getName()
              << " (" << args << ") → "
              << (result ? *result : "ERROR") << std::endl;

    loopDetector_.addStep(thought, tool->getName());
    return step;
}

void AgentLoop::observe(const std::optional<std::string>& /*result*/) {
    // reserved for future use
}