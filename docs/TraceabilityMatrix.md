# Traceability-Matrix (farbig + „bestanden“)

| **Req-ID** | **Beschreibung** | **Systemkomponente** | **Zugehörige Klassen** | **Methoden / Schnittstellen** | **Sprint** | **Implementierung** | **Testfälle** |
|-----------|------------------|----------------------|-------------------------|-------------------------------|------------|----------------------|----------------|
| **F1** | Grundsteuerung Motor & Sollwertverarbeitung | MCU – Main Control Unit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | `executeCycle()`, `setSpeedStep()` | 1 | 🟢 Implementiert (Basis) | TC-I2 🟢 (bestanden), TC-I3 🟢 (bestanden) |
| **F2** | Sicherheitsfreigabe & Eingangserfassung | SI – SafetyInput / MCU | `SafetyInput`, `MainControlUnit` | `readInputs()`, `getSafetyStatus()` | 1 | 🟢 Implementiert (Basis) | TC-M1 🟢 (bestanden), TC-M2 🟢 (bestanden), TC-I1 🟢 (bestanden), TC-I3 🟢 (bestanden) |
| **F3** | Strommessung & Überstromdetektion | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `readCurrent()`, `checkOvercurrent()` | – | 🔴 Nicht implementiert | — |
| **F4** | Wartungslogik (Betriebszeit, Hinweis) | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `isMaintenanceDue()` | – | 🔴 Nicht implementiert | — |
| **F5** | Logging von Status & Messdaten | DM – CsvLogger | `CsvLogger`, `FileDriver` | `logStatus()`, `rotateLogIfNeeded()` | – | 🔴 Nicht implementiert | — |
| **NF1** | Reaktionszeit der Zyklussteuerung | MCU – Main Control Unit | `MainControlUnit` | `executeCycle()`, `getLastCycleTimeMs()` | 1 | 🟡 Teilweise implementiert | TC-I3 🟢 (bestanden) |
| **NF2** | Messintervall Stromsensor | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `updateMonitoring()`, `readCurrent()` | – | 🔴 Nicht implementiert | — |
| **NF3** | Stabilität / Regelqualität Motorsteuerung | MCU / MA | `MainControlUnit`, `MotorActuator` | `updateControlLoop()`, `getMeasuredSpeed()` | 1 | 🟡 Teilweise implementiert | TC-I2 🟢 (bestanden) |
| **NF4** | Wartungszeit-Zähler zuverlässig führen | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `getMaintenanceAdvice()` | – | 🔴 Nicht implementiert | — |
| **NF5** | Dateigröße & Log-Rollover | DM – CsvLogger | `CsvLogger`, `FileDriver` | `checkFileSize()`, `rotateLogIfNeeded()` | – | 🔴 Nicht implementiert | — |
| **NF6** | Kompilierbarkeit & Portabilität | Gesamtsystem / Build | gesamte Architektur | `main()`, `init()` | 1 | 🟢 Implementiert | TC-I3 🟢 (bestanden) |
| **NF7** | Hardware-Selbsttest beim Start | Hardware – Gesamtsystem | `MainControlUnit`, `MotorActuator`, `CurrentSensor` | `runSelfTest()`, `getStatus()` | – | 🔴 Nicht implementiert | — |


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
