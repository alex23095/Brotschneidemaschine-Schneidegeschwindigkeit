#pragma once
#include "status.hpp"

class SafetyInput {
public:
    SafetyInput();

    // true = Safety ausgelöst (Not-Halt aktiv)
    bool isTripped() const;

    // Setter SafetyInput
    void setSafetyInput(SafetyState state);
    // Getter SafetyInput
    SafetyState getState() const;
    
private:
    SafetyState state_;

};
