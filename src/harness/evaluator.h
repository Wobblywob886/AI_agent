#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../common/Header.h"

class Evaluator {
public:
    virtual ~Evaluator() = default;
    virtual bool evaluate(const std::string& result, const json& task) = 0;
    virtual float getScore() const = 0;
    virtual std::string getName() const = 0;
};

#endif // EVALUATOR_H