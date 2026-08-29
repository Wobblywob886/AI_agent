#include "common/Header.h"
#include "tools/tool_registry.h"
#include "tools/calculator_tool.h"
#include "tools/file_tool.h"
#include "client/ollama_client.h"  
#include "tools/exec_tool.h"
#include "agent/agent_loop.h"      // ← Add
#include "agent/skill_loader.h"    // ← Add

int main() {
    std::cout << "--- AI Agent OOP Project - C++23 Started ---" << std::endl;

    ToolRegistry registry;

    // Register tools
    registry.registerTool(std::make_unique<CalculatorTool>());
    registry.registerTool(std::make_unique<FileTool>());
    registry.registerTool(std::make_unique<ExecTool>());

    // Test calculator
    auto calc = registry.getTool("calculator");
    if (calc) {
        auto result = calc->execute("15 * 17");
        std::cout << "Test: 15 * 17 = " << (result ? *result : "ERROR") << std::endl;
    }

    std::cout << "\nTool registration successful!" << std::endl;
    std::cout << registry.getSchemas() << std::endl;

    // Test OllamaClient
    std::shared_ptr<LLMClient> client = std::make_shared<OllamaClient>("https://lyricism-untimed-paralyses.ngrok-free.dev");
    std::cout << "OllamaClient initialized successfully!" << std::endl;

    // Test AgentLoop
    SkillLoader skillLoader("../skills");
    AgentLoop agent(client, registry, skillLoader);
    std::string result = agent.run("Calculate 15 * 17");
    std::cout << "AgentLoop result: " << result << std::endl;

    return 0;
}