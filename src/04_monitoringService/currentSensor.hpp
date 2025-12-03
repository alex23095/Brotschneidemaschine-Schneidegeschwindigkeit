#pragma once

#include <cstdint>
#include <chrono>

/// CurrentSensor
/// - simuliert eine Strommessung in Milliampere
/// - stellt Grenzwertprüfung für Überstrom bereit
class CurrentSensor {
public:
    explicit CurrentSensor(std::uint16_t overcurrentThresholdMa = 8000U);

    /// Liefert den zuletzt erfassten Stromwert in Milliampere.
    /// Ohne echte Hardware wird ein konservativer Simulationswert verwendet,
    /// der über setSimulatedCurrentMa() angepasst werden kann.
    std::uint16_t readCurrent();

    /// Prüft, ob ein übergebener Stromwert den Grenzwert überschreitet.
    bool checkOvercurrent(std::uint16_t currentMa) const;

    /// Anpassung des Simulationswertes (optional für Tests oder UI-Hooks).
    void setSimulatedCurrentMa(std::uint16_t currentMa) { simulatedCurrentMa_ = currentMa; }

    /// Zeitstempel der letzten Messung (steady_clock, Millisekunden).
    std::uint64_t lastSampleTimestampMs() const { return lastSampleTimestampMs_; }

private:
    std::uint16_t overcurrentThresholdMa_;
    std::uint16_t simulatedCurrentMa_;
    std::uint64_t lastSampleTimestampMs_;
};