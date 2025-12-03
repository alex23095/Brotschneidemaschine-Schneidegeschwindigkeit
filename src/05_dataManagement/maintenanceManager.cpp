#include "maintenanceManager.hpp"

MaintenanceManager::MaintenanceManager(std::uint64_t maintenanceIntervalMs)
    : maintenanceIntervalMs_{ maintenanceIntervalMs }
    , runtimeMs_{ 0ULL }
{
}

void MaintenanceManager::updateRuntimeMs(std::uint64_t deltaMs)
{
    runtimeMs_ += deltaMs;
}

std::string MaintenanceManager::getMaintenanceAdvice() const
{
    if (!isMaintenanceDue()) {
        return "Maintenance not required yet.";
    }

    const auto hours = runtimeMs_ / (1000ULL * 60ULL * 60ULL);
    return "Maintenance required. Runtime hours: " + std::to_string(hours);
}