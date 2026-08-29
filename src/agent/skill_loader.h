#ifndef SKILL_LOADER_H
#define SKILL_LOADER_H

#include "../common/Header.h"

class SkillLoader {
private:
    std::filesystem::path skillsDir_;
    std::map<std::string, std::string> skills_;

public:
    SkillLoader(const std::string& dir = "skills");
    void loadAll();
    std::string selectSkill(const std::string& task) const;
    std::string getAllSkillsPrompt() const;
};

#endif // SKILL_LOADER_H