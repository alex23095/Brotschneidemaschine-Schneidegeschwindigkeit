# Architektur – Sprint 3

## Überblick
Die Software bleibt in der etablierten 4-Schichten-Architektur.  
Sprint 3 schließt die Persistenz-Schicht funktional ab: **CSV-Logging (F5)** und **Log-Rotation / Dateigrößenbegrenzung (NF5)** werden in der Architektur weiterhin vorgesehen.  

## Schichtenmodell

1. **User Interface Layer**
   - `UserInterfaceService`, `LogViewer` 
   - Bedienung (Sollwert 10%-Schritte), Anzeige von Status & Maintenance

2. **Control Logic Layer**
   - `MainControlUnit`, `SetpointManager`, `MonitoringService`, `MaintenanceManager`
   - Zentrale Steuerlogik, Sollwertaufbereitung, Safety-Verarbeitung, Statusbereitstellung, Monitoring/Wartung

3. **Hardware Abstraction Layer**
   - `MotorActuator`, `CurrentSensor`, `SafetyInput`
   - Motoransteuerung, Strommessung, Safety-Eingänge

4. **Persistence Layer**
   - `CsvLogger`, `FileDriver`
   - Logging, Dateigrößenprüfung, Rotation

## Hauptänderungen seit Sprint 2 (methodenkonsistent)
- Externe Bedienung erfolgt über **MCU-API**, die im Code existiert:
  - Sollwert: `setSpeedStep(int step10)`
  - Safety-Inputs an MCU: `setSafetyInputs(const SafetyInput::Inputs& in)`
  - Zyklus: `tick()` und/oder `executeCycle()`
- Safety-Verarbeitung ist zweistufig:
  - MCU nimmt Inputs an (`setSafetyInputs(...)`)
  - `SafetyInput` verarbeitet (`readInputs(...)`) und liefert Status (`getSafetyStatus()`, `isSafetyOk()`)
- Monitoring und Wartung sind über konkrete APIs abfragbar:
  - Strom: `lastMeasuredCurrentMa()`, `isOvercurrent()`
  - Wartung: `isMaintenanceDue()`, `maintenanceAdvice()`

## Komponentenrollen

### User Interface
- Bedienung & Statusanzeige  
- Übergabe Sollwertstufen / ggf. Safety-Inputs (z. B. Simulation/Test)

### Control Logic
- Koordination aller Komponenten  
- Sollwertverwaltung über `SetpointManager`  
- Safety-Verarbeitung über `SafetyInput`  
- Statusbereitstellung über `getStatus()`

### Hardware-Abstraktion
- Motorregelung/Ansteuerung über `MotorActuator`  
- Strommessung über `CurrentSensor`  
- Safety-Zustand über `SafetyInput`

### Persistenz
- (Falls implementiert) nachgelagerte Protokollierung/Rotation ohne Einfluss auf Safety-/Regelpfad

## Schnittstellen 

| Quelle | Ziel | Aufruf |
|--------|------|--------|
| `MainControlUnit` → `SafetyInput` | `SafetyInput` | `readInputs(const SafetyInput::Inputs&)` |
| `MainControlUnit` ← `SafetyInput` | `SafetyInput` | `isSafetyOk() const`, `getSafetyStatus() const`, `hasFault() const`, `isEstopActive() const`, `isGuardOpen() const` |
| `MainControlUnit` → `SetpointManager` | `SetpointManager` | `setSpeedStep(int)`, `update()` |
| `MainControlUnit` ← `SetpointManager` | `SetpointManager` | `currentSetpointRpm() const`, `targetSetpointRpm() const`, `commandStep() const` |
| `MainControlUnit` → `MotorActuator` | `MotorActuator` | `setCommandRpm(int)`, `setSafetyOk(bool)`, `updateControlLoop()` |
| `MainControlUnit` ← `MotorActuator` | `MotorActuator` | `isEnabled() const`, `dutyCyclePercent() const`, `commandRpm() const`, `getMeasuredSpeed() const` |
| `MainControlUnit` → `MonitoringService` | `MonitoringService` | `updateMonitoring()` |
| `MainControlUnit` ← `MonitoringService` | `MonitoringService` | `readCurrent() const`, `checkOvercurrent() const`, `sampleIntervalMs() const` |
| `MonitoringService` → `CurrentSensor` | `CurrentSensor` | `readCurrent()`, `checkOvercurrent(std::uint16_t) const` |
| `MainControlUnit` → `MaintenanceManager` | `MaintenanceManager` | `updateRuntimeMs(std::uint64_t)` |
| `MainControlUnit` ← `MaintenanceManager` | `MaintenanceManager` | `isMaintenanceDue() const`, `getMaintenanceAdvice() const`, `runtimeMs() const` |


## Requirements-Zuordnung

| Schicht | Zuordnung |
|---------|-----------|
| User Interface | F2, NF6 |
| Control Logic | F1, F2, NF1, NF3, NF7 |
| Hardware Abstraction | F2, F3, NF2 |
| Persistence | F5, NF5 |

## Architekturregeln
- Kommunikation nur über definierte Schnittstellen  
- Systemgrenze: Extern → `MainControlUnit` (keine direkte Steuerung von HAL-Komponenten im Normalbetrieb)  
- MCU ist zentrale Steuerinstanz  
- Hardware vollständig gekapselt  
- Monitoring/Wartung liefern Daten/Flags, beeinflussen Steuerpfad nicht direkt  
- Persistenz ist nachgelagert und darf Safety-/Regelpfade nicht beeinflussen
