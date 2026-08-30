#include "trajectory.h"
#include "../agent/agent_loop.h"
#include <fstream>
#include <iostream>

Trajectory::Trajectory(const std::string& taskId, const std::string& model) {
    data_["task_id"] = taskId;
    data_["model"] = model;
    data_["success"] = false;
    data_["steps"] = json::array();
}

void Trajectory::addStep(const Step& step) {
    json s;
    s["step_id"] = step.stepId;
    s["thought"] = step.thought;
    s["action"] = step.action;
    s["tool_result"] = step.toolResult.value_or("");
    s["tokens_used"] = step.tokensUsed;
    s["latency_ms"] = step.latencyMs;

    data_["steps"].push_back(s);
}

void Trajectory::setSuccess(bool success) {
    data_["success"] = success;
}

void Trajectory::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data_.dump(2);
        file.close();
        std::cout << "[Trajectory] Saved to " << filename << std::endl;
    } else {
        std::cout << "[Trajectory] Failed to save " << filename << std::endl;
    }
}