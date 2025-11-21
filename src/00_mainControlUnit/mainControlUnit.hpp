#pragma once
#include <chrono>
#include <cstdint>
#include "safetyInput.hpp"
#include "setPointManager.hpp"
#include "motorActuator.hpp"

/// MainControlUnit:
/// - zentrale Steuerlogik
/// - bindet SafetyInput, SetpointManager und MotorActuator zusammen
/// - Singleton (eine globale Instanz für das System)
class MainControlUnit {
public:
    /// Zugriff auf die Singleton-Instanz.
    static MainControlUnit& instance();

    /// UI-Befehl: Schritt 0..10 (entspricht 0..100 % in 10%-Schritten).
    void setSpeedStep(int step10);

    /// Rohzustände der Sicherheits-Eingänge (von HW-Ebene/GPIO).
    void readInputs(const SafetyInput::Inputs& in);

    /// Zyklische Steuerung (z.B. alle 10 ms aufrufen).
    void executeCycle();

    /// Manuelles Setzen des Duty-Cycle (z.B. für Testzwecke).
    void setDutyCycle(std::uint8_t dutyPercent);

    /// Monitoring / Debug-Informationen:
    int currentSetpointRpm() const { return setpointManager_.currentSetpointRpm(); }
    int targetSetpointRpm() const { return setpointManager_.targetSetpointRpm(); }

    bool isMotorEnabled() const { return motorActuator_.isEnabled(); }
    std::uint8_t dutyCyclePercent() const { return motorActuator_.dutyCyclePercent(); }

    /// Ergebnis der Sicherheitsbewertung.
    SafetyState getSafetyStatus() const { return safetyInput_.state(); }

    /// Dauer des letzten executeCycle-Aufrufs in Millisekunden.
    std::uint32_t getLastCycleTimeMs() const { return lastCycleTimeMs_; }

    SafetyState safetyState() const { return safetyInput_.state(); }
    bool isSafetyOk() const { return safetyInput_.isSafetyOk(); }

    // Legacy-Kompatibilität für bestehenden Aufrufcode
    void setUiSpeedCommandStep(int step10) { setSpeedStep(step10); }
    void setSafetyInputs(const SafetyInput::Inputs& in) { readInputs(in); }
    void tick() { executeCycle(); }

private:
    // Singleton: Konstruktor privat
    MainControlUnit();

    // Kopieren/Bewegen verbieten
    MainControlUnit(const MainControlUnit&) = delete;
    MainControlUnit& operator=(const MainControlUnit&) = delete;
    MainControlUnit(MainControlUnit&&) = delete;
    MainControlUnit& operator=(MainControlUnit&&) = delete;

    // Untergeordnete Module
    SafetyInput      safetyInput_;
    SetpointManager  setpointManager_;
    MotorActuator    motorActuator_;

    // Letzte Roh-Safety-Eingänge
    SafetyInput::Inputs pendingSafetyInputs_;

    // Zykluszeit-Messung
    std::uint32_t lastCycleTimeMs_;
    std::chrono::steady_clock::time_point lastCycleTimestamp_;
};
