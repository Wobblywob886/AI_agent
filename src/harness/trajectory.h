#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "../common/Header.h"

// Forward declaration
struct Step;

class Trajectory {
private:
    json data_;

public:
    Trajectory(const std::string& taskId, const std::string& model);
    void addStep(const Step& step);
    void setSuccess(bool success);
    void saveToFile(const std::string& filename) const;
};

#endif // TRAJECTORY_H