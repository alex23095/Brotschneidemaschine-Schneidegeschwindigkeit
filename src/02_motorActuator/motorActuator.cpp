#include "motorActuator.hpp"

MotorActuator::MotorActuator()
    : dutyPct_(0), enabled_(false) {
}
void MotorActuator::enable() {
    enabled_ = true;
}
void MotorActuator::disable() {
    enabled_ = false;
    dutyPct_ = 0;
}
void MotorActuator::setDutyPercent(int pct) {
    if (!enabled_) {
        dutyPct_ = 0;
        return;
    }
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;

    dutyPct_ = pct;

}
int MotorActuator::readActualPercent() const {
    return dutyPct_;
}
void MotorActuator::stopEmergency() {
    enabled_ = false;
    dutyPct_ = 0;
}

