# Design

## Design-Zerlegung (funktional, ohne OO)

### MCU – Main Control Unit

#### **C01 Sollwert- und Drehzahlsteuerung**
- **Aufgabe:** Verarbeitung der Sollwertvorgabe (10-%-Stufen), Aktualisierung des Motor-Istwerts, Weitergabe an Motorsteuerung  
- **Inputs:** `ui_speed_cmd`, `safety_status`  
- **Outputs:** `speed_target`, `system_state`  
- **Timing:** zyklisch alle 100 ms  
- **Trace:** F1, F2, NF2, NF3  

#### **C02 Not-Halt-Verarbeitung**
- **Aufgabe:** Erkennung und Umsetzung des Safety-Signals (STOP innerhalb ≤ 100 ms), Zustandsübergang auf FAULT  
- **Inputs:** `estop_signal`, `motor_feedback`  
- **Outputs:** `stop_command`, `status_fault`  
- **Timing:** ereignisgetrieben, Reaktionszeit ≤ 100 ms  
- **Trace:** F2, NF2, NF7  

#### **C03 Monitoring-Eingang**
- **Aufgabe:** Erfassen der Stromaufnahme über Sensor (500 ms-Intervall), Mittelwert über 5 Messungen bilden  
- **Inputs:** `adc_current`, `adc_timestamp`  
- **Outputs:** `mean_current`  
- **Timing:** zyklisch alle 500 ms  
- **Trace:** F3, NF3  

#### **C04 Wartungsmanagement**
- **Aufgabe:** Aufsummieren der kumulierten Laufzeit, Auslösen eines Wartungshinweises nach 48 h Betrieb  
- **Inputs:** `runtime_increment`, `mean_current`  
- **Outputs:** `maintenance_due_flag`  
- **Timing:** 1 s-Takt (über Systemzeit)  
- **Trace:** F4  

#### **C05 Protokollierung / CSV-Logger**
- **Aufgabe:** Erfassung von Soll-/Istwert, Stromaufnahme, Betriebszustand; Schreiben in CSV-Datei, automatische Neuerstellung bei 1 MB  
- **Inputs:** `speed_target`, `speed_actual`, `mean_current`, `system_state`  
- **Outputs:** `log_entry.csv`  
- **Timing:** 1 Hz (zyklisch)  
- **Trace:** F5, NF4, NF5  

#### **C06 Systemlogik und Zustandsautomat**
- **Aufgabe:** Verwaltung der Betriebszustände **RUN**, **STOP**, **FAULT**, inkl. Übergangslogik und Reset-Mechanismus  
- **Inputs:** `user_command`, `safety_status`, `maintenance_due`  
- **Outputs:** `system_state`, `ui_feedback`  
- **Timing:** zyklisch 100 ms  
- **Trace:** NF3, NF5  

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

