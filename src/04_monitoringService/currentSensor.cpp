#include "currentSensor.hpp"

CurrentSensor::CurrentSensor()
    : currentmA_(1200)   // Dummy: 1.2A Grundlast
{
}

int CurrentSensor::readCurrent() const {
    return currentmA_;
}


void CurrentSensor::setSimCurrentmA(int mA) {
    if (mA < 0) mA = 0;
    currentmA_ = mA;
}
