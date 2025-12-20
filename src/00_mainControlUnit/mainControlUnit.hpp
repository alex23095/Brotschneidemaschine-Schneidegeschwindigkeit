#pragma once
#include <cstdint>
#include "motorActuator.hpp"
#include "safetyInput.hpp"
#include "status.hpp"
#include "speedSensorSim.hpp"
#include "currentSensor.hpp"
#include "monitoringService.hpp"
#include "maintenanceManager.hpp"


class MainControlUnit {
public:
    //Konsturktor
    MainControlUnit(MotorActuator& motor, SafetyInput& safety,
        CurrentSensor& currentSensor, MonitoringService& monitoring);


    // UI-Befehle
    void setSpeedStep(int step10);   // 0..10
    void start();
    void stop();

    // Steuerzyklus (alle 100 ms)
    void tick();
    void executeCycle();
    void updateControlLoop();

    // Status (für UI)
    bool isRunning() const;
    int  speedStep() const;

    //Gesamtstatus
    const Status& getStatus() const;

    //Wartung Simulation
    std::uint64_t getRuntimeMs() const;
    void setMaintenanceLimitMs(std::uint64_t limitMs);

    //Selftest
    bool runSelfTest() const;

private:

    MotorActuator& motor_;
    SafetyInput& safety_;
    CurrentSensor& currentSensor_;
    MonitoringService& monitoring_;

    MaintenanceManager maintenance_;

    //Analgenstatus
    Status status_{};

    //Sensor Simulation
    SpeedSensorSim sensorSim_;

    bool running_;
    int  speedStep_; // 0..10

};
