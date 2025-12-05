# Sprint 2 – Testdokumentation

Diese Testdokumentation beschreibt alle durchgeführten Modul- und Integrationstests zur Überprüfung der in Sprint 2 implementierten Funktionalitäten.  
Der Fokus dieses Sprints lag auf der erweiterten Safety-Logik, der verbesserten Rampenfunktion des SetpointManagers sowie der robusteren MotorActuator-Begrenzung.  
Alle Testfälle wurden manuell ausgeführt und dokumentiert.

---

## 1. Modul-Testfälle

---

### **TC-M4 – MainControlUnit: Safety-Reaktion bei TRIPPED**
- **Ziel:** Sofortige Rücksetzung von Sollwert und Duty bei Safety TRIPPED  
- **Modul:** MainControlUnit  
- **Funktionen:** `setSafetyInputs()`, `tick()`  
- **Vorbedingung:** MCU aktiv, `ui_step10 = 5` gesetzt  
- **Aktion:** SafetyState → TRIPPED  
- **Erwartetes Ergebnis:** `rpm_target = 0`, `duty = 0`  
- **Nachbedingungen:**  
  - SetpointManager Target auf 0  
  - MotorActuator Duty auf 0  
  - interne Safety-Flags gesetzt  
- **Durchgeführt:** Ja  
- **Testergebnis:** OK  
- **Bemerkung:** Sofortige Abschaltung korrekt

---

### **TC-M5 – SetpointManager: neues Rampenverhalten Sprint 2**
- **Ziel:** Prüfung der erweiterten Rampenlogik  
- **Modul:** SetpointManager  
- **Funktion:** `calcNext()`  
- **Vorbedingung:** `current = 5`, `target = 8`  
- **Aktion:** Berechnung nächster Rampenwert  
- **Erwartetes Ergebnis:** neue Schrittweite gemäß Spezifikation (z. B. +2)  
- **Nachbedingungen:** korrekter interner Rampenzustand  
- **Durchgeführt:** Ja  
- **Testergebnis:** OK  
- **Bemerkung:** neue Rampenlogik erfüllt Anforderungen

---

### **TC-M6 – MotorActuator: Duty-Limitierung Sprint 2**
- **Ziel:** Prüfung der neuen Begrenzungslogik (z. B. MaxDuty)  
- **Modul:** MotorActuator  
- **Funktion:** `calcDuty()`  
- **Vorbedingung:** Setpoint über zulässigem Bereich  
- **Aktion:** `calcDuty(overshoot_value)`  
- **Erwartetes Ergebnis:** Duty wird auf MaxDuty limitiert  
- **Nachbedingungen:** keine Schutzfunktionen ausgelöst  
- **Durchgeführt:** Ja  
- **Testergebnis:** OK  
- **Bemerkung:** Limitierung greift wie vorgesehen

---

## 2. Integrations-Testfälle

---

### **TC-I4 – SafetyInput + MCU + MotorActuator: Not-Halt-Kette**
- **Ziel:** Überprüfung der vollständigen Abschaltkette bei TRIPPED  
- **Module:** SafetyInput, MCU, MotorActuator  
- **Vorbedingung:** ui_step10 = 4  
- **Aktion:** SafetyState → TRIPPED, dann `tick()`  
- **Erwartetes Ergebnis:** Setpoint 0, Duty 0  
- **Nachbedingungen:** kein Rampenfortschritt, MotorActuator stabil  
- **Testergebnis:** OK  
- **Bemerkung:** Gesamtkette funktioniert zuverlässig

---

### **TC-I5 – MCU: Übergang SAFE → TRIPPED → SAFE**
- **Ziel:** korrekte Verhaltensweise bei Sequenzwechsel  
- **Module:** MCU, SafetyInput, SetpointManager  
- **Vorbedingung:** ui_step10 = 3  
- **Aktion:**  
  1. Wechsel zu TRIPPED  
  2. anschließend zurück zu SAFE  
- **Erwartetes Ergebnis:**  
  - Schritt 1: sofort rpm_target = 0  
  - Schritt 2: Wiederanlauf über Rampenlogik  
- **Nachbedingungen:** konsistente Zustandsverwaltung  
- **Testergebnis:** OK  
- **Bemerkung:** Zustandswechsel fehlerfrei

---

### **TC-I6 – UI-Eingabe + SetpointManager + MotorActuator (neuer Steuerfluss)**
- **Ziel:** Validierung des neuen End-to-End-Flows  
- **Module:** MCU, SetpointManager, MotorActuator  
- **Vorbedingung:** MCU aktiv  
- **Aktion:** ui_step10 = 7, mehrere `tick()`  
- **Erwartetes Ergebnis:**  
  - Setpoint nähert sich zielgerecht  
  - Duty folgt neuer Rampenlogik  
- **Nachbedingungen:** stabiler Systemzustand  
- **Testergebnis:** OK  
- **Bemerkung:** kontinuierliche Steuerkette korrekt

---

## **Zusammenfassung Sprint 2**

| Test-ID | Ergebnis |
|--------|----------|
| TC-M4  | OK |
| TC-M5  | OK |
| TC-M6  | OK |
| TC-I4  | OK |
| TC-I5  | OK |
| TC-I6  | OK |

---

## Fazit

Die in Sprint 2 umgesetzten Funktionalitäten wurden vollständig getestet und erfüllen die definierten Anforderungen.  
Insbesondere die neue Safety-Logik, die Erweiterungen der Rampenfunktion sowie die Duty-Limitierung zeigen ein stabiles, konsistentes und erwartungskonformes Verhalten.  
Damit ist die Grundlage für weitere Erweiterungen und Sprint-3-Funktionalitäten technisch solide vorbereitet.

