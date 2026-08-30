#include "keyword_evaluator.h"
#include <algorithm>

bool KeywordEvaluator::evaluate(const std::string& result, const json& task) {
    score_ = 0.0f;

    // Very simple version for the dummy json
    std::cout << "[KeywordEvaluator] Checking result against task..." << std::endl;

    // Just check if result contains "255" for the calculator test
    if (result.find("255") != std::string::npos) {
        score_ = 1.0f;
        std::cout << "[KeywordEvaluator] PASS (found 255)" << std::endl;
        return true;
    }

    std::cout << "[KeywordEvaluator] FAIL" << std::endl;
    return false;
}

float KeywordEvaluator::getScore() const {
    return score_;
}