#pragma once

class CurrentSensor {
public:
    CurrentSensor();

    // liefert aktuellen Strom in mA
    int readCurrent() const;

    // für Konsole/Test
    void setSimCurrentmA(int mA);

private:
    int currentmA_;   // Simulierter Messwert
};
