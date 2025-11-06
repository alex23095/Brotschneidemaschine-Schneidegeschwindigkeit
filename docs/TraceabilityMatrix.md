# Traceability-Matrix (aktuell – Brotschneidemaschine)

| Requirement | Systemkomponente | Software-Design-Komponente | Sprint 1 | Status |
|--------------|------------------|-----------------------------|-----------|--------|
| **F1** | MCU – Main Control Unit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | x | Implementiert |
| **F2** | UI – UserInterfaceService | `UserInterfaceService` | x | Implementiert |
| **F3** | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | - | Geplant für Sprint 2 |
| **F4** | MNT – MaintenanceManager | `MaintenanceManager` | - | Abhängig von F3 |
| **F5** | DM – CsvLogger | `CsvLogger`, `FileDriver` | - | Später |
| **NF1** | MCU – Main Control Unit | `MainControlUnit` | x | Plattform STM32 / C++ |
| **NF2** | UI – UserInterfaceService | `UserInterfaceService` | x | Reaktionszeit getestet (≤200 ms) |
| **NF3** | MCU – Main Control Unit | `MainControlUnit` | x | Zykluszeit 100 ms validiert |
| **NF4** | DM – CsvLogger | `CsvLogger` | - | Logging 1 Hz in Arbeit |
| **NF5** | MCU – Main Control Unit | `Status` | - | Zustandstransparenz folgt in Sprint 2 |

---

### Markierte Requirements für Sprint 1

| Requirement | Kurzbeschreibung |
|--------------|------------------|
| **F1** | Schneidemotor-Steuerung über Sollwert in 10 %-Stufen |
| **F2** | Bedienung und Statusanzeige über UserInterface |
| **NF1** | Implementierung in C++ auf STM32 (funktionsfähig) |
| **NF2** | Reaktionszeit < 200 ms getestet |
| **NF3** | Steuerungszyklus 100 ms validiert |

---

### Ergänzung: Traceability SW-Design → Architekturkomponenten

| Software-Design-Komponente | Zugehörige Systemkomponente | Beschreibung |
|-----------------------------|-----------------------------|---------------|
| `MainControlUnit` | MCU – Steuerungslogik | Hauptsteuerung, Koordination aller Teilfunktionen |
| `SetpointManager` | MCU – Steuerungslogik | Umsetzung Sollwert (10 %-Schritte), Rampensteuerung |
| `UserInterfaceService` | UI – User Interface | Eingabe (Start/Stop/Soll%), Anzeige Status & Wartung |
| `MotorActuator` | MA – Motorsteuerung | Umsetzung der Soll-Geschwindigkeit, Not-Halt |
| `CurrentSensor` | CS – Strommessung | Erfassung Stromaufnahme (500 ms Intervall) |
| `MonitoringService` | MON – Monitoring | Gleitmittelwertbildung (5 Messpunkte) |
| `MaintenanceManager` | MNT – Wartungsmanagement | Laufzeitzählung, Wartungshinweis nach 48 h |
| `CsvLogger` | DM – Datenmanagement | Protokollierung 1 Hz, Dateigrößenlimit 1 MB |
| `FileDriver` | DM – Datenmanagement | Dateizugriff & Rollover-Verwaltung |
| `SafetyInput` | SI – Safety | Erfassung Not-Halt (≤100 ms), Übergabe an MCU |
