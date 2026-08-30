#ifndef HARNESS_RUNNER_H
#define HARNESS_RUNNER_H

#include "../common/Header.h"
#include "../agent/agent_loop.h"
#include "evaluator.h"
#include "trajectory.h"
#include <memory>
#include <vector>

class HarnessRunner {
private:
    std::shared_ptr<AgentLoop> agent_;
    std::vector<std::unique_ptr<Evaluator>> evaluators_;
    Trajectory trajectory_;

public:
    HarnessRunner(std::shared_ptr<AgentLoop> agent);
    bool runTask(const json& task);
    json runBatch(const std::string& tasksFile);
};

#endif // HARNESS_RUNNER_H