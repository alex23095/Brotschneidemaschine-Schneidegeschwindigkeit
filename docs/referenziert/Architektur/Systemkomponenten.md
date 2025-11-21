# Systemkomponenten

| **Komponente** | **Bezeichnung** | **Hauptaufgabe / Funktion** | **Kommunikation mit** |
|----------------|------------------|------------------------------|------------------------|
| **MCU** | Main Control Unit / **Steuerungslogik** | Zustandsverwaltung (RUN/STOP/FAULT), Sollwertverarbeitung (10 %-Stufen), 100-ms-Zyklus, Koordination aller Komponenten | UI, MA, CS, PM, SI |
| **UI** | User-Interface-Service | Bedienbefehle (Start/Stop/E-Stop, Soll-%), Anzeige von Ist-/Sollwerten, Status, Wartung | MCU, PM |
| **MA** | Motor-Aktuator | Umsetzung Soll-Duty-Cycle, Motorfreigabe/Sperre, sicherer Not-Halt | MCU, SI, PM |
| **CS** | Current-Sensor | Strommessung alle 500 ms, Bereitstellung der Rohwerte | MCU, PM |
| **PM** | PersistenceManager (Logger & Wartung) | CSV-Logging (1 Hz), Datei-Rollover (1 MB), Wartungszähler, Bereitstellung von Logdaten | MCU, UI, MA, CS |
| **SI** | Safety-Input | Not-Halt-Erkennung, Weitergabe an MCU sowie Hardware-Stopp | MCU, MA |

---

### Übersicht (vereinfachte Architektur)

         ┌────────────────────┐
         │   User Interface   │
         │        (UI)        │
         └─────────┬──────────┘
                   │  Bedienung/Anzeige
┌──────────────────┴──────────────────┐
│      Main Control Unit (MCU)        │
│     Steuerungslogik, 100 ms Zyklus  │
└───────┬───────────┬───────────┬─────┘
        │           │           │
        │           │           │
       ▼            ▼           ▼
 Motor-Aktuator   Current-     Persistence-
     (MA)         Sensor (CS)  Manager (PM)
        ▲                         ▲
        └──────── Safety-Input ───┘
                   (SI, Not-Halt)

---

# Systemschnittstellen mit Kommunikationssyntax

| **Schnittstelle (Quelle → Ziel)** | **Zweck / Datentyp** | **Kommunikationsart / Syntax** |
|----------------------------------|------------------------|--------------------------------|
| **User Interface ↔ MCU** | Start/Stop/E-Stop, Soll-%, Statusdaten | **Asynchron** (Event/Callback) |
| **MCU → MA** | Soll-Duty-Cycle, Freigabe/Sperre | **Synchron** (Funktionsaufruf, 100-ms-Tick) |
| **SI → MCU** | Not-Halt-Signal | **Sicherheitsgerichtet, synchron** |
| **SI → MA** | Sofortige Hardware-Abschaltung | **Sicherheitsgerichtet, synchron (≤100 ms)** |
| **CS → MCU** | Strommesswert (500 ms) | **Asynchron** (Interrupt/DMA oder Polling) |
| **MCU → PM** | Zustandsdaten für Logging (1 Hz) | **Asynchron** (Log-API) |
| **MA → PM** | Ist-Drehzahl / Aktuatorstatus (1 Hz) | **Asynchron** (Log-API) |
| **CS → PM** | Stromwerte (1 Hz) | **Asynchron** (Log-API) |
| **PM ↔ UI** | Anzeige von Logs, Wartungsstatus | **Asynchron** (Service-Aufruf) |

---

### Hinweise zur Zuordnung

- **F1/F2:** MCU steuert MA; SI erzwingt Not-Halt  
- **F3:** CS liefert Stromwerte (500 ms)  
- **F4:** PM verwaltet Betriebszeit → MCU zeigt Wartung an  
- **F5 / NF4:** PM schreibt CSV (1 Hz), Rollover bei 1 MB  
- **NF2/NF3:** Befehlsreaktion ≤200 ms; MCU arbeitet deterministisch (100-ms-Zyklus)
