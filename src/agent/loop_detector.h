#ifndef LOOP_DETECTOR_H
#define LOOP_DETECTOR_H

#include "../common/Header.h"
#include <deque>   // ← Add this

class LoopDetector {
private:
    std::deque<std::string> history_;
    int threshold_ = 3;

public:
    void addStep(const std::string& thought, const std::string& action);
    bool isLooping() const;
    std::string getLoopType() const;
    void reset();
};

#endif // LOOP_DETECTOR_H