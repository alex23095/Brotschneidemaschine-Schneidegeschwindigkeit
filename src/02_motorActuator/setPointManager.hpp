#pragma once

#include <cstdint>

/// Verantwortlich für:
/// - Abbildung von 10%-Schritten (0..10) auf eine Drehzahl in U/min
/// - optionale Rampenbildung zwischen alter und neuer Drehzahl
class SetpointManager {
public:
    /// \param minRpm          minimale Drehzahl (z.B. 500 U/min)
    /// \param maxRpm          maximale Drehzahl (z.B. 3000 U/min)
    /// \param rampRpmPerTick  maximale Drehzahländerung pro update()-Aufruf
    SetpointManager(int minRpm = 500,
        int maxRpm = 3000,
        int rampRpmPerTick = 100);

    /// Setzt den Sollwert in 10%-Schritten (0..10).
    /// 0 = Motor aus (0 U/min),
    /// 1..10 = 10..100% → 500..3000 U/min.
    void setCommandStep(int step10);

    /// Traceability-Matrix-kompatibler Alias.
    void setSpeedStep(int step10) { setCommandStep(step10); }

    /// Liefert den aktuell gesetzten Schritt (0..10).
    int commandStep() const { return cmdStep10_; }

    /// Zyklischer Aufruf aus der Main Control Loop.
    void update();

    /// Gerampte aktuelle Drehzahl (an den Motor weitergeben).
    int currentSetpointRpm() const { return currentRpm_; }

    /// Reine Ziel-Drehzahl gemäß aktuellem Schritt (ohne Rampe).
    int targetSetpointRpm() const { return targetRpm_; }

private:
    int minRpm_;
    int maxRpm_;
    int rampRpmPerTick_;

    int cmdStep10_;   // 0..10
    int targetRpm_;   // Sollwert entsprechend cmdStep10_
    int currentRpm_;  // gerampter Wert

    int clampStep(int step) const;
    int mapStepToRpm(int step) const;
};