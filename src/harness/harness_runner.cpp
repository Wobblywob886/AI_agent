#include "harness_runner.h"
#include <fstream>
#include <iostream>

HarnessRunner::HarnessRunner(std::shared_ptr<AgentLoop> agent)
    : agent_(agent), trajectory_("task_001", "mock-model") {}

bool HarnessRunner::runTask(const json& task) {
    std::string instruction = task.value("instruction", "Calculate 15 * 17");
    std::cout << "[Harness] Running task: " << task.value("description", "unknown") << std::endl;

    std::string result = agent_->run(instruction);

    bool success = result.find("255") != std::string::npos || result.find("DONE") != std::string::npos;
    trajectory_.setSuccess(success);

    std::string filename = "trajectory_" + task.value("id", "unknown") + ".json";
    trajectory_.saveToFile(filename);

    return success;
}

json HarnessRunner::runBatch(const std::string& tasksFile) {
    std::ifstream f(tasksFile);
    if (!f.is_open()) {
        std::cout << "[Harness] Cannot open " << tasksFile << std::endl;
        return json::array();
    }

    json tasks = json::parse(f);
    json results = json::array();

    int passed = 0;
    int total = 0;

    for (const auto& task : tasks) {
        total++;
        bool ok = runTask(task);
        if (ok) passed++;

        json r;
        r["id"] = task.value("id", "unknown");
        r["success"] = ok;
        results.push_back(r);
    }

    float rate = total > 0 ? (float)passed / total * 100.0f : 0.0f;
    std::cout << "\n=== Batch Evaluation Finished ===" << std::endl;
    std::cout << "Passed: " << passed << " / " << total << " (" << rate << "%)" << std::endl;

    return results;
}