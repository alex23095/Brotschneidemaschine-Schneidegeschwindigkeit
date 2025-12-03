#pragma once

#include <cstdint>
#include <chrono>

#include "currentSensor.hpp"

/// MonitoringService
/// - pollt den CurrentSensor in einem definierten Intervall
/// - prüft Überstromzustände
/// - stellt den letzten Messwert für die Anwendung bereit
class MonitoringService {
public:
    MonitoringService(CurrentSensor& sensor, std::uint32_t sampleIntervalMs = 500U);

    /// Führt bei abgelaufenem Sample-Intervall eine neue Messung durch.
    /// \return true, wenn eine neue Messung aufgenommen wurde.
    bool updateMonitoring();

    /// Letzter erfasster Stromwert in Milliampere.
    std::uint16_t readCurrent() const { return lastCurrentMa_; }

    /// Flag, ob die letzte Messung einen Überstrom ergab.
    bool checkOvercurrent() const { return overcurrent_; }

    std::uint32_t sampleIntervalMs() const { return sampleIntervalMs_; }

private:
    CurrentSensor& sensor_;
    std::uint32_t sampleIntervalMs_;
    std::chrono::steady_clock::time_point lastSampleTime_;
    std::uint16_t lastCurrentMa_;
    bool overcurrent_;
};