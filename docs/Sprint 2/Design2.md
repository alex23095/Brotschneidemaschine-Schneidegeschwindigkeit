# Design – Sprint 2

## Design-Zerlegung

### MCU – Main Control Unit

#### C01 Sollwert- und Drehzahlsteuerung (überarbeitet)
- **Aufgabe:** Verarbeitung der Sollwertvorgabe in 10-%-Schritten über `setUiSpeedCommandStep()`, Übergabe an den `SetpointManager` und Ableitung des Duty-Cycle über den `MotorActuator`.
- **Inputs:** `ui_step10`, `safety_state`
- **Outputs:** `rpm_target`, `duty_target`
- **Timing:** zyklisch über `tick()` (≈ 100 ms)
- **Trace:** F1, F2, NF2, NF3

#### C02 Not-Halt / Safety-Verarbeitung (angepasst)
- **Aufgabe:** Auswertung des Safety-Zustands (`SafetyState::SAFE/TRIPPED`) sowie sofortige Reduktion des Sollwerts auf 0 bei TRIPPED; Weitergabe an den Motor über Duty 0 %.
- **Inputs:** `SafetyInput::getState()`
- **Outputs:** `rpm_target = 0`, `system_fault`
- **Timing:** ereignisnah, reagiert beim nächsten `tick()`
- **Trace:** F2, NF2

#### C06 Systemlogik und Statusbereitstellung (aktualisiert)
- **Aufgabe:** Verwaltung der Systemparameter (aktuelle und Ziel-Drehzahl), Ausgabe des Status an die UI-Schicht, Erzeugung von Monitoring- und Logging-Daten.
- **Inputs:** `ramp_output`, `current_measurement`, `maintenance_flag`
- **Outputs:** `Status`
- **Timing:** zyklisch
- **Trace:** NF3

---

### CS / PM – Monitoring, Wartung und Protokollierung (modelliert, nicht implementiert)

#### C03 Monitoring-Eingang (modelliert)
- **Aufgabe:** Mittelwertbildung der Stromaufnahme über Fenster von 5 Werten. Dient der späteren Diagnose, Belastungsabschätzung und Wartungslogik.
- **Inputs:** `readMilliAmpere()`
- **Outputs:** `mean_current`
- **Timing:** 500 ms
- **Trace:** F3, NF3

#### C04 Wartungsmanagement (modelliert)
- **Aufgabe:** Berechnung der kumulierten Laufzeit im Zustand RUN; Aktivierung des Wartungsflags ab definierter Schwelle.
- **Inputs:** `runtime_increment`
- **Outputs:** `maintenance_due_flag`
- **Timing:** 1 s
- **Trace:** F4

#### C05 CSV-Protokollierung (modelliert)
- **Aufgabe:** 1-Hz-Protokollierung von Sollwert, Stromaufnahme und Systemstatus; Dateirotation nach Erreichen der Maximalgröße.
- **Inputs:** `Status`, `mean_current`
- **Outputs:** CSV-Dateieinträge
- **Timing:** 1 Hz
- **Trace:** F5, NF4, NF5

---

## Klassendiagramm

Das Klassendiagramm für Sprint 2 wurde vollständig bereinigt und entspricht nun der tatsächlichen Implementierung.
Es beinhaltet:

- korrekte Signaturen der `MainControlUnit` (`setUiSpeedCommandStep()`, `setSafetyInputs()`, `tick()`)
- reduzierte und reale Schnittstelle des `MotorActuator`
- Safety-State-Handling gemäß Implementierung (`SafetyState`)
- alle zukünftigen Module (UI, Monitoring, Maintenance, Logging) als weiterhin gültige Architekturbausteine

![Klassendiagramm](../referenziert/Design/Klassendiagramm_Sprint2.png)

---

## Sequenzdiagramm

Das Sequenzdiagramm zeigt die Abläufe folgender zentraler Szenarien:

- **Änderung der Geschwindigkeit** über UI → MainControlUnit → SetpointManager → MotorActuator  
- **Safety-Auslösung**: SafetyInput → MainControlUnit → Duty=0 %  
- **Monitoring & Logging (modelliert)**: MCU → MonitoringService → CsvLogger  

Diese Abläufe verdeutlichen die zeitliche Interaktion der Module.

![Sequenzdiagramm](../referenziert/Design/Sequenzdiagramm.png)

---

## Kommunikationsdiagramm

Das Kommunikationsdiagramm stellt die gleichen Abläufe wie das Sequenzdiagramm dar, allerdings strukturell:

- Welche Komponenten direkt interagieren  
- Welche Nachrichten in welcher Reihenfolge fließen  
- Welche Abhängigkeiten zwischen den Systemteilen existieren  

Durch diese Darstellung wird klar erkennbar, dass die `MainControlUnit` weiterhin die zentrale Kommunikationsinstanz ist.

![Kommunikationsdiagramm](../referenziert/Design/Kommunikationsdiagramm.png)

---

## Design Pattern

### Singleton-Pattern (unverändert)

Auch in Sprint 2 wird das Singleton für die `MainControlUnit` eingesetzt, um:

- eine einzige Instanz der zentralen Steuerlogik zu gewährleisten  
- einheitlichen Systemstatus zu sichern  
- konsistente Kommunikation zwischen UI, Safety, Motor und späteren Modulen zu ermöglichen  

Dies bleibt für ressourcenschwache Embedded-Systeme weiterhin das optimale Muster.
