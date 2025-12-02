# Architektur – Sprint 2 

## Überblick
Die Software folgt weiterhin einer klar getrennten 4-Schichten-Architektur.  
Für Sprint 2 wurden Schnittstellen bereinigt, das Klassendiagramm angepasst und die Struktur an die tatsächliche Implementierung ausgerichtet.

## Schichtenmodell

1. **User Interface Layer**
   - `UserInterfaceService`, `LogViewer`
   - Bedienung (Start/Stop/E-Stop, Sollwert 10%-Schritte), Anzeige von Status & Maintenance

2. **Control Logic Layer**
   - `MainControlUnit`, `SetpointManager`, `MonitoringService`, `MaintenanceManager`, `Status`, `RunState`
   - Zentrale Steuerlogik, Zustandsverwaltung, Rampenfunktion, Wartungs- & Monitoringvorbereitung

3. **Hardware Abstraction Layer**
   - `MotorActuator`, `CurrentSensor`, `SafetyInput`
   - PWM-Ausgabe, Strommessung, Safety-Signal

4. **Persistence Layer**
   - `CsvLogger`, `FileDriver`
   - 1-Hz-Logging, Dateiverwaltung

## Hauptänderungen seit Sprint 1
- MainControlUnit an realen Code angepasst (`setUiSpeedCommandStep`, `setSafetyInputs`, `tick`)
- MotorActuator gestrafft (nur `setDutyPercent`)
- SafetyInput um echten SafetyState ergänzt
- Nicht benötigte Methoden entfernt
- Alle geplanten Module bleiben bestehen (UI, Monitoring, Maintenance, Logging)

## Komponentenrollen

### User Interface
- Bedienung & Statusanzeige  
- Übergabe der Sollwertstufen und Start/Stop/E-Stop  
- Anzeige von Maintenance-Status

### Control Logic
- Koordination aller Komponenten  
- Sollwertaufbereitung (Quantisierung + Rampen)  
- Safety-Verarbeitung  
- Bereitstellung des Status-Objekts (`Status`)

### Hardware-Abstraktion
- PWM/Duty-Cycle setzen  
- Safety-Zustand lesen  
- Strommessung

### Persistenz
- Periodisches Logging  
- Dateigrößenprüfung & Rotation

## Schnittstellen

| Quelle | Ziel | Aufruf |
|--------|------|--------|
| UI → MCU | `setUiSpeedCommandStep()`, `commandStart()`, `commandStop()`, `commandEStop()` |
| MCU → UI | `displayStatus()`, `showMaintenance()` |
| MCU → SetpointManager | `quantize10()`, `rampTo()` |
| MCU → SafetyInput | `setSafetyInputs()`, `isTripped()` |
| MCU → MotorActuator | `setDutyPercent()` |
| MCU → CurrentSensor | `readMilliAmpere()` |
| MCU → CsvLogger | `log1Hz()` |
| CsvLogger → FileDriver | `writeLine()`, `sizeBytes()` |

## Requirements-Zuordnung

| Schicht | Zuordnung |
|---------|-----------|
| User Interface | F2, NF2, NF6 |
| Control Logic | F1, F2, NF2, NF3 |
| Hardware Abstraction | F1, F2 |
| Persistence | F3, NF4 |

## Architekturregeln
- Kommunikation nur über definierte Schnittstellen  
- UI ↔ MCU ↔ Hardware (keine Querbeziehungen)  
- MCU ist zentrale Steuerinstanz  
- Hardware vollständig gekapselt  
- Logging entkoppelt und erweiterbar

