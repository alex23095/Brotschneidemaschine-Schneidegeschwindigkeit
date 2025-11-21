# Traceability-Matrix (aktueller Stand)

| Requirement | Systemkomponente | Software-Design-Komponente | Sprint 1 | Status |
|------------|------------------|-----------------------------|----------|--------|
| **F1** – Drehzahlregelung (10%-Stufen) | MCU – MainControlUnit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | x | Implementiert |
| **F2** – Not-Halt ≤100 ms | SI – SafetyInput | `SafetyInput`, `MainControlUnit`, `MotorActuator` | x | Implementiert |
| **F3** – Strommessung 500 ms | CS – CurrentSensor | `CurrentSensor` | - | Geplant für Sprint 2 |
| **F4** – Wartung 48h | PM – PersistenceManager | `PersistenceManager` | - | Abhängig von F3 |
| **F5** – CSV-Logging 1 Hz | PM – PersistenceManager | `PersistenceManager` | - | Später |
| **NF1** – Sicherheit / grundlegende Funktionsfähigkeit | SI – SafetyInput | `SafetyInput` | x | Basisfunktion aktiv |
| **NF2** – Reaktionszeit UI ≤200 ms | UI – UserInterfaceService | `UserInterfaceService` | x | Getestet |
| **NF3** – Deterministik 100-ms-Zyklus | MCU – MainControlUnit | `MainControlUnit` | x | Validiert |
| **NF4** – Datei-Rollover 1 MB | PM – PersistenceManager | `PersistenceManager` | - | folgt in Sprint 2 |
| **NF5** – Statusinformationen vollständig | MCU – MainControlUnit | `MainControlUnit` | x | Teilweise erfüllt |
| **NF6** – Genauigkeit 10%-Sollwert | UI – UserInterfaceService | `UserInterfaceService` | x | getestet |
| **NF7** – Temperaturbereich Hardware | MA / CS / SI | `MotorActuator`, `CurrentSensor`, `SafetyInput` | x | Hardware erfüllt |

### Markierte Requirements für Sprint 1

| Requirement | Kurzbeschreibung |
|------------|------------------|
| **F1**     | Drehzahlvorgabe in 10%-Schritten über UI, Motor läuft entsprechend |
| **F2**     | Not-Halt / Sicherheitsabschaltung des Motors (≤ 100 ms über SI/MA) |
| **NF2**    | Reaktionszeit von Bedienbefehlen (UI → MCU → MA) ≤ 200 ms |
| **NF3**    | Deterministischer Steuerzyklus der MCU (100 ms) |
| **NF6**    | Genauigkeit der Sollwertvorgabe (10%-Stufen werden korrekt umgesetzt) |


### Traceability: Software-Design-Komponenten → Systemkomponenten

| Software-Design-Komponente | Zugehörige Systemkomponente | Beschreibung |
|-----------------------------|-----------------------------|--------------|
| `MainControlUnit`          | MCU – Main Control Unit     | Zentrale Steuerlogik, Zustandsautomat, Koordination aller Komponenten |
| `SetpointManager`          | MCU – Main Control Unit     | Aufbereitung der Sollwertstufen (10 %-Rastung), Rampenlogik |
| `UserInterfaceService`     | UI – User Interface         | Entgegennahme von Bedienbefehlen, Anzeige von Statusinformationen |
| `MotorActuator`            | MA – Motor-Aktuator         | Umsetzung Duty-Cycle, Motorsteuerung, Not-Halt-Ausführung |
| `SafetyInput`              | SI – Safety-Input           | Bereitstellung des Not-Halt-Signals (digital) |
| `CurrentSensor`            | CS – Current-Sensor         | Lieferung der Strommesswerte im 500-ms-Takt |
| `PersistenceManager`       | PM – Persistence Manager    | CSV-Logging (1 Hz), Rollover, Wartungsüberwachung |
