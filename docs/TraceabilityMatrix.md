# Traceability-Matrix (erweitert nach Sprint 1 + Testfälle)

| Requirement | Systemkomponente | Software-Design-Komponente | Schnittstellen / Methoden | Sprint 1 | Testfälle Sprint 1 | Umsetzungsstand nach Sprint 1 | Status |
|-------------|------------------|-----------------------------|---------------------------|----------|--------------------|-------------------------------|--------|
| **F1** | MCU – Main Control Unit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | `executeCycle()`, `setSpeedStep()`, `setDutyCycle()` | ✓ | TC-I2, TC-I3 | Basissteuerung implementiert (Grund-Sollwert + Motoransteuerung) | Erfüllt (Basis) |
| **F2** | SI – SafetyInput / MCU | `SafetyInput`, `MainControlUnit` | `readInputs()`, `getSafetyStatus()` | ✓ | TC-M1, TC-M2, TC-I1, TC-I3 | SafetyInput angebunden, Freigabelogik aktiv | Erfüllt (Basis) |
| **F3** | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `readCurrent()`, `checkOvercurrent()` | - | — | Sensorlogik vorbereitet, aber nicht umgesetzt | Offen |
| **F4** | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `isMaintenanceDue()` | - | — | Keine Laufzeitlogik implementiert | Offen |
| **F5** | DM – CsvLogger | `CsvLogger`, `FileDriver` | `logStatus()`, `rotateLogIfNeeded()` | - | — | Logging nicht begonnen | Offen |
| **NF1** | MCU – Main Control Unit | `MainControlUnit` | `executeCycle()`, `getLastCycleTimeMs()` | ✓ | TC-I3 | Reaktionszeit strukturell erreicht | Teilweise erfüllt |
| **NF2** | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `updateMonitoring()`, `readCurrent()` | - | — | Sampling nicht vorhanden | Offen |
| **NF3** | MCU / MA | `MainControlUnit`, `MotorActuator` | `updateControlLoop()`, `getMeasuredSpeed()` | ✓ | TC-I2 | Reglerschleife grundsätzlich angelegt | Teilweise erfüllt |
| **NF4** | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `getMaintenanceAdvice()` | - | — | Kein Laufzeit-Counter vorhanden | Offen |
| **NF5** | DM – CsvLogger | `CsvLogger`, `FileDriver` | `checkFileSize()`, `rotateLogIfNeeded()` | - | — | Kein Log-Rollover umgesetzt | Offen |
| **NF6** | Gesamtsystem / Compilerumgebung | gesamte Softwarearchitektur | `main()`, `init()` | ✓ | TC-I3 | C++/STM32 erfolgreich umgesetzt | Erfüllt |
| **NF7** | Hardware – Gesamtsystem | `MainControlUnit`, `MotorActuator`, `CurrentSensor` | `runSelfTest()`, `getStatus()` | - | — | Keine End-zu-End-Tests möglich | Offen |

---

### Markierte Requirements für Sprint 1

| Requirement | Kurzbeschreibung |
|--------------|------------------|
| **F1** | Sollwertsteuerung des Schneidemotors (10 %-Schritte) |
| **F2** | Safety-Not-Halt mit Reaktionszeit < 100 ms |
| **NF1** | Bedienreaktionszeit ≤ 200 ms validiert |
| **NF6** | C++-Implementierung auf STM32 lauffähig |

---

### Testfälle Sprint 1

| Test-ID | Kurzbeschreibung |
|---------|------------------|
| **TC-M1** | Prüfung negativer bzw. ungültiger Speed-Werte im SafetyInput |
| **TC-M2** | Prüfung des oberen Grenzwerts im SafetyInput |
| **TC-M3** | Rampenfunktion: Berechnung des nächsten Sollwertschritts |
| **TC-I1** | SafetyInput → SetpointManager: Validierung und Weitergabe |
| **TC-I2** | SetpointManager → MotorActuator: Übergabe des Sollwerts |
| **TC-I3** | MainControlUnit übernimmt gültigen Eingabewert korrekt |

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
