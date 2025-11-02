# Traceability-Matrix

| Requirement | Systemkomponente | **Sprint 1 Umsetzung** |
|------------------|----------------------|-------------------------|
| **F1** | MC - Motor-Controller | Ja – Grundfunktion: Motorsteuerung |
| **F2** | UI - User-Interface-Service | Ja – UI-Steuerung der Drehzahl |
| **F3** | SM - Sensor | Nein – Implementierung in Sprint 2 |
| **F4** | MS - Monitoring-Service | Nein – Abhängig von Sensordaten |
| **F5** | MS - Monitoring-Service | Nein – Wartungslogik später |
| **F6** | UI - User-Interface-Service | Nein – UI-Erweiterung später |
| **F7** | DM - Data-Management | Nein – Logging später |
| **F8** | DM - Data-Management | Nein – Export-Funktion später |
| **F9** | SCU - Safety-Control-Unit | Ja – Not-Halt / Sicherheitsabschaltung |
| **NF1** | SCU - Safety-Control-Unit | Ja – Sicherheitsnorm (PL d) |
| **NF2** | UI - User-Interface-Service | Ja – Reaktionszeit / Bedienbarkeit |
| **NF3** | SCU - Safety-Control-Unit | Ja – Sicherheitsreaktionszeit |
| **NF4** | UI - User-Interface-Service | Nein – Statusanzeige folgt später |
| **NF5** |  DM - Data-Management | Nein – USB-Kompatibilität später |

# Traceability-Matrix (erweitert)

| Requirement | Systemkomponente | Software-Design-Komponente | Sprint 1 | Status |
|--------------|------------------|-----------------------------|-----------|--------|
| **F1** | MC – Motor-Controller | `MainControlUnit`, `PWMManager` | x | Implementiert |
| **F2** | UI – UserInterfaceService | `UserInterfaceService`, `PresetManager` | x | Implementiert |
| **F3** | SM – Sensor | `SensorManager` | - | Geplant für Sprint 2 |
| **F4** | MS – MonitoringService | `MonitoringService` | - | Abhängig von F3 |
| **F5** | MS – MonitoringService | `MaintenanceAdvisor` | - | Später |
| **F6** | UI – UserInterfaceService | `MenuController` | - | Später |
| **F7** | DM – DataManagement | `LogManager` | - | Später |
| **F8** | DM – DataManagement | `ExportService` | - | Später |
| **F9** | SCU – SafetyControlUnit | `EmergencyStopHandler` | x | Basisfunktion aktiv |
| **NF1** | SCU – SafetyControlUnit | `SafetySupervisor` | x | PL d eingehalten |
| **NF2** | UI – UserInterfaceService | `UIEventLoop` | x | Reaktionszeit getestet |
| **NF3** | SCU – SafetyControlUnit | `ReactionTimer` | x | gemessen / validiert |
| **NF4** | UI – UserInterfaceService | `StatusDisplay` | - | folgt in Sprint 2 |
| **NF5** | DM – DataManagement | `UsbHandler` | - | folgt später |

---

### Markierte Requirements für Sprint 1

| Requirement | Kurzbeschreibung |
|--------------|------------------|
| **F1** | Motorsteuerung implementiert |
| **F2** | Drehzahlsteuerung über UI |
| **F9** | Not-Halt / Sicherheitsabschaltung |
| **NF1** | Sicherheitsnorm (PL d) |
| **NF2** | Reaktionszeit & Bedienbarkeit |
| **NF3** | Sicherheitsreaktionszeit |
