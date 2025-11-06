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

---

### Komponentenübersicht

| **Komponente**              | **Requirements**                  | **Beschreibung / Zweck**                                                    |
|-----------------------------|-----------------------------------|------------------------------------------------------------------------------|
| **UI – UserInterfaceService** | F1, NF2, NF6                    | Eingabe der Drehzahl (10 %-Schritte), Anzeige Soll-/Istwert, Statusmeldungen |
| **MA – MotorActuator**        | F1, F2, NF6                     | Steuerung des Schneidemotors, PWM-Ausgabe, Stop bei Not-Halt                |
| **MCU – MainControlUnit**     | F1, F2, NF2, NF3, NF5           | Koordination, Sollwertverarbeitung, Zustandsverwaltung (RUN/STOP/FAULT)     |
| **SI – SafetyInput**          | F2, NF2, NF7                    | Überwachung Not-Halt, Signalweitergabe an Steuerlogik                        |
| **CS – CurrentSensor**        | F3, NF3                         | Erfassung der Stromaufnahme (500 ms), Mittelwertbildung                      |
| **MNT – MaintenanceManager**  | F4                              | Laufzeitzählung und Wartungshinweis nach 48 h                               |
| **DM – CsvLogger / FileDriver** | F5, NF4, NF5                  | Protokollierung der Prozessdaten (1 Hz), CSV-Export und Dateiverwaltung      |

---

### Traceability (Auszug)

| **Requirement** | **Komponente**                  |
|-----------------|----------------------------------|
| F1              | MCU, MA, UI                     |
| F2              | MCU, SI, MA                     |
| F3              | CS, MCU                         |
| F4              | MNT                             |
| F5              | DM                              |
| NF1             | MCU, gesamte Architektur (C++)  |
| NF2             | UI, MCU, SI                     |
| NF3             | MCU, CS                         |
| NF4             | DM                              |
| NF5             | MCU, DM                         |
| NF6             | MA, MCU                         |
| NF7             | MA, SI, CS                      |

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

| **Quelle**             | **Ziel**                | **Schnittstelle / Aufruf**                                               | **Zweck**                                      |
|------------------------|-------------------------|---------------------------------------------------------------------------|-----------------------------------------------|
| `UserInterfaceService` | `MainControlUnit`       | `setSpeedPercent(step10)`                                                 | Übermittlung neuer Sollwertvorgabe (10 %-Stufe) |
| `MainControlUnit`      | `MotorActuator`         | `setSpeed(rpmTarget)` / `stopEmergency()`                                 | Weitergabe Sollwert bzw. sofortiger Stop       |
| `MotorActuator`        | `MainControlUnit`       | `confirmSpeedSet()` / `stopped()`                                         | Rückmeldung Motorzustand                      |
| `MainControlUnit`      | `SafetyInput`           | `readSafetyStatus()`                                                      | Abfrage Not-Halt-Status                       |
| `SafetyInput`          | `MainControlUnit`       | `reportTripCondition(status = TRIPPED)`                                   | Signalisiert Not-Halt an Logik                |
| `MainControlUnit`      | `CurrentSensor`         | `readCurrent()`                                                           | Anforderung Messwert                          |
| `CurrentSensor`        | `MainControlUnit`       | `returnSample(meanCurrent)`                                               | Rückgabe Mittelwert                           |
| `MainControlUnit`      | `MaintenanceManager`    | `updateRuntime(deltaTime)`                                                | Übermittlung der Betriebszeit                 |
| `MaintenanceManager`   | `MainControlUnit`       | `maintenanceDue()`                                                        | Signal für fällige Wartung                    |
| `MainControlUnit`      | `CsvLogger`             | `logEvent(eventType, payload)`                                            | Ereignis-Logging                              |
| `CsvLogger`            | `FileDriver`            | `writeCSV(fileName, data)`                                                | Speichern von Logdaten                        |
| `MainControlUnit`      | `UserInterfaceService`  | `updateDisplay(status)` / `showPopup(msg)`                                | Rückmeldung aktueller Systemzustand           |

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

**Hinweis zu Abhängigkeiten:**

- `UserInterfaceService` kommuniziert nur mit der `MainControlUnit`.
- `MainControlUnit` koordiniert alle anderen Komponenten (Motor, Safety, Sensorik, Logging).
- Hardware-Module (`MotorActuator`, `CurrentSensor`, `SafetyInput`) sind vollständig gekapselt.  
- Logging erfolgt zentral über `CsvLogger`, der über `FileDriver` schreibt.
