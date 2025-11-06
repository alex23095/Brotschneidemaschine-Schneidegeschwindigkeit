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


### Markierte Requirements für Sprint 1

| Requirement | Kurzbeschreibung |
|--------------|------------------|
| **F1** | Motorsteuerung implementiert |
| **F2** | Drehzahlsteuerung über UI |
| **F9** | Not-Halt / Sicherheitsabschaltung |
| **NF1** | Sicherheitsnorm (PL d) |
| **NF2** | Reaktionszeit & Bedienbarkeit |
| **NF3** | Sicherheitsreaktionszeit |


### Ergänzung: Traceability SW-Design → Architekturkomponenten

| Software-Design-Komponente | Zugehörige Systemkomponente | Beschreibung |
|-----------------------------|-----------------------------|---------------|
| `MainControlUnit` | MC – Motor-Controller | Regelung und Sollwertverarbeitung |
| `PWMManager` | MC – Motor-Controller | PWM-Ausgabe, Sanftanlauf |
| `UserInterfaceService` | UI – User-Interface | Anzeige und Eingabe |
| `PresetManager` | UI – User-Interface | Drehzahl-Presets |
| `EmergencyStopHandler` | SCU – Safety-Control | Reaktion auf Not-Halt |
| `SafetySupervisor` | SCU – Safety-Control | Überwachung Sicherheitszustände |
| `MonitoringService` | MS – Monitoring | Berechnung Restlebensdauer |
| `LogManager` | DM – DataManagement | Aufzeichnung Betriebsdaten |
