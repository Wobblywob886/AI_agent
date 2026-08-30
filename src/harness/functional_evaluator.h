#ifndef FUNCTIONAL_EVALUATOR_H
#define FUNCTIONAL_EVALUATOR_H

#include "evaluator.h"

class FunctionalEvaluator : public Evaluator {
private:
    float score_ = 0.0f;

public:
    bool evaluate(const std::string& result, const json& task) override;
    float getScore() const override;
    std::string getName() const override { return "FunctionalEvaluator"; }
};

#endif