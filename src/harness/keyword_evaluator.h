#ifndef KEYWORD_EVALUATOR_H
#define KEYWORD_EVALUATOR_H

#include "evaluator.h"

class KeywordEvaluator : public Evaluator {
private:
    float score_ = 0.0f;

public:
    bool evaluate(const std::string& result, const json& task) override;
    float getScore() const override;
    std::string getName() const override { return "KeywordEvaluator"; }
};

#endif