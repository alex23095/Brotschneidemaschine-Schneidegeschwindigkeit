#include "maintenanceManager.hpp"

MaintenanceManager::MaintenanceManager()
    : runtimeMs_(0)
    , limitMs_(48ull * 60ull * 60ull * 1000ull) // 48h in ms
{
}

void MaintenanceManager::updateRuntimeMS(std::uint32_t deltaMs, bool motorRunning) {
    if (!motorRunning) return;
    runtimeMs_ += static_cast<std::uint64_t>(deltaMs);
}

bool MaintenanceManager::isMaintenanceDue() const {
    return runtimeMs_ >= limitMs_;
}

bool MaintenanceManager::getMaintenanceAdvice() const {
    return isMaintenanceDue();
}

std::uint64_t MaintenanceManager::getRuntimeMs() const {
    return runtimeMs_;
}

void MaintenanceManager::setMaintenanceLimitMs(std::uint64_t limitMs) {
    limitMs_ = limitMs;
}
