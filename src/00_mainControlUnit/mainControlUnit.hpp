#pragma once

#include <cstdint>

#include "SafetyInput.hpp"
#include "SetpointManager.hpp"
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
    void setUiSpeedCommandStep(int step10);

    /// Rohzustände der Sicherheits-Eingänge (von HW-Ebene/GPIO).
    void setSafetyInputs(const SafetyInput::Inputs& in);

    /// Zyklische Steuerung (z.B. alle 10 ms aufrufen).
    void tick();

    /// Monitoring / Debug-Informationen:
    int currentSetpointRpm() const { return setpointManager_.currentSetpointRpm(); }
    int targetSetpointRpm() const { return setpointManager_.targetSetpointRpm(); }

    bool isMotorEnabled() const { return motorActuator_.isEnabled(); }
    std::uint8_t dutyCyclePercent() const { return motorActuator_.dutyCyclePercent(); }

    SafetyState safetyState() const { return safetyInput_.state(); }
    bool isSafetyOk() const { return safetyInput_.isSafetyOk(); }

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
};
