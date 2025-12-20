#pragma once

struct Status;

class SpeedSensorSim {
public:
    SpeedSensorSim();

    // wird zyklisch (100 ms) aufgerufen
    void update(Status& st);

    int readActualRpm() const;
    
    //Stoppen bei Overcurrent
    void forceStop(Status& st);

private:
    int actualRpm_;      // Istwert in rpm
    int maxRpm_;         // z.B. 3000
    int accelRpmPerTick_; // Hochlauf pro Tick
    int decelRpmPerTick_; // Auslauf pro Tick
};
