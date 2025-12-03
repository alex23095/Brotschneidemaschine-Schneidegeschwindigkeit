#include "monitoringService.hpp"

MonitoringService::MonitoringService(CurrentSensor& sensor, std::uint32_t sampleIntervalMs)
    : sensor_{ sensor }
    , sampleIntervalMs_{ sampleIntervalMs }
    , lastSampleTime_{ std::chrono::steady_clock::now() }
    , lastCurrentMa_{ 0U }
    , overcurrent_{ false }
{
}

bool MonitoringService::updateMonitoring()
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSampleTime_).count();

    if (static_cast<std::uint32_t>(elapsed) < sampleIntervalMs_) {
        return false;
    }

    lastCurrentMa_ = sensor_.readCurrent();
    overcurrent_ = sensor_.checkOvercurrent(lastCurrentMa_);
    lastSampleTime_ = now;
    return true;
}