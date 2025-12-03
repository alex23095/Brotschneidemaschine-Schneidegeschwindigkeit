# Traceability-Matrix 

| Req-ID | Beschreibung | Systemkomponente | Zugehörige Klassen | Methoden / Schnittstellen | **Sprint 1 – NR** | **Sprint 1 – Implementierung** | **Sprint 1 – Testfälle** | **Sprint 2 – NR** | **Sprint 2 – Implementierung** | **Sprint 2 – Testfälle** |
|--------|--------------|------------------|---------------------|----------------------------|-------------------|-------------------------------|---------------------------|-------------------|-------------------------------|---------------------------|
| **F1** | Grundsteuerung Motor & Sollwertverarbeitung | MCU – Main Control Unit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | `executeCycle()`, `setSpeedStep()`, `setDutyCycle()` | 1 | `executeCycle()`, `setSpeedStep()`, `setDutyCycle()` | 🟢 TC-I2, 🟢 TC-I3 | – | — | — |
| **F2** | Sicherheitsfreigabe & Eingangserfassung | SI – SafetyInput / MCU | `SafetyInput`, `MainControlUnit` | `readInputs()`, `getSafetyStatus()` | 1 | `readInputs()`, `getSafetyStatus()` | 🟢 TC-M1, 🟢 TC-M2, 🟢 TC-I1, 🟢 TC-I3 | – | — | — |
| **F3** | Strommessung & Überstromdetektion | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `readCurrent()`, `checkOvercurrent()` | – | — | 🔴 — | 2 | `readCurrent()`, `checkOvercurrent()` | 🔵 TC-C1, 🔵 TC-C2 |
| **F4** | Wartungslogik (Betriebszeit, Hinweis) | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `isMaintenanceDue()` | – | — | 🔴 — | 2 | `updateRuntimeMs()`, `isMaintenanceDue()` | 🔵 TC-MNT1, 🔵 TC-MNT2 |
| **F5** | Logging von Status & Messdaten | DM – CsvLogger | `CsvLogger`, `FileDriver` | `logStatus()`, `rotateLogIfNeeded()` | – | — | 🔴 — | – | — | — |
| **NF1** | Reaktionszeit der Zyklussteuerung | MCU – Main Control Unit | `MainControlUnit` | `executeCycle()`, `getLastCycleTimeMs()` | 1 | `executeCycle()` | 🟢 TC-I3 | – | — | — |
| **NF2** | Messintervall Stromsensor | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | `updateMonitoring()`, `readCurrent()` | – | — | 🔴 — | 2 | `updateMonitoring()`, `readCurrent()` | 🔵 TC-C1 |
| **NF3** | Stabilität / Regelqualität Motorsteuerung | MCU / MA | `MainControlUnit`, `MotorActuator` | `updateControlLoop()`, `getMeasuredSpeed()` | 1 | `updateControlLoop()` | 🟢 TC-I2 | – | — | — |
| **NF4** | Wartungszeit-Zähler zuverlässig führen | MNT – MaintenanceManager | `MaintenanceManager` | `updateRuntimeMs()`, `getMaintenanceAdvice()` | – | — | 🔴 — | 2 | `updateRuntimeMs()`, `getMaintenanceAdvice()` | 🔵 TC-MNT1 |
| **NF5** | Dateigröße & Log-Rollover | DM – CsvLogger | `CsvLogger`, `FileDriver` | `checkFileSize()`, `rotateLogIfNeeded()` | – | — | 🔴 — | – | — | — |
| **NF6** | Kompilierbarkeit & Portabilität | Gesamtsystem / Build | gesamte Architektur | `main()`, `init()` | 1 | `main()` | 🟢 TC-I3 | – | — | — |
| **NF7** | Hardware-Selbsttest beim Start | Hardware – Gesamtsystem | `MainControlUnit`, `MotorActuator`, `CurrentSensor` | `runSelfTest()`, `getStatus()` | – | — | 🔴 — | 2 | `runSelfTest()`, `getStatus()` | 🔵 TC-SELF1, 🔵 TC-SELF2 |

---

### Markierte Requirements für Sprint 1

| Requirement | Kurzbeschreibung |
|--------------|------------------|
| **F1** | Sollwertsteuerung des Schneidemotors (10 %-Schritte) |
| **F2** | Safety-Not-Halt mit Reaktionszeit < 100 ms |
| **NF1** | Bedienreaktionszeit ≤ 200 ms validiert |
| **NF6** | C++-Implementierung auf STM32 lauffähig |


### Markierte Requirements für Sprint 2

| Requirement | Kurzbeschreibung |
|--------------|------------------|
| **F3** | Strommessung & automatische Überstromabschaltung |
| **F4** | Wartungslogik (Betriebszeit-Zähler & Wartungshinweis) |
| **NF2** | Zyklisches Messintervall für Stromsensor (500 ms) |
| **NF4** | Zuverlässige Führung des Wartungs-Zeitkontos |
| **NF7** | Hardware-Selbsttest beim Systemstart |

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


### Testfälle Sprint 2

| Test-ID | Kurzbeschreibung |
|---------|------------------|
| **TC-C1** | Stromsensor liefert gültige Messwerte im geforderten Intervall |
| **TC-C2** | Überstrom wird erkannt → Motorabschaltung erfolgt |
| **TC-MNT1** | Wartungszeit wird korrekt hochgezählt (Simulationslauf) |
| **TC-MNT2** | Wartungshinweis erscheint nach Überschreiten der Grenze |
| **TC-SELF1** | Selbsttest erkennt defekte Sensoren korrekt |
| **TC-SELF2** | Selbsttest meldet „OK“ bei intakter Hardware |

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
