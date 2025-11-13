#include "SetpointManager.hpp"

SetpointManager::SetpointManager(int minRpm,
                                 int maxRpm,
                                 int rampRpmPerTick)
    : minRpm_{minRpm}
    , maxRpm_{maxRpm}
    , rampRpmPerTick_{rampRpmPerTick}
    , cmdStep10_{0}
    , targetRpm_{0}
    , currentRpm_{0}
{
    if (minRpm_ < 0) {
        minRpm_ = 0;
    }
    if (maxRpm_ < minRpm_) {
        maxRpm_ = minRpm_;
    }
    if (rampRpmPerTick_ <= 0) {
        rampRpmPerTick_ = 1;
    }
}

int SetpointManager::clampStep(int step) const
{
    if (step < 0) {
        return 0;
    }
    if (step > 10) {
        return 10;
    }
    return step;
}

int SetpointManager::mapStepToRpm(int step) const
{
    // step = 0  → 0 U/min (Motor aus)
    // step = 1  → minRpm_
    // step = 10 → maxRpm_

    if (step <= 0) {
        return 0;
    }

    if (step >= 10) {
        return maxRpm_;
    }

    // Linearverteilung von step 1..10 auf minRpm_..maxRpm_
    // (1 → minRpm_, 10 → maxRpm_)
    const int stepsRange = 9; // (10 - 1)
    const int deltaRpm   = maxRpm_ - minRpm_;

    int index = step - 1; // 0..8
    int rpm   = minRpm_ + (deltaRpm * index) / stepsRange;

    return rpm;
}

void SetpointManager::setCommandStep(int step10)
{
    cmdStep10_ = clampStep(step10);
    targetRpm_ = mapStepToRpm(cmdStep10_);
}

void SetpointManager::update()
{
    if (currentRpm_ == targetRpm_) {
        return;
    }

    if (currentRpm_ < targetRpm_) {
        currentRpm_ += rampRpmPerTick_;
        if (currentRpm_ > targetRpm_) {
            currentRpm_ = targetRpm_;
        }
    } else {
        currentRpm_ -= rampRpmPerTick_;
        if (currentRpm_ < targetRpm_) {
            currentRpm_ = targetRpm_;
        }
    }
}
