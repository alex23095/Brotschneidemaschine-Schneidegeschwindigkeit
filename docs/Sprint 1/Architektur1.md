# Architektur

## Architekturmuster festlegen

**Schichten- und komponentenbasierte Architektur**:

- **Klare Trennung der Verantwortlichkeiten**:
  - hardwarenahe Steuerung des Schneidemotors
  - Steuerungs- und Zustandslogik (MainControlUnit)
  - Benutzeroberfläche (Bedienung und Statusanzeige)
  - Safety / Not-Halt-Signalverarbeitung
  - Datenerfassung und Protokollierung
- **Top-down-Kommunikation:** Eingaben erfolgen über das UI → Logik → Hardware; Rückmeldungen (z. B. Safety, Monitoring) werden nach oben propagiert.
- **Erweiterbarkeit:** Wartungslogik, Monitoring und Logging können als separate Module ergänzt werden, ohne bestehende Steuerlogik zu verändern.
- **Testbarkeit:** Jede Komponente kann isoliert getestet werden (Mock-Interfaces für Hardware und UI).
- **Kein Querzugriff:** UI kommuniziert ausschließlich mit der `MainControlUnit`; Hardware-Komponenten (z. B. Motor, Sensor) sind abstrahiert.

_Ebenenübersicht_:

1. **Presentation Layer**: `UserInterfaceService`
2. **Application / Control Layer**: `MainControlUnit`, `SetpointManager`, `MaintenanceManager`
3. **Hardware Layer**: `MotorActuator`, `CurrentSensor`, `SafetyInput`
4. **Persistence / Infrastructure Layer**: `CsvLogger`, `FileDriver`

---

## Komponentendiagramm

![Komponentendiagramm](../referenziert/Architektur/Komponentendiagramm.png)

## Zuordnung der Requirements zu den Architektur-Schichten

| Architektur-Schicht      | Zugehörige Requirements | Beschreibung |
|--------------------------|--------------------------|--------------|
| **User Interface**       | F1, NF2, NF6             | Eingabe der Drehzahl, schnelle Reaktionszeit, Genauigkeit der Anzeige/Sollwertvorgabe |
| **Control-Logic**        | F1, F2, NF2, NF3, NF5     | Sollwertaufbereitung, Not-Halt-Reaktion, Zustandsverwaltung RUN/STOP/FAULT, deterministische Zykluszeiten |
| **Hardware Abstraction** | F2, F3, NF2, NF7          | Not-Halt, Stromaufnahme erfassen, Hardware-Reaktionszeit, Temperaturbereich |
| **Persistence Manager**  | F3, F4, F5, NF4, NF5      | Logging 1 Hz, Wartungszähler, CSV-Dateiverwaltung, maximale Dateigröße |

---

## Beschreibung der Komponenten

| Komponente         | Rolle                         | Verantwortlichkeiten |
|--------------------|-------------------------------|-----------------------|
| **UserInterface**  | Präsentationsschicht          | Eingabe der Schneidegeschwindigkeit (10 %-Schritte); Anzeige von Soll-/Istwert; Anzeige von Statusmeldungen (RUN/STOP/FAULT); Rückmeldung an die Steuerungslogik |
| **Steuerungslogik** | Anwendungslogik              | Verarbeitung der Bedienbefehle; Verwaltung der Betriebszustände (RUN, STOP, FAULT); Sollwertaufbereitung; Reaktion auf Not-Halt; zyklische Steuerung des Gesamtsystems |
| **HardwareAbstraction** | Hardware-Interface       | Ansteuerung des Schneidemotors (PWM/Duty-Cycle); Erfassung der Stromaufnahme; Weiterleitung des Safety-Signals; Vereinheitlichung aller hardware-nahen Funktionen |
| **PersistenceManager** | Speicher-/Diagnose-Schicht | Protokollierung der Prozessdaten (Sollwert, Istwert, Stromaufnahme, Systemzustand); CSV-Dateiverwaltung (1 Hz); Verwaltung des Wartungszählers; Bereitstellung von Diagnose- und Log-Daten |

---

## Verantwortlichkeiten der Komponenten

| **Komponente**          | **Rolle**                 | **Verantwortlichkeiten**                                                                 |
|-------------------------|---------------------------|-------------------------------------------------------------------------------------------|
| **UserInterfaceService**| Präsentationsschicht      | Eingabe (Start/Stop/Sollwert), Anzeige von Status, Drehzahl, Warn- und Wartungshinweisen |
| **MainControlUnit**     | zentrale Steuerlogik      | Steuerung aller Teilfunktionen, Zustandsverwaltung RUN/STOP/FAULT, Logging-Trigger       |
| **MotorActuator**       | Hardware-Schicht          | PWM-Ausgabe, Geschwindigkeit setzen, sofortiger Stop bei Safety-Signal                   |
| **SafetyInput**         | Safety-Schicht            | Not-Halt-Signal detektieren, Statusmeldung an Logik weiterleiten                         |
| **CurrentSensor**       | Sensorik / Monitoring     | Stromaufnahme messen (500 ms), Mittelwert über 5 Messpunkte                              |
| **MaintenanceManager**  | Wartungslogik             | Betriebszeit erfassen, Wartungshinweis nach 48 h kumulierter Laufzeit                   |
| **CsvLogger / FileDriver** | Persistenz / Logging   | Ereignisse und Prozessdaten protokollieren, CSV-Datei rotieren bei 1 MB                 |

