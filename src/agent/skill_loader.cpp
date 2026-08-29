#include "skill_loader.h"
#include <fstream>
#include <iostream>

SkillLoader::SkillLoader(const std::string& dir) : skillsDir_(dir) {
    loadAll();
}

void SkillLoader::loadAll() {
    if (!std::filesystem::exists(skillsDir_)) {
        std::cout << "[SkillLoader] skills/ folder not found. Create it for custom skills." << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(skillsDir_)) {
        if (entry.path().extension() == ".md") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                skills_[entry.path().stem().string()] = buffer.str();
                std::cout << "[SkillLoader] Loaded skill: " << entry.path().filename() << std::endl;
            }
        }
    }

    if (skills_.empty()) {
        std::cout << "[SkillLoader] No .md skill files found." << std::endl;
    }
}

std::string SkillLoader::getAllSkillsPrompt() const {
    if (skills_.empty()) {
        return "You are a helpful AI agent. Use tools when appropriate.";
    }

    std::string prompt = "You have the following skills available:\n\n";
    for (const auto& [name, content] : skills_) {
        prompt += "### " + name + "\n" + content + "\n\n";
    }
    prompt += "Choose the most relevant skill for the current task.";
    return prompt;
}

std::string SkillLoader::selectSkill(const std::string& task) const {
    // Simple keyword matching
    if (task.find("calculate") != std::string::npos || task.find("*") != std::string::npos) {
        if (skills_.count("calculator_expert")) {
            return skills_.at("calculator_expert");
        }
    }
    if (task.find("file") != std::string::npos || task.find("write") != std::string::npos) {
        if (skills_.count("file_manager")) {
            return skills_.at("file_manager");
        }
    }
    return getAllSkillsPrompt();
}