#include "../src/common/Header.h"
#include "../src/client/ollama_client.h"
#include "../src/tools/tool_registry.h"
#include "../src/tools/calculator_tool.h"
#include "../src/tools/file_tool.h"
#include "../src/tools/exec_tool.h"
#include "../src/agent/agent_loop.h"
#include "../src/agent/skill_loader.h"
#include "../src/harness/harness_runner.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== AI Agent Benchmark Runner ===" << std::endl;

    // Setup tools
    ToolRegistry registry;
    registry.registerTool(std::make_unique<CalculatorTool>());
    registry.registerTool(std::make_unique<FileTool>());
    registry.registerTool(std::make_unique<ExecTool>());

    // Setup client + agent
    auto client = std::make_shared<OllamaClient>("http://localhost:11434");
    SkillLoader skillLoader("../skills");
    auto agent = std::make_shared<AgentLoop>(client, registry, skillLoader);

    // Run harness
    HarnessRunner harness(agent);
    json results = harness.runBatch("../benchmark/tasks.json");

    std::cout << "\nBenchmark finished." << std::endl;
    return 0;
}