#include "mainControlUnit.hpp"

void init()
{
    // Platzhalter für spätere Initialisierungsschritte
    (void)MainControlUnit::instance();
}

// dein vorhandener Tick
int mainLoopTick()
{
    auto& mcu = MainControlUnit::instance();

    // zentrale Steuerung
    mcu.executeCycle();

    // PWM ansteuern
    std::uint8_t duty = mcu.dutyCyclePercent();
    bool enable = mcu.isMotorEnabled();

    // -> Duty + Enable in HAL/PWM-Treiber schreiben

    return 0;
}

// echte Programmeinstiegstelle
int main()
{
    init();
    while (true)
    {
        mainLoopTick();
        // optional: kleine Pause oder Timerabhängigkeit
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
