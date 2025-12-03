#pragma once

#include <cstdint>
#include <string>

/// MaintenanceManager
/// - zählt kumulierte Betriebszeit in Millisekunden
/// - meldet Wartungsbedarf nach Erreichen eines Schwellwertes
class MaintenanceManager {
public:
    explicit MaintenanceManager(std::uint64_t maintenanceIntervalMs = 172800000ULL); // 48 h

    /// Laufzeit um deltaMs erhöhen.
    void updateRuntimeMs(std::uint64_t deltaMs);

    /// true, wenn die kumulierte Laufzeit den Schwellwert überschritten hat.
    bool isMaintenanceDue() const { return runtimeMs_ >= maintenanceIntervalMs_; }

    /// Liefert einen kurzen Hinweistext für UI/Logger.
    std::string getMaintenanceAdvice() const;

    /// Zugriff auf kumulierte Laufzeit (z. B. für Tests oder Anzeige).
    std::uint64_t runtimeMs() const { return runtimeMs_; }

private:
    std::uint64_t maintenanceIntervalMs_;
    std::uint64_t runtimeMs_;
};