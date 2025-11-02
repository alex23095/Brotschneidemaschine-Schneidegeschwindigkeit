# Architektur

## Architekturmuster festlegen

**Schichten- und komponentenbasierte Architektur**:

- **Klare Trennung der Verantwortlichkeiten**:
  - hardwarenahe Ansteuerung des Motors
  - fachliche / regelungstechnische Logik
  - Benutzeroberfläche
  - Safety / Not-Halt
  - Persistenz / Logging
- **Nur Top-down-Kommunikation**: UI → Control Logic → Hardware; Monitoring und Safety melden nur nach oben, nicht quer.
- **Erweiterbarkeit**: spätere Funktionen (Wartungsempfehlung, Export, detailliertes Monitoring) können als eigene Komponenten ergänzt werden, ohne bestehende Kernlogik zu verändern.
- **Testbarkeit**: jede Komponente kann isoliert getestet werden (z. B. Control Logic mit Mock-Motor, UI ohne echte Hardware).
- **Kein Querzugriff**: UI spricht **nicht direkt** die Hardware an, sondern immer über die Control Logic / MainControlUnit. Safety meldet nur Zustände, schaltet aber nicht selbst den Motor.

_Ebenenübersicht_:

1. **Presentation Layer**: `UserInterfaceService`
2. **Application / Control Layer**: `MainControlUnit`, `MonitoringService`, `SafetyControlUnit`
3. **Hardware / Integration Layer**: `MotorController`, Sensorik
4. **Persistence / Infrastructure Layer**: `DataManagement`

## Komponentendiagramm

![Komponentendiagramm](../referenziert/Architektur/Komponentendiagramm_Motorsteuerung.png)

### Komponentenübersicht

| **Komponente**        | **Requirements**                                                                                                  | **Beschreibung / Zweck**                                                                 |
|-----------------------|--------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|
| **UI – UserInterfaceService** | F2, F6, NF2, NF4                                                                                                 | Anzeige, Eingabe der Drehzahl, Status-/Wartungspopups                                     |
| **MC – Motor-Controller**     | F1, F2                                                                                                       | Umsetzung der Drehzahlvorgabe, Ansteuerung PWM / Treiber                                 |
| **Control Logic – MainControlUnit** | F1, F2, F9, NF2, NF3                                                                                             | Orchestrierung, Validierung, Weitergabe von Sollwerten, Reaktion auf Safety               |
| **SCU – Safety-Control-Unit** | F9, NF1, NF3                                                                                                 | Überwachung Sicherheitszustände, Meldung „TRIPPED“ an Control Logic                       |
| **MS – MonitoringService**    | F3, F4, F5                                                                                                   | Auswertung von Betriebsdaten, RUL-Berechnung, Trigger für Wartung                         |
| **DM – DataManagement**       | F7, F8, NF5                                                                                                  | Logging von Events („SpeedChange“, „E-STOP“, „MaintenanceAdvice“), später Export          |

### Traceability (Auszug)

| **Requirement** | **Komponente**             |
|-----------------|----------------------------|
| F1              | Control Logic, Motor-Controller |
| F2              | UI, Control Logic          |
| F3              | MonitoringService          |
| F4              | MonitoringService          |
| F5              | MonitoringService, UI      |
| F7, F8, NF5     | DataManagement             |
| F9, NF1, NF3    | Safety-Control-Unit, Control Logic |
| NF2, NF4        | UI                         |

## Verantwortlichkeiten der Komponenten

| **Komponente**          | **Rolle**                 | **Verantwortlichkeiten**                                                                 |
|-------------------------|---------------------------|-------------------------------------------------------------------------------------------|
| **UserInterfaceService**| Präsentationsschicht      | Eingabe Drehzahl (10 %-Schritte), Anzeige Status, Wartungshinweise, Popup bei E-Stop     |
| **MainControlUnit**     | zentrale Steuerlogik      | Entgegennahme der UI-Befehle, Plausibilisierung, Weitergabe an Motor, Logging auslösen    |
| **Motor-Controller**    | Hardware-/Actuator-Schicht| PWM/Drehzahl setzen, Rückmeldung „ok“ / „stopped“                                        |
| **Safety-Control-Unit** | Querschnitt Sicherheit    | Not-Halt bzw. „TRIPPED“ erkennen und an MCU melden                                       |
| **MonitoringService**   | Analyse / Zustand         | Sensordaten auswerten, RUL berechnen, Wartungstrigger erzeugen                            |
| **DataManagement**      | Infrastruktur / Persistenz| Systemereignisse protokollieren, spätere Exportfunktion                                  |

