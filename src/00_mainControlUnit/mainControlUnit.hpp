#pragma once
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

    /// Direkte Vorgabe eines Duty-Cycle-Werts (0..100 %).
    void setDutyCycle(std::uint8_t dutyPercent);

    /// Rohzustände der Sicherheits-Eingänge (von HW-Ebene/GPIO).
    void readInputs(const SafetyInput::Inputs& in);

    /// Zyklische Steuerung (z.B. alle 10 ms aufrufen).
    void tick();

    /// Sprint-1-kompatible Bezeichnung für die Zyklusausführung.
    void executeCycle();

    /// Laufzeitmessung des letzten Zyklus in Millisekunden.
    std::uint32_t getLastCycleTimeMs() const { return lastCycleTimeMs_; }

    /// Monitoring / Debug-Informationen:
    int currentSetpointRpm() const { return setpointManager_.currentSetpointRpm(); }
    int targetSetpointRpm() const { return setpointManager_.targetSetpointRpm(); }

    /// Sicherheitsstatus abrufen.
    SafetyState getSafetyStatus() const { return safetyInput_.getSafetyStatus(); }

    bool isMotorEnabled() const { return motorActuator_.isEnabled(); }
    std::uint8_t dutyCyclePercent() const { return motorActuator_.dutyCyclePercent(); }

    bool isSafetyOk() const { return safetyInput_.isSafetyOk(); }

    /// Selbsttest beim Start ausführen.
    bool runSelfTest();

    /// Gesamtstatus für Diagnose / Traceability.
    bool getStatus() const { return selfTestPassed_ && isSafetyOk(); }

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

    // Laufzeitmessung
    std::uint32_t lastCycleTimeMs_;

    // Selbsttest-Ergebnis
    bool selfTestPassed_;
};
