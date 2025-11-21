#pragma once

#include <cstdint>

enum class SafetyState {
    NotReady,     // System noch nicht stabil gestartet
    Safe,         // Alle Bedingungen für sicheren Betrieb erfüllt
    EstopActive,  // Not-Halt betätigt
    GuardOpen,    // Schutztür offen (oder ähnlicher Schutzkreis)
    Fault         // Logischer Fehler / inkonsistenter Zustand
};

class SafetyInput {
public:
    struct Inputs {
        bool estopNc;       // true = Not-Halt-Kreis geschlossen (NC-Kontakt OK)
        bool guardDoorNc;   // true = Schutztür-Kreis geschlossen (Tür zu)
        bool safetyReset;   // true = Reset-Taster gedrückt (Momentanwert)
    };

    /// \param debounceTicks Anzahl aufeinanderfolgender identischer Messwerte,
    ///                      bevor ein Zustand übernommen wird.
    explicit SafetyInput(std::uint32_t debounceTicks = 5U);

    /// Zyklischer Aufruf aus der Main Loop (z.B. alle 10 ms).
    void update(const Inputs& inputs);

    /// true, wenn Motor freigegeben werden darf.
    bool isSafetyOk() const;

    /// Detaillierter Sicherheitszustand.
    SafetyState state() const { return state_; }

    /// Convenience-Abfragen
    bool isEstopActive() const { return state_ == SafetyState::EstopActive; }
    bool isGuardOpen()  const { return state_ == SafetyState::GuardOpen; }
    bool hasFault()     const { return state_ == SafetyState::Fault; }

private:
    std::uint32_t debounceTicks_;
    std::uint32_t stableCounter_;

    Inputs        lastRawInputs_;
    Inputs        stableInputs_;
    SafetyState   state_;
    bool          latchedFault_;

    static bool inputsEqual(const Inputs& a, const Inputs& b);
    SafetyState evaluate(const Inputs& in) const;
};
