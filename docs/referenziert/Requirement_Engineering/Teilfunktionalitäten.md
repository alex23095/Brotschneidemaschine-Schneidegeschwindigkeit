# Software-Enigeering-Git
Übungsaufgaben Software-Engineering

# Teilfunktionalitäten des Systems

## 1. Sicherheitsfunktionen (F9, N3)
**Ziel:**  
Sicherstellung eines gefahrlosen Betriebs gemäß EN ISO 13849-1.  

**Beschreibung:**  
- Not-Halt/Bedienerschutz über zweikanalige Sicherheitssteuerung.  
- Überwachung der Reaktionszeit und Protokollierung der Ergebnisse.  
- Safe-State-Aktivierung bei Fehler oder Signalabweichung.  

---

## 2. Motorsteuerung (F1, F2)
**Ziel:**  
Regelung des Antriebsmotors über PWM-Signal.

**Beschreibung:**  
- Drehzahl einstellbar von 0–100 % in 10 %-Schritten.  
- Steuerung über Touchdisplay (manueller oder automatischer Modus).  
- Implementierung einer PWM-gesteuerten Motorregelung mit Soft-Start und -Stop.  

---

## 3. Sensorik & Belastungsanalyse (F3, F4)
**Ziel:**  
Erfassung der Schneidenbelastung und Berechnung der Restlebensdauer (RUL).

**Beschreibung:**  
- Messung der Stromaufnahme und Vibrationen während des Betriebs.  
- Verarbeitung der Signale per FFT-Analyse zur Ermittlung der Werkzeugbelastung.  
- Berechnung der Restlebensdauer anhand der gemessenen Belastungsdaten.  

--

## 4. Anzeige und Bedienung (F5)
**Ziel:**  
Automatische Information des Bedieners über anstehende Wartung.  

**Beschreibung:**  
- Anzeige eines Pop-ups im Display bei Erreichen der RUL-Grenze.  
- Speicherung des Wartungseintrags im EEPROM (auch nach Neustart erhalten).  
- Quittierung durch den Benutzer über das Touchdisplay.  

---

## 4. Fehlerauswertung /-speicher (F7, F8)
**Ziel:**  
Erfassen, Speichern und Exportieren von Betriebs- und Diagnosedaten.  

**Beschreibung:**  
- Speicherung der Betriebsdaten (z. B. Drehzahl, Belastung, Status) in CSV-Dateien.  
- Pufferung von bis zu 1000 Einträgen.  
- Export über USB-Stick bei Einstecken des Geräts.   

---

