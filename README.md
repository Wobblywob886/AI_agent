# OOP AI Agent (C++23)

Đồ án môn **Lập trình Hướng đối tượng** – Xây dựng AI Agent Framework kết nối LLM (Ollama API / remote endpoint), gồm các tầng:

Agent Loop – Tool Registry – Skill Loader – Harness – Evaluator

## Thành viên
- **25127109 (A)**: Agent core, LLM client, main integration
- **25127307 (B)**: Tools + ToolRegistry
- **25127011 (C)**: Harness, benchmark, skills, documentation

## Tính năng đã triển khai
- Abstract interfaces: `LLMClient`, `Tool`, `Evaluator`
- Tools: `calculator`, `file` (read/write), `exec`
- `ToolRegistry` (register/get/list + allow/deny policy)
- `AgentLoop` (ReAct-style: think → act → observe)
- `SkillLoader` (load `.md` skills, inject guidance)
- `LoopDetector` (repeat / ping-pong detection)
- `HarnessRunner` + trajectory JSON logging
- `KeywordEvaluator` / `FunctionalEvaluator`
- Benchmark runner (`benchmark/tasks.json`, `run_eval`)
- HTTP LLM client (`OllamaClient`) via libcurl
  - có thể trỏ local Ollama hoặc remote Colab/ngrok endpoint

## Cấu trúc thư mục
```text
src/
  agent/       AgentLoop, SkillLoader, LoopDetector
  client/      LLMClient, OllamaClient
  common/      Header.h, json.hpp
  tools/       Tool, ToolRegistry, Calculator/File/Exec tools
  harness/     HarnessRunner, Trajectory, Evaluators
benchmark/     tasks.json, run_eval.cpp
skills/        skill markdown files
docs/          diagrams (optional)
main.cpp
CMakeLists.txt
README.md
LINKS.md