---

## Schnittstellendefinition

| **Quelle**             | **Ziel**                | **Schnittstelle / Aufruf**                         | **Zweck**                                      |
|------------------------|-------------------------|-----------------------------------------------------|-----------------------------------------------|
| UserInterfaceService   | MainControlUnit         | `setSpeedPercent(step10)`                          | Übergabe einer neuen Sollwertvorgabe (10%-Stufen) |
| UserInterfaceService   | MainControlUnit         | `requestStart()` / `requestStop()`                 | Bedienbefehle für Start/Stop                  |
| MainControlUnit        | UserInterfaceService     | `updateDisplay(status, rpmSet, rpmActual)`         | UI-Update zu Systemstatus und Werten          |
| MainControlUnit        | SetpointManager         | `setTarget(step10)`                                | Übergabe der Ziel-Sollwertstufe               |
| SetpointManager        | MainControlUnit         | `getCurrentSetpoint()`                             | Rückgabe des aufbereiteten Sollwerts          |
| MainControlUnit        | SafetyInput             | `readSafetyStatus()`                               | Abfrage Not-Halt-Zustand                      |
| SafetyInput            | MainControlUnit         | `onSafetyTrip()`                                   | Sofortiges Melden einer Not-Halt-Auslösung     |
| MainControlUnit        | MotorActuator           | `setSpeed(rpmTarget)`                              | Übergabe des Soll-Drehwerts an Motor          |
| MainControlUnit        | MotorActuator           | `stopEmergency()`                                  | Sofortstop bei Not-Halt                       |
| MotorActuator          | MainControlUnit         | `feedbackMotorState(state)`                        | Rückmeldung über Motorzustand                 |
| MainControlUnit        | CurrentSensor           | `requestCurrent()`                                 | Anforderung eines Strommesswerts              |
| CurrentSensor          | MainControlUnit         | `deliverCurrent(meanValue)`                        | Rückgabe des Mittelwerts über 5 Messpunkte    |
| MainControlUnit        | PersistenceManager      | `logEvent(eventType, payload)`                     | Protokollierung von Systemereignissen         |
| MainControlUnit        | PersistenceManager      | `updateRuntime(deltaTime)`                         | Übergabe der Betriebszeit an Wartungslogik     |
| PersistenceManager     | MainControlUnit         | `maintenanceDue()`                                 | Signalisiert anstehende Wartung               |

---

## Technologiestack

| Kategorie                | Technologie / Tool                            | Begründung                                                                                         |
|--------------------------|-----------------------------------------------|----------------------------------------------------------------------------------------------------|
| **Programmiersprache**   | C++ (STM32 / ARM Cortex-M)                    | Objektorientierte Struktur, direkte Hardwarezugriffe, gute Testbarkeit                             |
| **Buildsystem**          | CMake / STM32CubeIDE                          | Standardtoolchain für Embedded-Systeme                                                             |
| **Versionskontrolle**    | Git + GitHub / GitLab                         | Nachvollziehbare Änderungen, Team-Review-fähig                                                     |
| **Modellierung**         | PlantUML + draw.io                            | Nutzung für Klassendiagramme, Sequenz- und Komponentendarstellungen                                |
| **IDE**                  | STM32CubeIDE / VS Code mit C++-Plugin         | Unterstützung für Embedded-Builds und serielle Debugausgabe                                        |
| **Dokumentation**        | Markdown (.md)                                | Versionssicher, für Git-Repos optimiert                                                            |
| **Testframework**        | Unity / CppUTest                              | Leichtgewichtiges Framework für Unittests in Embedded-Umgebung                                    |
| **Codeanalyse**          | cppcheck / SonarLint                          | Analyse von Speicherzugriff, Laufzeitfehlern, Komplexität                                         |
| **Target / Deployment**  | STM32-Board (Simulation optional via Serial)  | Reale Ausführung und Test der Steuerlogik                                                          |

## Hinweis zu Abhängigkeiten

- **UserInterfaceService** kommuniziert ausschließlich mit der **MainControlUnit** und sendet Bedienbefehle (Start/Stop, Not-Halt, Geschwindigkeitsstufen) sowie erhält Status- und Wartungsmeldungen.
- **MainControlUnit** ist die zentrale Steuer- und Koordinationsinstanz des Systems. Sie verbindet alle weiteren Komponenten:
  - SetpointManager  
  - MotorActuator  
  - SafetyInput  
  - CurrentSensor  
  - PersistenceManager
- Die Hardware-nahen Module (**MotorActuator**, **CurrentSensor**, **SafetyInput**) sind vollständig gekapselt und werden ausschließlich über definierte Schnittstellen von der MainControlUnit angesteuert.
- Alle Aufgaben zur Datenpersistenz, Protokollierung und Wartungsverwaltung werden zentral durch den **PersistenceManager** ausgeführt.  
  Dieser übernimmt intern das CSV-Logging, die Dateiverwaltung sowie die Laufzeit- und Wartungsdatenspeicherung.

