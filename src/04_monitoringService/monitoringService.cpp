#include "monitoringService.hpp"

MonitoringService::MonitoringService()
    : overCurrentLimitmA_(5000)   // Dummy: 5A Grenze
{
}

bool MonitoringService::checkOverCurrent(int currentmA) const {
    return currentmA > overCurrentLimitmA_;
}

bool MonitoringService::updateMonitoring(int currentmA) const {
    return checkOverCurrent(currentmA);
}

