#include "speedSensorSim.hpp"
#include "status.hpp"

SpeedSensorSim::SpeedSensorSim()
    : actualRpm_(0),
    maxRpm_(3000),
    accelRpmPerTick_(10),  // pro 100ms -> 3000 rpm in ~1s (anpassen)
    decelRpmPerTick_(20)   // Auslauf langsamer (anpassen)
{
}

void SpeedSensorSim::update(Status& st) {
    if (st.safetyState != SafetyState::Safe) {
        actualRpm_ = 0;
        st.speedActualRpm = 0;
        return;
    }

    int targetRpm = 0;
    if (st.runState == RunState::Run) {
        targetRpm = (maxRpm_ * st.speedSetpointPct) / 100;
    }

    if (actualRpm_ < targetRpm) {
        actualRpm_ += accelRpmPerTick_;
        if (actualRpm_ > targetRpm) actualRpm_ = targetRpm;
    }
    else if (actualRpm_ > targetRpm) {
        actualRpm_ -= decelRpmPerTick_;
        if (actualRpm_ < targetRpm) actualRpm_ = targetRpm;
    }

    st.speedActualRpm = actualRpm_;
}

int SpeedSensorSim::readActualRpm() const {
    return actualRpm_;
}

void SpeedSensorSim::forceStop(Status& st) {
    actualRpm_ = 0;
    st.speedActualRpm = 0;
}
