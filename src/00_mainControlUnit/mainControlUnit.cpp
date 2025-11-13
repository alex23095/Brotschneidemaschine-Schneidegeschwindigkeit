#include "mainControlUnit.hpp"

// Hier ggf. System-Timing kommentieren:
// tick() wird z.B. alle 10 ms aufgerufen
// → Reaktionszeit auf Safety-Änderungen < 200 ms (NF1 erfüllbar, wenn Periodendauer passend gewählt).

MainControlUnit& MainControlUnit::instance()
{
    static MainControlUnit instance_;
    return instance_;
}

MainControlUnit::MainControlUnit()
    : safetyInput_{5U}                    // 5 Zyklen Entprellung
    , setpointManager_{500, 3000, 100}    // 500..3000 U/min, Rampe 100 RPM / Tick
    , motorActuator_{500, 3000}           // gleicher Bereich wie SetpointManager
    , pendingSafetyInputs_{}
{
    // Initialwerte der Safety-Eingänge: alle Kreise geöffnet/unsicher.
    pendingSafetyInputs_.estopNc     = false;
    pendingSafetyInputs_.guardDoorNc = false;
    pendingSafetyInputs_.safetyReset = false;
}

void MainControlUnit::setUiSpeedCommandStep(int step10)
{
    // UI wählt 0..10 → SetpointManager erzeugt entsprechende Drehzahl
    setpointManager_.setCommandStep(step10);
}

void MainControlUnit::setSafetyInputs(const SafetyInput::Inputs& in)
{
    // Rohwerte zwischenspeichern, eigentliche Verarbeitung in tick()
    pendingSafetyInputs_ = in;
}

void MainControlUnit::tick()
{
    // 1) Safety-Eingänge entprellen / auswerten
    safetyInput_.update(pendingSafetyInputs_);
    const bool safetyOk = safetyInput_.isSafetyOk();

    // 2) Sollwert-Rampe aktualisieren (RPM)
    setpointManager_.update();
    const int rpmCmd = setpointManager_.currentSetpointRpm();

    // 3) Motor-Aktuator versorgen
    motorActuator_.setSafetyOk(safetyOk);
    motorActuator_.setCommandRpm(rpmCmd);
    motorActuator_.update();

    // 4) Ergebnis:
    //    - motorActuator_.isEnabled()
    //    - motorActuator_.dutyCyclePercent()
    //    werden von der Hardware-nahen Schicht (PWM) ausgelesen.
}
