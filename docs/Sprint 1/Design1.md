# Design

## Design-Zerlegung

### MCU – Main Control Unit

#### C01 Sollwert- und Drehzahlsteuerung
- **Aufgabe:** Verarbeitung der Sollwertvorgabe (10-%-Stufen) aus dem User Interface, Aufbereitung des Drehzahlsollwerts und Weitergabe an den Motor-Aktuator.
- **Inputs:** `ui_speed_cmd`, `safety_status`
- **Outputs:** `speed_target_pct`, `system_state`
- **Timing:** zyklisch alle 100 ms
- **Trace:** F1, F2, NF2, NF3

#### C02 Not-Halt-Verarbeitung
- **Aufgabe:** Auswertung des Safety-Signals vom SafetyInput, Einleitung eines sicheren Stopps (≤ 100 ms) und Übergang in den Zustand FAULT.
- **Inputs:** `estop_signal` (von SI), `motor_feedback`
- **Outputs:** `stop_command`, `status_fault`
- **Timing:** ereignisgetrieben, Reaktionszeit ≤ 100 ms
- **Trace:** F2, NF2, NF7

#### C06 Systemlogik und Zustandsautomat
- **Aufgabe:** Verwaltung der Betriebszustände RUN, STOP und FAULT inkl. Übergangslogik, Start/Stop-Verhalten und Reset-Mechanismus; Übergabe von Statusinformationen an UI.
- **Inputs:** `user_command`, `safety_status`, `maintenance_due_flag`
- **Outputs:** `system_state`, `ui_feedback`
- **Timing:** zyklisch alle 100 ms
- **Trace:** NF3, NF5

---

### CS / PM – Monitoring, Wartung und Protokollierung

#### C03 Monitoring-Eingang
- **Aufgabe:** Erfassen der Motorstromaufnahme über den CurrentSensor im 500-ms-Intervall und Berechnung eines gleitenden Mittelwerts über 5 Messungen.
- **Inputs:** `adc_current`, `adc_timestamp` (von CS)
- **Outputs:** `mean_current`
- **Timing:** zyklisch alle 500 ms
- **Trace:** F3, NF3

#### C04 Wartungsmanagement
- **Aufgabe:** Aufsummieren der kumulierten Laufzeit im Betriebszustand RUN und Setzen eines Wartungskennzeichens nach 48 h Betrieb; Bereitstellung des Wartungsstatus für MCU/UI.
- **Inputs:** `runtime_increment` (von MCU), optional `mean_current`
- **Outputs:** `maintenance_due_flag`
- **Timing:** typischerweise 1-s-Takt (aus Systemzeit/MCU abgeleitet)
- **Trace:** F4

#### C05 Protokollierung / CSV-Logger
- **Aufgabe:** Erfassung und Ablage von Soll-/Istwert, Stromaufnahme und Betriebszustand in einer CSV-Datei; automatisches Anlegen einer neuen Logdatei beim Erreichen von 1 MB.
- **Inputs:** `timestamp`, `speed_target_pct`, `speed_actual_pct`, `mean_current`, `system_state`, `maintenance_due_flag`
- **Outputs:** `log_entry.csv` (fortlaufende CSV-Datei mit Rollover)
- **Timing:** 1 Hz (zyklisch)
- **Trace:** F5, NF4, NF5

---

## Klassendiagramm

Das **Klassendiagramm** beschreibt die **statische Struktur** des Softwaresystems.  
Es zeigt die **Systemkomponenten (Klassen)**, deren **Attribute und Methoden** sowie die **Beziehungen** zwischen ihnen.  

Für das Schneidesystem sind die zentralen Klassen in funktionale Gruppen gegliedert:

- **UserInterface** – Eingabe der Drehzahl, Anzeige von Status und Wartungshinweisen  
- **ControlLogic** – Hauptsteuerung (`MainControlUnit`, `SetpointManager`, `MaintenanceManager`)  
- **HardwareAbstraction** – Motor- und Sensorschnittstellen (`MotorActuator`, `CurrentSensor`)  
- **SafetySystem** – Not-Halt-Erkennung (`SafetyInput`)  
- **PersistenceManager** – Protokollierung und CSV-Export (`CsvLogger`, `FileDriver`)  

Das Diagramm definiert die Komponenten und deren Interaktionen.  
Es dient als Grundlage für Implementierung, Test und spätere Erweiterungen.

![Klassendiagramm](../referenziert/Design/Klassendiagramm.png)

---

## Sequenzdiagramm

Das **Sequenzdiagramm** stellt den **zeitlichen Ablauf** einer Interaktion zwischen den Objekten dar.  
Es zeigt, **welche Komponenten in welcher Reihenfolge kommunizieren**, um eine bestimmte Funktion umzusetzen.  

Im Diagramm sind folgende Abläufe dargestellt:

- **Drehzahländerung** über das User Interface → MainControlUnit → MotorActuator (F1, NF2)  
- **Wartungshinweis** nach Ablauf der 48 h Laufzeit (F4)  
- **Safety-Stop** bei Not-Halt-Signal (F2, NF7)  

Das Sequenzdiagramm unterstützt die **Testvorbereitung**,  
da es exakt aufzeigt, wann welche Komponente aktiv ist und welche Nachrichten versendet werden.

![Sequenzdiagramm](../referenziert/Design/Sequenzdiagramm.png)

---

## Kommunikationsdiagramm

Das **Kommunikationsdiagramm** zeigt die gleichen Abläufe wie das Sequenzdiagramm,  
jedoch in einer **strukturellen, knotenbasierten Darstellung**.  

Es verdeutlicht:
- welche Komponenten direkt miteinander kommunizieren,  
- welche Nachrichten und Datenflüsse bestehen,  
- und in welcher Reihenfolge (1.x, 2.x, 3.x) die Kommunikation abläuft.  

Das Diagramm eignet sich besonders gut, um **Abhängigkeiten und Nachrichtenpfade** zwischen Systemteilen übersichtlich darzustellen,  
ohne den Fokus auf den zeitlichen Ablauf zu legen.

![Kommunikationsdiagramm](../referenziert/Design/Kommunikationsdiagramm.png)

## Design Pattern

### Singleton-Pattern

Im Projekt wird das **Singleton-Pattern** eingesetzt, um sicherzustellen,  
dass zentrale Steuerungskomponenten wie die `MainControlUnit` nur **einmalig instanziiert** werden.  
Dadurch wird verhindert, dass mehrere voneinander unabhängige Steuerobjekte entstehen,  
die widersprüchliche Sollwerte oder Zustände verwalten könnten.

Das Singleton bietet folgende Vorteile:
- **Globale Zugriffsmöglichkeit** auf die Hauptsteuerung (`MainControlUnit::getInstance()`),
- **Konsistente Zustandsverwaltung** für RUN, STOP, FAULT über das gesamte System,
- **Eindeutige Koordination** der Kommunikation zwischen UI, Motorsteuerung und Safety.

Durch die Nutzung des Singleton-Patterns bleibt die Softwarestruktur einfach, testbar  
und für ein eingebettetes System mit begrenzten Ressourcen optimal geeignet.

