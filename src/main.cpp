#include "mainControlUnit.hpp"

void mainLoopTick()
{
    auto& mcu = MainControlUnit::instance();

    // Beispiel: UI-Lesevorgang
    int uiStep10 = /* vom UI lesen, 0..10 */;
    mcu.setUiSpeedCommandStep(uiStep10);

    // Beispiel: Safety-GPIO lesen
    SafetyInput::Inputs sin{};
    sin.estopNc     = /* GPIO lesen */;
    sin.guardDoorNc = /* GPIO lesen */;
    sin.safetyReset = /* GPIO lesen */;
    mcu.setSafetyInputs(sin);

    // Zentrale Steuerung
    mcu.tick();

    // PWM ansteuern
    std::uint8_t duty = mcu.dutyCyclePercent();
    bool enable      = mcu.isMotorEnabled();
    // → Duty + Enable in HAL/PWM-Treiber schreiben
}
