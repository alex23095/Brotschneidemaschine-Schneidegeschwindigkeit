// motorActuator.cpp
#include "motorActuator.hpp"

namespace {
    // Minimaler Duty-Cycle, bei dem der Motor sinnvoll anlaufen kann.
    // Kann bei Bedarf an echte Hardware angepasst werden.
    constexpr std::uint8_t kMinDutyPercent = 10U; // 10 %
}

MotorActuator::MotorActuator(int minRpm, int maxRpm)
    : minRpm_{ minRpm }
    , maxRpm_{ maxRpm }
    , rpmCmd_{ 0 }
    , safetyOk_{ false }
    , enabled_{ false }
    , dutyCyclePercent_{ 0U }
    , manualDutyActive_{ false }
    , manualDutyPercent_{ 0U }
{
    if (minRpm_ < 0) {
        minRpm_ = 0;
    }
    if (maxRpm_ < minRpm_) {
        maxRpm_ = minRpm_;
    }
}

int MotorActuator::clampRpm(int rpm) const
{
    if (rpm <= 0) {
        return 0;
    }
    if (rpm < minRpm_) {
        return minRpm_;
    }
    if (rpm > maxRpm_) {
        return maxRpm_;
    }
    return rpm;
}

std::uint8_t MotorActuator::mapRpmToDuty(int rpm) const
{
    const int clamped = clampRpm(rpm);

    if (clamped <= 0) {
        return 0U;
    }

    // Bereich der möglichen Drehzahl
    const int rpmRange = maxRpm_ - minRpm_;
    if (rpmRange <= 0) {
        return 0U;
    }

    // lineare Interpolation zwischen minRpm_ und maxRpm_
    const int rpmOffset = clamped - minRpm_;              // 0 .. rpmRange
    const int span = 100 - static_cast<int>(kMinDutyPercent); // verbleibender Duty-Bereich

    int duty = static_cast<int>(kMinDutyPercent)
        + (rpmOffset * span) / rpmRange;

    if (duty < 0)      duty = 0;
    else if (duty > 100) duty = 100;

    return static_cast<std::uint8_t>(duty);
}

void MotorActuator::setCommandRpm(int rpm)
{
    rpmCmd_ = clampRpm(rpm);
    manualDutyActive_ = false;
}

void MotorActuator::setSafetyOk(bool ok)
{
    safetyOk_ = ok;
}

void MotorActuator::setManualDutyCycle(std::uint8_t dutyPercent)
{
    manualDutyActive_ = true;
    if (dutyPercent > 100U) {
        manualDutyPercent_ = 100U;
    }
    else {
        manualDutyPercent_ = dutyPercent;
    }
}

void MotorActuator::update()
{
    // Safety hat immer Priorität:
    if (!safetyOk_) {
        enabled_ = false;
        dutyCyclePercent_ = 0U;
        return;
    }

    if (manualDutyActive_) {
        enabled_ = manualDutyPercent_ > 0U;
        dutyCyclePercent_ = manualDutyPercent_;
        return;
    }

    if (rpmCmd_ <= 0) {
        enabled_ = false;
        dutyCyclePercent_ = 0U;
        return;
    }

    enabled_ = true;
    dutyCyclePercent_ = mapRpmToDuty(rpmCmd_);
}