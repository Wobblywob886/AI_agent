# OOP AI Agent (C++23)

Course project: Object-Oriented AI Agent with tool calling, ReAct-style loop, skill loading, LLM client integration, harness and benchmark evaluation.

## Team
- 25127307
- 25127011
- 25127109

## Team Contributions
- **25127109**: Agent core (`AgentLoop`, `SkillLoader`, `LoopDetector`), LLM client (`OllamaClient`), `main.cpp` integration
- **25127307**: Tool system (`Tool`, `CalculatorTool`, `FileTool`, `ExecTool`, `ToolRegistry`)
- **25127011**: Harness/evaluation (`HarnessRunner`, evaluators, `Trajectory`), benchmark (`tasks.json`, `run_eval`), skills and documentation

> Adjust the MSSV-role mapping above if your internal split differs.

## Features
- OOP architecture with abstract interfaces (`Tool`, `LLMClient`, `Evaluator`)
- Tool Registry pattern
- ReAct-style AgentLoop (think → act → observe)
- Skill loading from Markdown files
- Loop detection
- Benchmark harness + trajectory logging
- Optional real LLM via HTTP client (Colab/ngrok or local endpoint)

## Project Structure
```text
src/
  agent/          AgentLoop, SkillLoader, LoopDetector
  client/         LLMClient, OllamaClient
  common/         shared headers / json
  tools/          Calculator, File, Exec, ToolRegistry
  harness/        evaluators, trajectory, harness runner
benchmark/        tasks.json, run_eval.cpp
skills/           skill markdown files
main.cpp
CMakeLists.txt
diagrams/         class / sequence / component diagrams
