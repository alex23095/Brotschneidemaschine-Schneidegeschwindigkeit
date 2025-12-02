# Sprint 2

### Sprint Planning

Zu Beginn des zweiten Sprints stand die Aufgabe im Mittelpunkt, die bereits umgesetzten Kernfunktionen aus Sprint 1 sauber in die geplante Gesamtarchitektur einzubinden.  
Dabei sollten Architektur, Klassendiagramm und Implementierung aufeinander abgestimmt und die strukturellen Grundlagen für die kommenden Erweiterungen gelegt werden.

Der Fokus lag daher auf:

- Anpassung des Klassendiagramms an den tatsächlichen Code  
- Harmonisierung der Schnittstellen zwischen den vorhandenen Kernkomponenten  
- Ergänzung der Architektur um die noch nicht implementierten, aber geplanten Module  
- Integration des Safety-Zustands in die Systemlogik  
- Vorbereitung des Systems für Monitoring, Wartung und Logging  

Die noch nicht umgesetzten Funktionsbereiche bleiben modelliert, damit sie in späteren Sprints nahtlos ergänzt werden können.

**Requirements:**

- **F1:** Erweiterung der Steuerlogik (Sollwert → Duty-Cycle)  
- **F2:** Einbindung des Safety-Zustands gemäß UML  
- **F3:** Struktur für Logging und Persistenz vorbereiten  
- **NF2:** Reaktionszeit weiterhin sicherstellen  
- **NF3:** Stabiler Zyklus über `tick()`  

**Sprintziel:**

1. Schaffung eines konsistenten, umsetzbaren Klassendiagramms  
2. Klare und einheitliche Schnittstellen der Kernmodule  
3. Architektur vollständig vorbereiten für kommende Module  
4. Aktualisierte und abgestimmte Architekturgrundlage für Sprint 3  

---

### Schritt 1: Analyse & Konsolidierung

Zu Beginn von Sprint 2 wurde geprüft, wie die bestehende Implementierung aus Sprint 1 optimal in das vollständige Systemdesign eingeordnet werden kann.  
Dabei ging es nicht darum, Fehler aus Sprint 1 aufzuarbeiten, sondern darum, **bestehende Funktionalität mit der Zielarchitektur zu verbinden** und das Modell so zu erweitern, dass zukünftige Features klar eingegliedert sind.

**Zentrale Ergebnisse der Konsolidierung:**

- Die vorhandenen Kernklassen (`MainControlUnit`, `SetpointManager`, `MotorActuator`, `SafetyInput`) wurden im Klassendiagramm verankert.  
- Der Safety-Mechanismus wurde mit einem systemweiten `SafetyState` integriert.  
- Schnittstellen wurden vereinheitlicht und an die reale Implementierung angepasst (z. B. `setUiSpeedCommandStep()`, `setDutyPercent()`).  
- Monitoring-, Maintenance- und Persistenz-Komponenten bleiben als Strukturbausteine bestehen und wurden logisch eingeordnet.  
- Das Gesamtdiagramm bildet nun sowohl die bestehende Funktionalität als auch die zukünftigen Erweiterungen konsistent ab.

Durch diese Konsolidierung wurde eine stabile, klare Basis geschaffen, auf der alle weiteren Sprint-Funktionen ohne strukturelle Brüche aufbauen können.

---

### Schritt 2: Architektur (Sprint 2)

Die Architektur folgt weiterhin einer vierteiligen Schichtenstruktur:

1. **User Interface Layer**  
   (`UserInterfaceService`, `LogViewer`)  
   Bedienung & Anzeige (noch nicht implementiert)

2. **Control Logic Layer**  
   (`MainControlUnit`, `SetpointManager`, `MonitoringService`, `MaintenanceManager`, `Status`)  
   Zentrale Steuerung, Sollwertverarbeitung, Zustandsverwaltung

3. **Hardware Abstraction Layer**  
   (`MotorActuator`, `CurrentSensor`, `SafetyInput`)  
   Duty-Ausgabe, Strommessung, Not-Halt-Erkennung

4. **Persistence Layer**  
   (`CsvLogger`, `FileDriver`)  
   Logging-Datenhaltung (noch nicht implementiert)

**Änderungen in Sprint 2:**

- Klassendiagramm vollständig bereinigt  
- SafetyState eingeführt  
- MotorActuator-Schnittstellen reduziert  
- MainControlUnit-Schnittstellen an echte Header-Dateien angepasst  
- Kardinalitäten ergänzt und Beziehungen korrigiert  

---

### Schritt 3: Design

Das aktualisierte Klassendiagramm für Sprint 2 zeigt:

- Alle existierenden Code-Klassen korrekt abgebildet  
- Alle späteren Module (Monitoring, Maintenance, Logging, UserInterface) weiterhin im Modell  
- Konsistente und reduzierte Schnittstellen  
- Klare Einbindung von Safety, Motor und Sensor  
- Zentrale Koordination über die MainControlUnit  

**Relevante Klassen in Sprint 2:**

| Komponente | Klasse | Aufgabe |
|-----------|--------|---------|
| **MCU** | `MainControlUnit` | Koordination aller Systembausteine, Sollwert- und Safety-Verarbeitung |
| **MCU** | `SetpointManager` | Quantisierung & Rampen für Sollwert |
| **MA** | `MotorActuator` | PWM/Duty-Cycle Ausgabe |
| **SI** | `SafetyInput` | Safety-Zustand (SAFE/TRIPPED) |
| **Sensor** | `CurrentSensor` | Strommessung |
| **Monitoring / Maintenance** | Modelle vorhanden, aber nicht implementiert |
| **Logging** | Modelle vorhanden, aber nicht implementiert |

