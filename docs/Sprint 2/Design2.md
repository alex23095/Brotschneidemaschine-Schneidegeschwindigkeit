# Design – Sprint 2

## Ziel von Sprint 2 im Software-Design

Sprint 2 fokussiert sich auf die **Präzisierung, Entkopplung und Stabilisierung**
der bereits eingeführten Architektur.  
Im Gegensatz zu Sprint 1, der die funktionale Basis gelegt hat, werden in
Sprint 2 Verantwortlichkeiten klar getrennt, Schnittstellen geschärft und
sicherheitsrelevante Abläufe explizit modelliert.

---

## Design-Zerlegung

### MCU – Main Control Unit

#### C01 Sollwert- und Drehzahlsteuerung (funktional erweitert)

- **Aufgabe:**  
  Verarbeitung der UI-Sollwertvorgabe in 10-%-Schritten über
  `setUiSpeedCommandStep()`.  
  Entkopplung zwischen Benutzereingabe, Sollwertaufbereitung
  (`SetpointManager`) und Aktuatoransteuerung (`MotorActuator`).

- **Sprint-2-Erweiterungen gegenüber Sprint 1:**  
  - klare Trennung von UI-Eingabe und internem Drehzahlsollwert  
  - definierte Übergabeschnittstelle zum `SetpointManager`  
  - Vorbereitung einer Rampen- und Glättungslogik im Design  

- **Inputs:** `ui_step10`, `safety_state`  
- **Outputs:** `rpm_target`, `duty_target`  
- **Timing:** zyklisch über `tick()` (≈ 100 ms)  
- **Trace:** F1, F2, NF2, NF3  

---

#### C02 Not-Halt / Safety-Verarbeitung (verhaltenslogisch präzisiert)

- **Aufgabe:**  
  Zentrale Auswertung des Safety-Zustands (`SAFE` / `TRIPPED`) und
  deterministische Durchsetzung eines sicheren Systemzustands.

- **Sprint-2-Erweiterungen gegenüber Sprint 1:**  
  - Safety-Ereignisse überschreiben alle aktiven Sollwerte  
  - definierter Systemzustand `TRIPPED` mit eindeutigem Ausgang  
  - Safety ist kein Nebencheck mehr, sondern ein dominanter Steuerpfad  

- **Inputs:** `SafetyInput::getState()`  
- **Outputs:** `rpm_target = 0`, `duty_target = 0`, `system_fault`  
- **Timing:** ereignisnah, wirksam im nächsten `tick()`  
- **Trace:** F2, NF2  

---

#### C06 Systemlogik und Statusbereitstellung (neu strukturiert)

- **Aufgabe:**  
  Zentrale Aggregation des Systemzustands zur Bereitstellung für UI,
  Monitoring und spätere Protokollierung.

- **Sprint-2-Erweiterungen:**  
  - Bündelung von Sollwert, Istwert und Systemzustand  
  - klare Trennung zwischen Steuerlogik (C01/C02) und Statushaltung  
  - definierte Ausgangsschnittstelle für spätere Services  

- **Inputs:** `rpm_target`, `current_measurement`, `safety_state`  
- **Outputs:** `SystemStatus`  
- **Timing:** zyklisch  
- **Trace:** NF3  

---

## CS / PM – Monitoring, Wartung und Protokollierung  
*(in Sprint 2 neu modelliert, noch nicht implementiert)*

#### C03 Monitoring-Service (neu modelliert)

- **Aufgabe:**  
  Mittelwertbildung der Stromaufnahme über definierte Zeitfenster
  zur späteren Diagnose und Zustandsbewertung.

- **Inputs:** `readMilliAmpere()`  
- **Outputs:** `mean_current`  
- **Timing:** 500 ms  
- **Trace:** F3, NF3  

---

#### C04 Wartungsmanagement (neu modelliert)

- **Aufgabe:**  
  Erfassung der kumulierten Laufzeit im RUN-Zustand und Ableitung
  eines Wartungsbedarfs.

- **Inputs:** `runtime_increment`  
- **Outputs:** `maintenance_due_flag`  
- **Timing:** 1 s  
- **Trace:** F4  

---

#### C05 CSV-Protokollierung (neu modelliert)

- **Aufgabe:**  
  Zeitbasierte Protokollierung zentraler Systemdaten mit
  Dateirotationskonzept.

- **Inputs:** `SystemStatus`, `mean_current`  
- **Outputs:** CSV-Dateieinträge  
- **Timing:** 1 Hz  
- **Trace:** F5, NF4, NF5  

---

## Klassendiagramm

Das Klassendiagramm wurde in Sprint 2 **bereinigt und an die reale
Implementierung angepasst**:

- reduzierte Schnittstellen ohne konzeptionelle Platzhalter  
- korrekte Methodensignaturen der `MainControlUnit`  
- explizites Safety-State-Handling  
- zukünftige Module weiterhin als gültige Architekturbausteine modelliert  

![Klassendiagramm](../referenziert/Design/Klassendiagramm_Sprint2.png)

---

## Sequenzdiagramm

Das Sequenzdiagramm verdeutlicht zentrale Sprint-2-Szenarien:

- UI-Sollwertänderung mit entkoppelter Sollwertaufbereitung  
- deterministische Safety-Auslösung mit Priorität  
- vorbereitete Monitoring- und Logging-Kette  

![Sequenzdiagramm](../referenziert/Design/Sequenzdiagramm.png)

---

## Kommunikationsdiagramm

Das Kommunikationsdiagramm zeigt die strukturellen Abhängigkeiten
zwischen den Komponenten und bestätigt die `MainControlUnit`
als zentrale Koordinationsinstanz.

![Kommunikationsdiagramm](../referenziert/Design/Kommunikationsdiagramm.png)

---

## Design Pattern

### Singleton – bewusste Beibehaltung

Das Singleton-Pattern für die `MainControlUnit` wurde in Sprint 2
bewusst beibehalten:

- eindeutiger Systemzustand  
- deterministisches Verhalten  
- geringe Komplexität im Embedded-Kontext  

Die Stabilität dieses Entwurfs ist Teil der Designentscheidung.
