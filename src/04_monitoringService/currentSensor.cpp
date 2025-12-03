#include "currentSensor.hpp"

#include <algorithm>

namespace {
    std::uint64_t nowMs()
    {
        return static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch())
            .count());
    }
}

CurrentSensor::CurrentSensor(std::uint16_t overcurrentThresholdMa)
    : overcurrentThresholdMa_{ overcurrentThresholdMa }
    , simulatedCurrentMa_{ 0U }
    , lastSampleTimestampMs_{ nowMs() }
{
}

std::uint16_t CurrentSensor::readCurrent()
{
    // In Ermangelung echter Hardware liefern wir den Simulationswert zurück.
    lastSampleTimestampMs_ = nowMs();
    return simulatedCurrentMa_;
}

bool CurrentSensor::checkOvercurrent(std::uint16_t currentMa) const
{
    return currentMa > overcurrentThresholdMa_;
}