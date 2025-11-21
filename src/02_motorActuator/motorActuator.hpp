// motorActuator.hpp
#pragma once

#include <cstdint>

/// MotorActuator:
/// - nimmt einen Drehzahl-Sollwert (RPM) entgegen
/// - berücksichtigt Safety-Freigabe
/// - berechnet daraus einen Duty-Cycle 0..100 % für die PWM
class MotorActuator {
public:
    /// \param minRpm minimale sinnvolle Drehzahl (z.B. 500 U/min)
    /// \param maxRpm maximale Drehzahl (z.B. 3000 U/min)
    MotorActuator(int minRpm = 500, int maxRpm = 3000);

    /// Soll-Drehzahl in U/min (typisch vom SetpointManager.currentSetpointRpm()).
    void setCommandRpm(int rpm);

    /// Ergebnis der Sicherheitskette (SafetyInput / MainControlUnit).
    /// false → Motor wird abgeschaltet, Duty-Cycle = 0 %.
    void setSafetyOk(bool ok);

    /// Zyklischer Aufruf aus der MainControlUnit.
    /// Berechnet Enable-Flag und Duty-Cycle.
    void update();

    /// Alias für Traceability: entspricht update().
    void updateControlLoop() { update(); }

    /// Direkte Vorgabe eines Duty-Cycle-Werts (0..100 %). Überschreibt den aus RPM berechneten Wert.
    void setManualDutyCycle(std::uint8_t dutyPercent);

    /// true, wenn Motor energiert werden darf.
    bool isEnabled() const { return enabled_; }

    /// Duty-Cycle 0..100 %, an PWM-Treiber weiterzugeben.
    std::uint8_t dutyCyclePercent() const { return dutyCyclePercent_; }

    /// Intern verwendeter, geklemmter Drehzahlsollwert.
    int commandRpm() const { return rpmCmd_; }

    /// Alias für Traceability: gibt den aktuellen (geklemmten) RPM-Wert zurück.
    int getMeasuredSpeed() const { return rpmCmd_; }

private:
    int         minRpm_;
    int         maxRpm_;

    int         rpmCmd_;            // zuletzt gesetzter RPM-Sollwert (geklammert)
    bool        safetyOk_;          // letzte bekannte Safety-Freigabe
    bool        enabled_;           // ergibt sich aus rpmCmd_ & safetyOk_
    std::uint8_t dutyCyclePercent_; // 0..100

    bool         manualDutyActive_;
    std::uint8_t manualDutyPercent_;

    int clampRpm(int rpm) const;
    std::uint8_t mapRpmToDuty(int rpm) const;
};