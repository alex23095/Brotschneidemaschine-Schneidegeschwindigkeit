#include "mainControlUnit.hpp"
#include "motorActuator.hpp"
#include "safetyInput.hpp"
#include "speedSensorSim.hpp"

MainControlUnit::MainControlUnit(MotorActuator& motor, SafetyInput& safety,
    CurrentSensor& currentSensor, MonitoringService& monitoring)
    : motor_(motor)
    , safety_(safety)
    , currentSensor_(currentSensor)
    , monitoring_(monitoring)
    , running_(false)
    , speedStep_(0)
{
}


void MainControlUnit::setSpeedStep(int step10) {
    if (step10 < 0) step10 = 0;
    if (step10 > 10) step10 = 10;
    speedStep_ = step10;
}

void MainControlUnit::start() {

    // Start nur erlaubt, wenn Sollwert = 0
    if (speedStep_ != 0) {
        return;   // Start verweigert
    }

    // zusätzlich: Safety muss frei sein
    if (safety_.isTripped()) {
        return;
    }
    running_ = true;
}

void MainControlUnit::stop() {
    running_ = false;
    motor_.setDutyPercent(0);
    speedStep_ = 0;

}

void MainControlUnit::tick() {
    // 1) Safety
    status_.safetyState = safety_.getState();

    if (status_.safetyState != SafetyState::Safe) {
        motor_.stopEmergency();
        speedStep_ = 0;
        running_ = false;
    }

    // 2) Run-State
    status_.runState = running_
        ? RunState::Run
        : RunState::Stop;

    // 3) Sollwert aus UI
    status_.speedSetpointPct = speedStep_ * 10;

    // 4) Motor ansteuern
    if (status_.runState == RunState::Run &&
        status_.safetyState == SafetyState::Safe) {

        motor_.enable();
        motor_.setDutyPercent(status_.speedSetpointPct);
    }
    else {
        motor_.disable();
    }

    // 5) Stromaufnahme messen --> OverCurrent Motor hart aus wie bei ausgelöster Safety
    const int currentmA = currentSensor_.readCurrent();
    status_.filteredCurrentmA = currentmA;
    if (monitoring_.updateMonitoring(currentmA)) {
        motor_.stopEmergency();
        speedStep_ = 0;
        running_ = false;
        //ablöschen des aktuellen Zustandes
        status_.runState = RunState::Stop;
        status_.speedSetpointPct = 0;
        sensorSim_.forceStop(status_);
    }

    //6 Betriebstimer und Auswertung ob Wartung fällig ist
    maintenance_.updateRuntimeMS(100, running_ && status_.safetyState == SafetyState::Safe);
    status_.maintenanceDue = maintenance_.isMaintenanceDue();
    status_.runtimeMs = maintenance_.getRuntimeMs();


    // 6) Sensor-Simulation (schreibt Istwert!)
    sensorSim_.update(status_);
}

void MainControlUnit::executeCycle() {
    tick();
}

void MainControlUnit::updateControlLoop() {
    tick();
}

bool MainControlUnit::isRunning() const {
    return running_;
}

int MainControlUnit::speedStep() const {
    return speedStep_;
}

const Status& MainControlUnit::getStatus() const {
    //Status st{};
    //st.speedSetpointPct = speedStep_ * 10;
    //st.speedActualPct = motor_.readActualPercent();
    //st.runState = running_ ? RunState::Run : RunState::Stop;
    //st.safetyState = safety_.isTripped() ? SafetyState::Tripped : SafetyState::Safe;
    //return st;

    return status_;
}


std::uint64_t MainControlUnit::getRuntimeMs() const {
    return maintenance_.getRuntimeMs();
}

void MainControlUnit::setMaintenanceLimitMs(std::uint64_t limitMs) {
    maintenance_.setMaintenanceLimitMs(limitMs);
}

bool MainControlUnit::runSelfTest() const{
    // 1) Safety muss frei sein
    if (safety_.getState() != SafetyState::Safe) {
        return false;
    }

    // 2) CurrentSensor plausibel?
    const int currentmA = currentSensor_.readCurrent();
    if (currentmA < 0) {
        return false;
    }

    // 3) Motor-Grundfunktion testen (minimal)
    motor_.enable();
    motor_.setDutyPercent(10);

    motor_.disable();
    if (motor_.readActualPercent() != 0) {
        motor_.stopEmergency();
        return false;
    }

    // sicherer Endzustand
    motor_.stopEmergency();
    return true;
}
