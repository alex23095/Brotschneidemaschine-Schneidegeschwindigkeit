#pragma once

class MonitoringService {
public:
    MonitoringService();

    // true = Überstrom erkannt
    bool checkOverCurrent(int currentmA) const;
    
    //Bündelung der Monitoring-Services
    bool updateMonitoring(int currentmA) const;


private:
    int overCurrentLimitmA_;   // z.B. 5000 mA
};
