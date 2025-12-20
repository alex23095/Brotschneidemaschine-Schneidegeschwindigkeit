#pragma once
#include <cstdint>

enum class RunState {
    Stop,
    Run
};

enum class SafetyState {
    Safe,
    EstopActive,
    GuardOpen
};

struct Status {
    //Motor Konfiguration
    int speedSetpointPct = 0;     // 0..100 (aus speedStep)
    int speedActualPct = 0;       // 0..100 (aus duty / später RPM)
    RunState runState = RunState::Stop;
    SafetyState safetyState = SafetyState::EstopActive;
    
    // Messwerte
    int speedActualRpm = 0;     // ISTDREHZAHL (Sensor/Simulation)

    //Betriebsdauer
    std::uint64_t runtimeMs = 0;

    //Wartungsanforderung
    bool maintenanceDue = false;

    //Stromwert
    int filteredCurrentmA = 0;    
};
