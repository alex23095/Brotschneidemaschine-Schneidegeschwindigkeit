# Traceability-Matrix (Sprint 2 – aktueller Stand)

| Requirement | Systemkomponente | Software-Design-Komponente | Sprint 2 | Status |
|------------|------------------|-----------------------------|----------|--------|
| **F1** – Drehzahlregelung (10%-Stufen) | MCU – MainControlUnit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | x (Sprint 1) | Implementiert |
| **F2** – Not-Halt / Safety-Zustand | SI – SafetyInput | `SafetyInput`, `MainControlUnit` | x (Sprint 1) | Implementiert |
| **F3** – Strommessung 500 ms | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | x | Implementiert in Sprint 2 |
| **F4** – Wartung nach 48 h Laufzeit | PM – PersistenceManager | `MaintenanceManager` | x | Implementiert in Sprint 2 |
| **F5** – CSV-Logging 1 Hz | PM – PersistenceManager | `CsvLogger`, `FileDriver` | - | vorgesehen für Sprint 3/4 |
| **NF1** – Basisfunktionsfähigkeit | SI – SafetyInput | `SafetyInput` | x (Sprint 1) | erfüllt |
| **NF2** – Reaktionszeit ≤ 200 ms | UI / MCU | `MainControlUnit`, `UserInterfaceService` | x | verbessert in Sprint 2 |
| **NF3** – 100-ms-Zyklus | MCU – MainControlUnit | `MainControlUnit` | x (Sprint 1) | stabil |
| **NF4** – Datei-Rollover 1 MB | PM – PersistenceManager | `FileDriver` | x | strukturell implementiert |
| **NF5** – vollständige Statusinfos | MCU – MainControlUnit | `Status` | - | folgt mit UI |
| **NF6** – 10%-Sollwertgenauigkeit | UI – UserInterfaceService | `SetpointManager` | x (Sprint 1) | erfüllt |
| **NF7** – Hardware-Temperaturbereich | MA / CS / SI | `MotorActuator`, `CurrentSensor`, `SafetyInput` | x | validiert in Sprint 2 |

---

## Markierte Requirements für Sprint 2

| Requirement | Kurzbeschreibung |
|------------|------------------|
| **F3** | Erfassung der Stromaufnahme über `CurrentSensor`, 500-ms-Takt, Vorbereitung MonitoringService |
| **F4** | Wartungslogik (48 h kumulierte Laufzeit), Berechnung Maintenance-Flag |
| **NF2** | Verbesserung der Reaktionszeit UI → MCU → Safety/Motor |
| **NF4** | Dateiverwaltung mit Rollover-Struktur im FileDriver |
| **NF7** | Validierung der Hardwarekomponenten im vorgesehenen Temperaturbereich |

---

## Traceability: Software-Design-Komponenten → Systemkomponenten (Sprint 2)

| Software-Design-Komponente | Zugehörige Systemkomponente | Beschreibung |
|-----------------------------|-----------------------------|--------------|
| `CurrentSensor`            | CS – Current-Sensor         | Liefert Strommesswerte im 500-ms-Rhythmus |
| `MonitoringService`        | CS – Monitoring             | Mittelwertbildung (Fenster 5), Vorstufe für Diagnose |
| `MaintenanceManager`       | PM – Maintenance / Persistence | Errechnet Laufzeit, setzt Maintenance-Flag nach 48 h |
| `FileDriver`               | PM – Persistence            | Dateioperationen (write/size), 1-MB-Rollover-Struktur |
| `CsvLogger`                | PM – Persistence            | Logging-Schnittstelle, wird in späterem Sprint aktiviert |
| `MainControlUnit`          | MCU – Control Logic         | Zentrale Steuerung, Übergabe Mess- & Zustandsdaten |
| `SafetyInput`              | SI – Safety                 | Safety-Zustand, Temperaturtoleranzen |
| `MotorActuator`            | MA – Hardware               | Duty-Ausgabe, Temperaturtoleranzen |

---

