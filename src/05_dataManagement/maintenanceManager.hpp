#pragma once
#include <cstdint>

class MaintenanceManager {
public:
    MaintenanceManager();

    // Zeit hochzählen
    void updateRuntimeMS(std::uint32_t deltaMs, bool motorRunning);
    // Auswertung Zeitintervall überschritten
    bool isMaintenanceDue() const;
    
    //Wartungsempfehlung
    bool getMaintenanceAdvice() const;

    // für Simulation
    std::uint64_t getRuntimeMs() const;
    void setMaintenanceLimitMs(std::uint64_t limitMs);


private:
    std::uint64_t runtimeMs_;
    std::uint64_t limitMs_;
};
