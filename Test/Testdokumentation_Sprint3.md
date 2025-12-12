# Sprint 3 – Testdokumentation

Diese Testdokumentation beschreibt alle durchgeführten Modul- und Integrationstests zur Überprüfung der in Sprint 3 implementierten Funktionalitäten.  
Der Schwerpunkt dieses Sprints lag auf dem CSV-Logging (1 Hz), der Nutzung von `std::filesystem`, der erweiterten Persistence-Logik sowie der Anbindung des UserInterface an den bestehenden Steuerfluss.

Alle Testfälle wurden manuell ausgeführt und dokumentiert.

**Zusätzlich wurden im Rahmen der Absicherung der Sprint-3-Implementierungen alle Testfälle aus Sprint 1 und Sprint 2 erneut vollständig durchgeführt (Regressionstest), um sicherzustellen, dass bestehende Funktionalitäten durch die Erweiterungen in Sprint 3 nicht beeinträchtigt wurden.**

---

## 1. Modul-Testfälle

---

### **TC-M7 – CsvLogger: Erzeugen einer neuen CSV-Datei**
- **Ziel:** Prüfung der initialen CSV-Dateierstellung  
- **Modul:** CsvLogger  
- **Funktion:** `initLogFile()`  
- **Vorbedingung:** Zielverzeichnis existiert, keine CSV-Datei vorhanden  
- **Aktion:** Initialisierung des Loggers  
- **Erwartetes Ergebnis:** CSV-Datei wird erzeugt und enthält einen gültigen Header  
- **Nachbedingungen:** leere, konsistente CSV-Datei vorhanden  
- **Durchgeführt:** Ja  
- **Testergebnis:** OK  
- **Bemerkung:** Datei korrekt angelegt

---

### **TC-M8 – CsvLogger: Periodisches Schreiben (1 Hz)**
- **Ziel:** Sicherstellen des zeitgesteuerten CSV-Loggings  
- **Modul:** CsvLogger  
- **Funktion:** `logTick()`  
- **Vorbedingung:** Logger initialisiert  
- **Aktion:** Mehrfache Aufrufe im 1-Sekunden-Intervall  
- **Erwartetes Ergebnis:** Pro Sekunde wird ein neuer CSV-Datensatz geschrieben  
- **Nachbedingungen:** Datei wächst konsistent ohne Datenverlust  
- **Durchgeführt:** Ja  
- **Testergebnis:** OK  
- **Bemerkung:** Zeitverhalten entspricht Spezifikation

---

### **TC-M9 – PersistenceManager: Datei-Existenzprüfung**
- **Ziel:** Validierung der Nutzung von `std::filesystem::exists()`  
- **Modul:** PersistenceManager  
- **Funktion:** `checkLogFile()`  
- **Vorbedingung:** CSV-Datei existiert / existiert nicht  
- **Aktion:** Aufruf der Existenzprüfung  
- **Erwartetes Ergebnis:** korrekter boolescher Rückgabewert  
- **Nachbedingungen:** keine Seiteneffekte  
- **Durchgeführt:** Ja  
- **Testergebnis:** OK  
- **Bemerkung:** Dateisystemzugriff stabil

---

### **TC-M10 – UserInterface: Übergabe von UI-Sollwerten**
- **Ziel:** Prüfung der UI-Anbindung an die MainControlUnit  
- **Modul:** UserInterface  
- **Funktion:** `setUiSpeedCommandStep()`  
- **Vorbedingung:** Systemzustand SAFE  
- **Aktion:** UI setzt neuen Sollwert (10-%-Schritt)  
- **Erwartetes Ergebnis:** Sollwert wird von der MCU übernommen  
- **Nachbedingungen:** SetpointManager aktualisiert Zielwert  
- **Durchgeführt:** Ja  
- **Testergebnis:** OK  
- **Bemerkung:** UI-Fluss korrekt integriert

---

## 2. Integrations-Testfälle

---

### **TC-I7 – MCU + CsvLogger: Logging während Regelbetrieb**
- **Ziel:** Überprüfung des CSV-Loggings im laufenden Regelbetrieb  
- **Module:** MainControlUnit, CsvLogger  
- **Vorbedingung:** ui_step10 = 5, System SAFE  
- **Aktion:** Mehrere `tick()`-Zyklen  
- **Erwartetes Ergebnis:** Aktuelle Regelgrößen werden korrekt geloggt  
- **Nachbedingungen:** CSV-Datei konsistent, keine Aussetzer  
- **Testergebnis:** OK  
- **Bemerkung:** Logging stabil im Betrieb

---

### **TC-I8 – SafetyInput + CSV: Not-Halt während aktivem Logging**
- **Ziel:** Verhalten des CSV-Loggings bei Safety TRIPPED  
- **Module:** SafetyInput, MainControlUnit, CsvLogger  
- **Vorbedingung:** Logging aktiv, ui_step10 > 0  
- **Aktion:** SafetyState → TRIPPED  
- **Erwartetes Ergebnis:**  
  - `rpm_target = 0`  
  - `duty = 0`  
  - Logging läuft weiter und dokumentiert den Zustand  
- **Nachbedingungen:** CSV-Datei nicht beschädigt  
- **Testergebnis:** OK  
- **Bemerkung:** Robust gegen Safety-Ereignisse

---

### **TC-I9 – UserInterface → MCU → CsvLogger (End-to-End)**
- **Ziel:** Validierung des vollständigen Steuer- und Logging-Flows  
- **Module:** UserInterface, MainControlUnit, SetpointManager, CsvLogger  
- **Vorbedingung:** System SAFE  
- **Aktion:** UI-Sollwertänderung, mehrere Regelzyklen  
- **Erwartetes Ergebnis:**  
  - Setpoint folgt UI-Vorgabe  
  - Duty folgt Rampenlogik  
  - CSV-Daten sind konsistent und plausibel  
- **Nachbedingungen:** stabiler Systemzustand  
- **Testergebnis:** OK  
- **Bemerkung:** End-to-End-Fluss korrekt

---

## 3. Regressionstests (Sprint 1 & Sprint 2)

Im Rahmen von Sprint 3 wurden **alle Modul- und Integrationstests aus Sprint 1 und Sprint 2** erneut durchgeführt.

- **Ziel:** Sicherstellung der Abwärtskompatibilität  
- **Ergebnis:** Alle Testfälle erneut **OK**  
- **Bemerkung:** Keine Regressionen durch Sprint-3-Erweiterungen festgestellt

---

## **Zusammenfassung Sprint 3**

| Test-ID | Ergebnis |
|--------|----------|
| TC-M7 | OK |
| TC-M8 | OK |
| TC-M9 | OK |
| TC-M10 | OK |
| TC-I7 | OK |
| TC-I8 | OK |
| TC-I9 | OK |
| Regression Sprint 1 & 2 | OK |

---

## Fazit

Die in Sprint 3 implementierten Funktionen – insbesondere CSV-Logging, Dateisystem-Nutzung und UI-Anbindung – wurden vollständig getestet und erfüllen die definierten Anforderungen.  
Durch die erneute Durchführung aller Tests aus Sprint 1 und Sprint 2 konnte sichergestellt werden, dass keine Regressionen oder Seiteneffekte entstanden sind.
