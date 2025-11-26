#include "safetyInput.hpp"

namespace {
    SafetyInput::Inputs makeInitialInputs()
    {
        SafetyInput::Inputs in{};
        in.estopNc = false;
        in.guardDoorNc = false;
        in.safetyReset = false;
        return in;
    }
}

SafetyInput::SafetyInput(std::uint32_t debounceTicks)
    : debounceTicks_{ debounceTicks }
    , stableCounter_{ 0U }
    , lastRawInputs_{ makeInitialInputs() }
    , stableInputs_{ makeInitialInputs() }
    , state_{ SafetyState::NotReady }
    , latchedFault_{ false }
{
}

bool SafetyInput::inputsEqual(const Inputs& a, const Inputs& b)
{
    return (a.estopNc == b.estopNc) &&
        (a.guardDoorNc == b.guardDoorNc) &&
        (a.safetyReset == b.safetyReset);
}

SafetyState SafetyInput::evaluate(const Inputs& in) const
{
    // Grundlogik:
    // - estopNc/guardDoorNc sind NC-Kontakte: false => Kreis geöffnet => unsicher
    // - safetyReset darf den Motor NICHT direkt freigeben, sondern nur
    //   aus einem zuvor sicheren Zustand wieder "armen".

    if (!in.estopNc) {
        return SafetyState::EstopActive;
    }

    if (!in.guardDoorNc) {
        return SafetyState::GuardOpen;
    }

    // Wenn wir hier sind, sind die Sicherheitskreise elektrisch OK:
    return SafetyState::Safe;
}

void SafetyInput::readInputs(const Inputs& inputs)
{
    // Entprellen / Stabilitätsprüfung
    if (inputsEqual(inputs, lastRawInputs_)) {
        if (stableCounter_ < debounceTicks_) {
            ++stableCounter_;
        }
    }
    else {
        stableCounter_ = 0U;
        lastRawInputs_ = inputs;
    }

    if (stableCounter_ >= debounceTicks_) {
        stableInputs_ = lastRawInputs_;

        // Nur wenn stabile Eingänge vorliegen, Zustand neu bewerten.
        SafetyState newState = evaluate(stableInputs_);

        // Einfache Fehlerlogik: einmal erkannter Fault bleibt gelatcht.
        if (latchedFault_) {
            state_ = SafetyState::Fault;
            return;
        }

        // Hier könnten Plausibilitätsprüfungen ergänzt werden.
        // Beispiel (nur als Platzhalter):
        // if (irgendwas_unlogisch) {
        //     latchedFault_ = true;
        //     state_ = SafetyState::Fault;
        //     return;
        // }

        state_ = newState;
    }
}

bool SafetyInput::isSafetyOk() const
{
    return (state_ == SafetyState::Safe) && !latchedFault_;
}
