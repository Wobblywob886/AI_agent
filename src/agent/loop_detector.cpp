#include "loop_detector.h"

void LoopDetector::addStep(const std::string& thought, const std::string& action) {
    history_.push_back(thought + "||" + action);
    if (history_.size() > 10) history_.pop_front();
}

bool LoopDetector::isLooping() const {
    if (history_.size() < threshold_) return false;

    // Type 1: Generic repeat
    const auto& last = history_.back();
    int count = 0;
    for (const auto& entry : history_) {
        if (entry == last) count++;
    }
    if (count >= threshold_) return true;

    // Type 2: Ping-pong
    if (history_.size() >= 4) {
        bool pingpong = true;
        for (size_t i = history_.size() - 1; i >= 2; i -= 2) {
            if (history_[i] != history_[i-2]) {
                pingpong = false;
                break;
            }
        }
        if (pingpong) return true;
    }

    return false;
}

std::string LoopDetector::getLoopType() const {
    return "generic repeat or ping-pong";
}

void LoopDetector::reset() {
    history_.clear();
}