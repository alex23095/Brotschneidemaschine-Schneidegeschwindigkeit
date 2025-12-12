# Traceability-Matrix (Sprint 3 – aktueller Stand)

| Requirement | Systemkomponente | Software-Design-Komponente | Sprint 3 | Status |
|------------|------------------|-----------------------------|----------|--------|
| **F1** – Drehzahlregelung (10%-Stufen) | MCU – MainControlUnit | `MainControlUnit`, `SetpointManager`, `MotorActuator` | x (Sprint 1) | Implementiert |
| **F2** – Not-Halt / Safety-Zustand | SI – SafetyInput | `SafetyInput`, `MainControlUnit` | x (Sprint 1) | Implementiert |
| **F3** – Strommessung 500 ms | CS – CurrentSensor | `CurrentSensor`, `MonitoringService` | x (Sprint 2) | Implementiert |
| **F4** – Wartung nach 48 h Laufzeit | PM – MaintenanceManager | `MaintenanceManager` | x (Sprint 2) | Implementiert |
| **F5** – CSV-Logging von Status & Messdaten | PM – PersistenceManager | `CsvLogger`, `FileDriver` | x | **Implementiert in Sprint 3** |
| **NF1** – Basisfunktionsfähigkeit | SI – SafetyInput | `SafetyInput` | x (Sprint 1) | erfüllt |
| **NF2** – Reaktionszeit ≤ 200 ms | MCU | `MainControlUnit` | x (Sprint 2) | erfüllt |
| **NF3** – 100-ms-Zyklus | MCU – MainControlUnit | `MainControlUnit` | x (Sprint 1) | stabil |
| **NF4** – Wartungszeit korrekt führen | PM – MaintenanceManager | `MaintenanceManager` | x (Sprint 2) | erfüllt |
| **NF5** – Dateigröße & Log-Rollover | PM – PersistenceManager | `CsvLogger`, `FileDriver` | x | **Implementiert in Sprint 3** |
| **NF6** – Kompilierbarkeit & Portabilität | Build / Gesamtsystem | gesamte Architektur | x (Sprint 1) | erfüllt |
| **NF7** – Hardware-Selbsttest | Hardware – Gesamtsystem | `MainControlUnit`, `MotorActuator`, `CurrentSensor` | x (Sprint 2) | erfüllt |

---

## Markierte Requirements für Sprint 3

| Requirement | Kurzbeschreibung |
|------------|------------------|
| **F5** | Persistentes CSV-Logging von Systemstatus und Messwerten (1 Hz) |
| **NF5** | Begrenzung der Logdateigröße und automatischer Log-Rollover |

---

## Traceability: Software-Design-Komponenten → Systemkomponenten (Sprint 3)

| Software-Design-Komponente | Zugehörige Systemkomponente | Beschreibung |
|-----------------------------|-----------------------------|--------------|
| `CsvLogger`                | PM – Persistence            | Zyklische Protokollierung von Status- und Messdaten |
| `FileDriver`               | PM – Persistence            | Dateizugriff, Größenüberwachung, Log-Rollover |
| `MainControlUnit`          | MCU – Control Logic         | Bereitstellung der zu loggenden Status- und Messdaten |
| `MonitoringService`        | CS – Monitoring             | Lieferung der Strommesswerte an Logger |
| `MaintenanceManager`       | PM – Maintenance            | Bereitstellung von Wartungsstatus für Logging |
| `SafetyInput`              | SI – Safety                 | Safety-Zustand als Bestandteil des Systemstatus |
| `MotorActuator`            | MA – Hardware               | Aktueller Ansteuerungszustand für Statuslogging |

---
