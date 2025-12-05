# Sprint 1 – Testdokumentation

Dieses Dokument protokolliert die 6 definierten Testfälle aus Sprint 1.  
Alle Tests wurden manuell ausgeführt. Das Ergebnis jedes Testfalls ist unten vermerkt  
(OK = bestanden, NOK = fehlgeschlagen).

---

## 1. Modul-Testfälle

---

### **TC-M1 – SafetyInput: gültiger Wert**

**Ziel:** Prüfen, ob ein gültiger Geschwindigkeitswert akzeptiert wird  
**Modul:** SafetyInput  
**Funktion:** `checkSpeed(int value)`  

**Vorbedingung:** System initialisiert  
**Aktion:** `checkSpeed(2)`  
**Erwartetes Ergebnis:** Rückgabewert = `true`  

**Nachbedingungen:**  
- `checkSpeed(2)` liefert `true`  
- interne Validierung abgeschlossen  
- Flags in `SafetyInput::Inputs` unverändert  
- keine Fallback- oder Fehler-Methoden aktiv  

**Durchgeführt:** Ja  
**Testergebnis:** OK  
**Bemerkung:** Wert 2 liegt im erlaubten Bereich  

---

### **TC-M2 – SafetyInput: Wert zu groß**

**Ziel:** Prüfen, ob ein zu großer Wert abgelehnt wird  
**Modul:** SafetyInput  
**Funktion:** `checkSpeed(int value)`  

**Vorbedingung:** System initialisiert  
**Aktion:** `checkSpeed(10)`  
**Erwartetes Ergebnis:** Rückgabewert = `false`  

**Nachbedingungen:**  
- `checkSpeed(10)` liefert `false`  
- Fehlerstatus über interne Logik gesetzt (z. B. `setInvalidFlag()`)  
- keine Weitergabe an Folgemethoden  
- Bereichsgrenzenprüfung abgeschlossen  

**Durchgeführt:** Ja  
**Testergebnis:** OK  
**Bemerkung:** Wert 10 wurde korrekt als ungültig erkannt  

---

### **TC-M3 – SetpointManager: Rampenfunktion**

**Ziel:** Prüfen der einfachen Rampenlogik  
**Modul:** SetpointManager  
**Funktion:** `calcNext(int current, int target)`  

**Vorbedingung:** `current = 1`, `target = 3`  
**Aktion:** `calcNext(1, 3)`  
**Erwartetes Ergebnis:** Rückgabewert = `2`  

**Nachbedingungen:**  
- `calcNext(1, 3)` berechnet neuen Wert (`2`)  
- Rampensteigerung intern korrekt von `1 → 2`  
- keine Rücksetzung über `resetRamp()` o. Ä.  
- Zustand des Moduls konsistent  

**Durchgeführt:** Ja  
**Testergebnis:** OK  
**Bemerkung:** Rampensteigerung um +1 korrekt  

---

## 2. Integrations-Testfälle

---

### **TC-I1 – SafetyInput + SetpointManager**

**Ziel:** Validierung + Rampenberechnung in Kombination  
**Module:** SafetyInput, SetpointManager  

**Vorbedingung:** Beide Module initialisiert  
**Aktion:**  
1. `checkSpeed(3)`  
2. Bei `true`: `calcNext(1, 3)`  

**Erwartetes Ergebnis:**  
- Schritt 1: `true`  
- Schritt 2: Rückgabewert `2`  

**Nachbedingungen:**  
- `checkSpeed(3)` erfolgreich (`true`)  
- `calcNext(1, 3)` liefert korrekten Rampenwert  
- Datenfluss SafetyInput → SetpointManager korrekt  
- keine Fehlerbehandlung ausgelöst  

**Durchgeführt:** Ja  
**Testergebnis:** OK  
**Bemerkung:** Datenfluss korrekt  

---

### **TC-I2 – SetpointManager + MotorActuator**

**Ziel:** Übergabe des Sollwerts an den Aktuator  
**Module:** SetpointManager, MotorActuator  

**Vorbedingung:** `current = 0`, `target = 3`  
**Aktion:**  
1. `calcNext(0, 3)` → ergibt `1`  
2. `calcDuty(1)`  

**Erwartetes Ergebnis:**  
- Schritt 1: `1`  
- Schritt 2: Beispielwert (z. B. 20 %)  

**Nachbedingungen:**  
- berechneter Setpoint aus `calcNext()` übernommen (`1`)  
- Duty-Cycle korrekt aus Setpoint durch `calcDuty(1)` bestimmt  
- kein Aufruf von Schutzfunktionen (`limitDuty()`, `safetyStop()`)  
- MotorActuator in stabilem Zustand  

**Durchgeführt:** Ja  
**Testergebnis:** OK  
**Bemerkung:** MotorDuty korrekt aus berechnetem Setpoint  

---

### **TC-I3 – MainControlUnit + SafetyInput**

**Ziel:** Prüfung der Wertübernahme im Hauptsystem  
**Module:** MainControlUnit, SafetyInput  

**Vorbedingung:** MCU aktiviert, keine Fehler  
**Aktion:**  
1. `userInput = 2`  
2. `ok = checkSpeed(2)`  
3. `usedSetpoint = ok ? 2 : 0`  

**Erwartetes Ergebnis:**  
- `ok == true`  
- `usedSetpoint == 2`  

**Nachbedingungen:**  
- `checkSpeed(2)` liefert `true`  
- MCU übernimmt Wert über interne Logik (z. B. `setUiSpeedCommandStep(2)`)  
- Weitergabe an SetpointManager erfolgt korrekt (`setTargetRpm()`)  
- keine Fehlertrigger durch `setSafetyInputs()`  
- MCU-Zustand konsistent aktualisiert  

**Durchgeführt:** Ja  
**Testergebnis:** OK  
**Bemerkung:** Wert korrekt übernommen  

---

# Zusammenfassung

| Test-ID | Ergebnis |
|--------|----------|
| TC-M1  | OK |
| TC-M2  | OK |
| TC-M3  | OK |
| TC-I1  | OK |
| TC-I2  | OK |
| TC-I3  | OK |

Alle 6 Testfälle wurden erfolgreich ausgeführt und bestanden.

---
