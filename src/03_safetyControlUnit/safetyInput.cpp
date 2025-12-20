#include "safetyInput.hpp"

SafetyInput::SafetyInput():
state_(SafetyState::EstopActive) {
}

bool SafetyInput::isTripped() const {
    return state_ != SafetyState::Safe;
}
//Setter SafetyInput
void SafetyInput::setSafetyInput(SafetyState state) {
    state_ = state;
}
//Getter SafetyInput
SafetyState SafetyInput::getState() const {
    return state_;
}
