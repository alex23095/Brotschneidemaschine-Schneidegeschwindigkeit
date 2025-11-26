#include "mainControlUnit.hpp"

#include <algorithm>
#include <chrono>

// Hier ggf. System-Timing kommentieren:
// tick() wird z.B. alle 10 ms aufgerufen
// → Reaktionszeit auf Safety-Änderungen < 200 ms (NF1 erfüllbar, wenn Periodendauer passend gewählt).

MainControlUnit& MainControlUnit::instance()
{
    static MainControlUnit instance_;
    return instance_;
}

MainControlUnit::MainControlUnit()
    : safetyInput_{ 5U }                    // 5 Zyklen Entprellung
    , setpointManager_{ 500, 3000, 100 }    // 500..3000 U/min, Rampe 100 RPM / Tick
    , motorActuator_{ 500, 3000 }           // gleicher Bereich wie SetpointManager
    , pendingSafetyInputs_{}
    , lastCycleTimeMs_{ 0 }
    , selfTestPassed_{ false }
{
    // Initialwerte der Safety-Eingänge: alle Kreise geöffnet/unsicher.
    pendingSafetyInputs_.estopNc = false;
    pendingSafetyInputs_.guardDoorNc = false;
    pendingSafetyInputs_.safetyReset = false;
}

void MainControlUnit::setSpeedStep(int step10)
{
    // UI wählt 0..10 → SetpointManager erzeugt entsprechende Drehzahl
    setpointManager_.setSpeedStep(step10);
}

void MainControlUnit::setDutyCycle(std::uint8_t dutyPercent)
{
    // Duty-Cycle 0..100 % an Aktuator weiterreichen.
    motorActuator_.setManualDutyCycle(std::min<std::uint8_t>(100, dutyPercent));
}

void MainControlUnit::readInputs(const SafetyInput::Inputs& in)
{
    // Rohwerte zwischenspeichern, eigentliche Verarbeitung in tick()
    pendingSafetyInputs_ = in;
}

void MainControlUnit::tick()
{
    // 1) Safety-Eingänge entprellen / auswerten
    safetyInput_.readInputs(pendingSafetyInputs_);
    const bool safetyOk = safetyInput_.isSafetyOk();

    // 2) Sollwert-Rampe aktualisieren (RPM)
    setpointManager_.update();
    const int rpmCmd = setpointManager_.currentSetpointRpm();

    // 3) Motor-Aktuator versorgen
    motorActuator_.setSafetyOk(safetyOk);
    motorActuator_.setCommandRpm(rpmCmd);
    motorActuator_.updateControlLoop();

    // 4) Ergebnis:
    //    - motorActuator_.isEnabled()
    //    - motorActuator_.dutyCyclePercent()
    //    werden von der Hardware-nahen Schicht (PWM) ausgelesen.
}

void MainControlUnit::executeCycle()
{
    const auto start = std::chrono::steady_clock::now();
    tick();
    const auto end = std::chrono::steady_clock::now();
    lastCycleTimeMs_ = static_cast<std::uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
}

bool MainControlUnit::runSelfTest()
{
    // Einfache Plausibilitätsprüfungen: Grenzwerte und Safety-Eingänge.
    const bool rpmRangeOk = setpointManager_.targetSetpointRpm() >= 500 &&
        setpointManager_.targetSetpointRpm() <= 3000;
    const bool dutyOk = motorActuator_.dutyCyclePercent() <= 100;

    // Safety-Reset muss explizit angefordert werden, bevor gestartet werden kann.
    const bool safetyInputsDefined = pendingSafetyInputs_.estopNc == false &&
        pendingSafetyInputs_.guardDoorNc == false;

    selfTestPassed_ = rpmRangeOk && dutyOk && safetyInputsDefined;
    return selfTestPassed_;
}
