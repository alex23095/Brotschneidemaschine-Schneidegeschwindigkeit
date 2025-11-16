# Traceability-Matrix 

| Requirement | Systemkomponente | Software-Design-Komponente | Sprint 1 | Status |
|--------------|------------------|-----------------------------|-----------|--------|
| **F1** | MCU – Main Control Unit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | x | Implementiert |
| **F2** | SI – SafetyInput / MCU | `SafetyInput`, `MainControlUnit` | x | Implementiert |
| **F3** | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | - | Geplant für Sprint 2 |
| **F4** | MNT – MaintenanceManager | `MaintenanceManager` | - | Abhängig von F3 |
| **F5** | DM – CsvLogger | `CsvLogger`, `FileDriver` | - | Später |
| **NF1** | MCU – Main Control Unit | `MainControlUnit` | x | Reaktionszeit ≤ 200 ms validiert |
| **NF2** | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | - | Zyklisches Sampling 500 ms |
| **NF3** | MCU / MA | `MainControlUnit`, `MotorActuator` | - | Soll-/Ist-Abweichung ±5 % |
| **NF4** | MNT – MaintenanceManager | `MaintenanceManager` | - | Wartungshinweis nach 48 h Laufzeit |
| **NF5** | DM – CsvLogger | `CsvLogger`, `FileDriver` | - | Rollover bei 1 MB getestet |
| **NF6** | MCU / Compilerumgebung | gesamte Softwarearchitektur | x | Implementierung in C++ (STM32) |
| **NF7** | Hardware – Gesamtsystem | `MainControlUnit`, `MotorActuator`, `CurrentSensor` | - | Funktion geprüft bei 0–45 °C Umgebung |

# Traceability-Matrix

| Requirement ID | Requirement Beschreibung | Systemkomponente | Zugehörige Klassen | Schnittstellen / Methoden | Bearbeitung in Sprint |
|----------------|--------------------------|------------------|--------------------|---------------------------|-----------------------|
| F1 | Schneidemotor-Drehzahlregelung in Stufen, Start/Stop, Enable | MCU – Main Control Unit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | `executeCycle()`, `setSpeedStep()`, `setDutyCycle()` | 1 |
| F2 | Auswertung Sicherheitseingänge (Not-Halt, Haube, Freigabe) | SI – SafetyInput / MCU | `SafetyInput`, `MainControlUnit` | `readInputs()`, `getSafetyStatus()` | 1 |
| F3 | Motorstrommessung, Überstrom-Erkennung | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `readCurrent()`, `checkOvercurrent()` | - |
| F4 | Wartungslogik basierend auf kumulierter Laufzeit | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `isMaintenanceDue()` | - |
| F5 | CSV-Logging von Betriebs- und Wartungsdaten | DM – CsvLogger | `CsvLogger`, `FileDriver` | `logStatus()`, `rotateLogIfNeeded()` | - |
| NF1 | Reaktionszeit der Steuerung ≤ 200 ms | MCU – Main Control Unit | `MainControlUnit` | `executeCycle()`, `getLastCycleTimeMs()` | 1 |
| NF2 | Zyklisches Strom-Sampling alle 500 ms | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `updateMonitoring()`, `readCurrent()` | - |
| NF3 | Regelgüte: Soll-/Ist-Abweichung ±5 % | MCU / MA | `MainControlUnit`, `MotorActuator` | `updateControlLoop()`, `getMeasuredSpeed()` | 1 |
| NF4 | Wartungshinweis nach 48 h kumulierter Laufzeit | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `getMaintenanceAdvice()` | - |
| NF5 | Log-Rollover bei Dateigröße ≥ 1 MB | DM – CsvLogger | `CsvLogger`, `FileDriver` | `checkFileSize()`, `rotateLogIfNeeded()` | - |
| NF6 | Implementierung in C++ auf STM32-Plattform | MCU / Compilerumgebung | gesamte Softwarearchitektur | `main()`, `init()` | 1 |
| NF7 | Funktionsfähigkeit 0–45 °C Umgebung | Hardware – Gesamtsystem | `MainControlUnit`, `MotorActuator`, `CurrentSensor` | `runSelfTest()`, `getStatus()` | - |


---

### Markierte Requirements für Sprint 1

| Requirement | Kurzbeschreibung |
|--------------|------------------|
| **F1** | Sollwertsteuerung des Schneidemotors (10 %-Schritte) |
| **F2** | Safety-Not-Halt mit Reaktionszeit < 100 ms |
| **NF1** | Bedienreaktionszeit ≤ 200 ms validiert |
| **NF6** | C++-Implementierung auf STM32 lauffähig |

---

### Ergänzung: Traceability SW-Design → Architekturkomponenten

| Software-Design-Komponente | Zugehörige Systemkomponente | Beschreibung |
|-----------------------------|-----------------------------|---------------|
| `MainControlUnit` | MCU – Steuerungslogik | Koordination aller Teilfunktionen, Zyklus 100 ms |
| `SetpointManager` | MCU – Steuerungslogik | Umsetzung Sollwert (10 %-Schritte), Rampenfunktion |
| `UserInterfaceService` | UI – Bedienung | Eingabe (Start/Stop/Soll%), Anzeige von Status & Wartung |
| `MotorActuator` | MA – Motorsteuerung | Ansteuerung PWM-Duty, Not-Halt-Reaktion |
| `SafetyInput` | SI – Sicherheit | Erfassung Not-Halt (digital, ≤ 100 ms) |
| `CurrentSensor` | CS – Sensorik | Messung Stromaufnahme (500 ms-Intervall) |
| `MonitoringService` | MON – Monitoring | Gleitmittelwert über 5 Messpunkte |
| `MaintenanceManager` | MNT – Wartung | Laufzeitzählung, Wartung nach 48 h |
| `CsvLogger` | DM – Datenmanagement | Protokollierung (1 Hz), Dateigrößenlimit 1 MB |
| `FileDriver` | DM – Datenmanagement | Dateizugriff, Rollover-Mechanismus |
