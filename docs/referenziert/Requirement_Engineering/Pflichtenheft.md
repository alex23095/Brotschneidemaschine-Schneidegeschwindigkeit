# Pflichtenheft  
## Schneidegeschwindigkeit einer Brotschneidemaschine

---

## 1. Sicherheitseinrichtungen

### 1.1 Sicherheitsfunktionen
- Sicherheitseinrichtungen werden **redundant überwacht**:
  - **Not-Halt-Taster**: löst Motorbremse **innerhalb von 100 ms** aus.
  - **Schutzabdeckung**: löst Motorbremse **unmittelbar** aus.
- **Wiederanlaufsperre** verhindert unbeabsichtigtes Anlaufen nach Spannungswiederkehr.
- **Not-Halt-Funktion** über zweikanalige Sicherheitssteuerung gemäß **EN ISO 13849-1**.
- **Reaktionszeitmessung** erfolgt gemäß Sicherheitsnorm.
- **Sicherheitsprüfung und Zertifizierung** nach relevanten Maschinenrichtlinien.

---

## 2. Regelbare Drehzahl

### 2.1 Funktionsumfang
- **Manuelle Einstellung** der Soll-Drehzahl über das User-Interface im Bereich von **500 bis 3000 U/min**.
- Implementierung eines **PWM-gesteuerten Motorcontrollers**.
- **PID-Regler** sorgt für konstante Ist-Drehzahl mit maximaler Abweichung von **± 5 %** unter Last.
- **Soll- und Ist-Werte** werden kontinuierlich überwacht und am Display angezeigt.
- **Automatische Abschaltung** mit Fehlermeldung bei:
  - Überstrom  
  - Blockade  
  - Fehlfunktion

### 2.2 Benutzeroberfläche
- Drehzahlregelung über Touchdisplay (0–100 % in 10 %-Schritten).
- Anzeige der aktuellen und der Soll-Drehzahl in Echtzeit.
- Anzeige-Update-Rate ≤ 2 s (asynchrone UI-Aktualisierung).

---

## 3. Automatische Schneiden-Verschleiß-Überwachung

### 3.1 Datenerfassung
- Erfassung und Speicherung von:
  - **Laufzeit**
  - **Drehzahl**
  - **Schnittkraft** bei Bearbeitung
- Ableitung eines **Verschleißindex** der Schneide anhand dieser Daten.

### 3.2 Berechnungs- und Speicherlogik
- **Grenzwerte** für Verschleiß werden durch Tests ermittelt.
- **Zyklische Berechnung** des Verschleißindexes.
- Speicherung aller Daten im **nichtflüchtigen Speicher (EEPROM)**.
- **Algorithmus zur Restlebensdauerberechnung** basierend auf Belastungsdaten.

### 3.3 Benutzerinformation
- Ausgabe von **Warnmeldungen** und **Hinweisen** (z. B. Messerwechsel) am User-Interface.
- **Wartungsanzeige** als Pop-up im Display.
- **Wartungsempfehlungen** auf Basis der Restlebensdauer.

---

## 4. Anzeige und Bedienung

### 4.1 Display und UI
- 3,5-Zoll-Touchdisplay mit hygienischer Oberfläche.
- Anzeige von:
  - **Betriebszuständen**  
    - „System ok“ = grün  
    - „Warnungen“ = orange  
    - „Fehler / Safety“ = rot (blinkend bei Fehlermeldungen)  
  - **Drehzahlen**, **Sollwerte**, **Betriebsmodus** und **Sicherheitsstatus**
- **Reaktionszeit des User-Interfaces**: < 100 ms auf Eingaben.
- Bedienung auch **mit Handschuhen** möglich (kapazitives Touchpanel mit angepasster Empfindlichkeit).

### 4.2 Benutzerführung
- Menüstruktur mit Betriebsmodi:
  - **Automatik**
  - **Manuell**
  - **Wartung**
- **Intuitive Bedienung** durch klare Symbolik, große Buttons, Farbkennzeichnung.
- **Warn- und Sicherheitsmeldungen** werden optisch hervorgehoben.
- Asynchrone UI-Aktualisierung für schnelle Reaktionsfähigkeit.

---

## 5. Fehlerprotokoll

### 5.1 Funktionsumfang
- Aufzeichnung aller **sicherheits- und betriebsrelevanten Ereignisse**.
- Protokoll enthält:
  - **Zeitstempel**
  - **Fehlercode**
  - **Kurzbeschreibung**
- **Speichergröße:** mindestens **10 000 Einträge**.
- Speicherung im internen Speicher (FIFO-Prinzip).
- Darstellung des Fehlerprotokolls auf dem User-Interface.

### 5.2 Export
- **Exportfunktion über USB-Schnittstelle**:
  - Dateiformat: **CSV**
  - Unterstützte Datenträger: **FAT32**
  - **Speicherpuffer:** 1000 Einträge pro Exportvorgang
- **Log-File** enthält Zeit, Ereignistyp und Fehlertext.

---

## 6. Systemarchitektur (Kurzbeschreibung)

- **Mikrocontroller-Steuerung** mit PWM-Ausgang und Drehzahlerfassung.
- **Sensorik** für Stromaufnahme, Drehzahl und Vibrationsanalyse.
- **Touchdisplay** über SPI/I²C-Anbindung.
- **Nichtflüchtiger Speicher (EEPROM)** für Parameter und Protokolle.
- **USB-Schnittstelle** zur Datenübertragung und Firmware-Updates.

---

## 7. Zusammenfassung

| Bereich | Kernfunktion | Technische Umsetzung |
|----------|---------------|----------------------|
| Sicherheit | Not-Halt, Abdeckung, Wiederanlaufsperre | Zweikanalige Sicherheitssteuerung, Motorbremse ≤ 100 ms |
| Drehzahlregelung | 500–3000 U/min, PID-Regelung | PWM-Controller, Echtzeitüberwachung |
| Verschleißüberwachung | Laufzeit, Drehzahl, Schnittkraft | Algorithmus + Datenspeicherung im EEPROM |
| Anzeige & Bedienung | UI, Statusanzeigen, Touchdisplay | 3,5"-Display, Reaktionszeit < 100 ms |
| Fehlerprotokoll | Ereignis-Logging, Export | CSV-Datei via USB, FAT32-Unterstützung |
