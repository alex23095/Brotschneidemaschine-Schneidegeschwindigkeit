#pragma once


class MotorActuator {

    public:
        MotorActuator();

        void enable();
        void disable();

        void setDutyPercent(int pct);
        void stopEmergency();
        
        // echte Istwerte (vom Sensor / Test / Simulation)
        void setActualRpm(int rpm);   // für später: Sensor-Update
        int readActualPercent() const;

    private:
        int  dutyPct_;     // 0..100
        bool enabled_;
   

 };
