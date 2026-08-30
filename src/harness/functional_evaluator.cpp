#include "functional_evaluator.h"
#include <cstdio>
#include <array>

bool FunctionalEvaluator::evaluate(const std::string& /*result*/, const json& /*task*/) {
    score_ = 0.0f;
    std::cout << "[FunctionalEvaluator] Running simple check..." << std::endl;

    // For now just return true (we will improve later)
    score_ = 1.0f;
    std::cout << "[FunctionalEvaluator] PASS (mock)" << std::endl;
    return true;
}

float FunctionalEvaluator::getScore() const {
    return score_;
}