## Schnittstellendefinition

| **Quelle**             | **Ziel**                | **Schnittstelle / Aufruf**                                                                                       | **Zweck**                                  |
|------------------------|-------------------------|-------------------------------------------------------------------------------------------------------------------|--------------------------------------------|
| `UserInterfaceService` | `MainControlUnit`       | `setSpeedPercent(step10)`                                                                                         | UI meldet neue Drehzahlanforderung         |
| `MainControlUnit`      | `MotorController`       | `setSpeed(rpmTarget)`                                                                                             | Sollwert an Hardware weitergeben           |
| `MotorController`      | `MainControlUnit`       | `confirmSpeedSet()` / `stopped()`                                                                                 | Rückmeldung aus Hardware                   |
| `MainControlUnit`      | `DataManagement`        | `logEvent(eventName, payload?)`                                                                                   | zentrales Logging                          |
| `MainControlUnit`      | `MonitoringService`     | `getStatusInputs()`                                                                                               | Anforderung aktueller Zustandsdaten        |
| `MonitoringService`    | `MainControlUnit`       | `returnRUL(rulPercent)`                                                                                           | Rückgabe berechneter Restlebensdauer       |
| `MonitoringService`    | `UserInterfaceService`  | `showMaintenanceAdvice(advice)`                                                                                   | UI soll Wartungshinweis anzeigen           |
| `SafetyControlUnit`    | `MainControlUnit`       | `reportTripCondition(status = TRIPPED)`                                                                           | Safety meldet Störung / Not-Halt           |
| `MainControlUnit`      | `UserInterfaceService`  | `updateDisplay(Status)` / `displayStop()`                                                                         | UI aktualisieren                            |
| `UserInterfaceService` | `DataManagement` (später) | `exportLogs()`                                                                                                    | Exportfunktion (F8)                         |


## Technologiestack

| Kategorie                | Technologie / Tool                           | Begründung                                                                                                   |
|--------------------------|----------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| Sprache                  | Java 17 (alternativ C++ für Embedded-Ziel)   | gute UML-/OO-Abbildung, Tests einfach, viele Libraries                                                       |
| Buildsystem              | Gradle / Maven (bei Java)                    | reproduzierbare Builds, später CI integrierbar                                                               |
| Versionskontrolle        | Git + GitHub / GitLab                        | Nachvollziehbarkeit Änderungen, Reviews                                                                      |
| Modellierung             | PlantUML + draw.io                           | bereits im Projekt genutzt, gut für UML-Diagramme (Sequenz, Kompo, Klassen)                                 |
| IDE                      | IntelliJ IDEA / VS Code mit PlantUML Plugin  | gute UML-Integration, Markdown-Vorschau                                                                      |
| Dokumentation            | Markdown (.md)                               | leicht versionierbar, in Repo ablegbar                                                                       |
| Test-Framework           | JUnit / AssertJ                              | Unittests für Control Logic, Monitoring, Safety                                                              |
| Codeanalyse              | SonarLint / Metrics / IntelliJ Inspections   | Unterstützung bei Clean Code, Komplexität, Kopplung                                                         |
| Deployment / Target      | (später) Embedded-Board / Simulator          | Echtbetrieb erst nach Sicherstellung der Kernlogik                                                           |

**Hinweis zu Abhängigkeiten**:

- `UserInterfaceService` kennt nur `MainControlUnit`.
- `MainControlUnit` kennt **MotorController**, **MonitoringService**, **SafetyControlUnit** und **DataManagement**.
- Hardware-nahe Klassen (Motor, Sensorik) kennen **keine** UI.
- Logging ist zentralisiert über `DataManagement`.
