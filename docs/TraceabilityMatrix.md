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

# Traceability-Matrix (erweitert nach Sprint 1)

| Requirement | Systemkomponente | Software-Design-Komponente | Schnittstellen / Methoden | Sprint 1 | Umsetzungsstand nach Sprint 1 | Status |
|-------------|------------------|-----------------------------|---------------------------|----------|-------------------------------|--------|
| **F1** | MCU – Main Control Unit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | `executeCycle()`, `setSpeedStep()`, `setDutyCycle()` | ✓ | Basissteuerung implementiert (Grund-Sollwert + Motoransteuerung) | Erfüllt (Basis) |
| **F2** | SI – SafetyInput / MCU | `SafetyInput`, `MainControlUnit` | `readInputs()`, `getSafetyStatus()` | ✓ | SafetyInput angebunden, Freigabelogik aktiv | Erfüllt (Basis) |
| **F3** | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `readCurrent()`, `checkOvercurrent()` | - | Sensorlogik vorbereitet, aber nicht umgesetzt | Offen |
| **F4** | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `isMaintenanceDue()` | - | Keine Laufzeitlogik implementiert | Offen |
| **F5** | DM – CsvLogger | `CsvLogger`, `FileDriver` | `logStatus()`, `rotateLogIfNeeded()` | - | Logging nicht begonnen | Offen |
| **NF1** | MCU – Main Control Unit | `MainControlUnit` | `executeCycle()`, `getLastCycleTimeMs()` | ✓ | Reaktionszeit strukturell erreicht | Teilweise erfüllt |
| **NF2** | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `updateMonitoring()`, `readCurrent()` | - | Sampling nicht vorhanden | Offen |
| **NF3** | MCU / MA | `MainControlUnit`, `MotorActuator` | `updateControlLoop()`, `getMeasuredSpeed()` | ✓ | Reglerschleife grundsätzlich angelegt | Teilweise erfüllt |
| **NF4** | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `getMaintenanceAdvice()` | - | Kein Laufzeit-Counter vorhanden | Offen |
| **NF5** | DM – CsvLogger | `CsvLogger`, `FileDriver` | `checkFileSize()`, `rotateLogIfNeeded()` | - | Kein Log-Rollover umgesetzt | Offen |
| **NF6** | Gesamtsystem / Compilerumgebung | gesamte Softwarearchitektur | `main()`, `init()` | ✓ | C++/STM32 erfolgreich umgesetzt | Erfüllt |
| **NF7** | Hardware – Gesamtsystem | `MainControlUnit`, `MotorActuator`, `CurrentSensor` | `runSelfTest()`, `getStatus()` | - | Keine End-zu-End-Tests möglich | Offen |

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
