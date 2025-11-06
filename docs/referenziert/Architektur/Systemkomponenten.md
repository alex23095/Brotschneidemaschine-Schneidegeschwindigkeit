# Systemkomponenten

| **Komponente** | **Bezeichnung** | **Hauptaufgabe / Funktion** | **Kommunikation mit** |
|----------------|------------------|------------------------------|------------------------|
| **MCU** | Main Control Unit / **Steuerungslogik** | Verwaltung der Betriebszustände (RUN/STOP/FAULT), Verarbeitung der Soll-Geschwindigkeit (10 %-Schritte), Koordination aller Komponenten, Zykluszeit 100 ms | UI, MA, CS, DM, SI |
| **UI** | User-Interface-Service | Entgegennahme von Bedienbefehlen (Start/Stop/Not-Halt, Soll-%); Anzeige von Soll-/Istwert, Wartungshinweis und Status | MCU, DM |
| **MA** | Motor-Aktuator | Umsetzung der Soll-Geschwindigkeit (Duty-Cycle) und sicheres Stillsetzen bei Not-Halt | MCU, SI, DM |
| **CS** | Current-Sensor / Monitoring | Erfassung der Motor-Stromaufnahme alle 500 ms; Gleitmittelwert (5 Samples) | MCU, DM |
| **DM** | Data-Management (Logger) | CSV-Protokollierung (1 Hz) von Zeit, Soll, Ist, Zustand und Strom; Roll-over bei 1 MB; Bereitstellung für Auswertung | MCU, UI, MA, CS |
| **SI** | Safety-Input | Aufnahme des Not-Halt-Signals; Weitergabe an MCU/MA zur sofortigen Abschaltung (≤ 100 ms) | MCU, MA |

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
     Motor-Aktuator   Current-     Data-
         (MA)         Sensor (CS)  Manager (DM)
            ▲                         ▲
            └──────── Safety-Input ───┘
                       (SI, Not-Halt)

---

# Systemschnittstellen mit Kommunikationssyntax

| **Schnittstelle (Quelle → Ziel)** | **Zweck / Datentyp** | **Kommunikationsart / Syntax** |
|----------------------------------|-----------------------|--------------------------------|
| **User Interface ↔ MCU** | Bedienbefehle (Start/Stop/Not-Halt), Soll-% (10 %-Schritte), Statusdaten | **Asynchron** (Ereignis-/Callback-API) |
| **MCU → MA** | Soll-% (Duty-Cycle), Freigabe/Sperre, Betriebszustand | **Synchron** (Funktionsaufruf / Treiber-API, 100 ms Zyklus) |
| **SI → MCU** | Not-Halt-Signal (digital) | **Sicherheitsgerichtet, synchron** (direkte Leitungsüberwachung) |
| **SI → MA** | Abschaltsignal (STO/Disable) | **Sicherheitsgerichtet, synchron** (direkte Hardwareabschaltung, ≤ 100 ms) |
| **CS → MCU** | Strommesswert alle 500 ms, Diagnoseflags | **Asynchron** (Interrupt/Callback oder DMA) |
| **MCU → DM** | Zustands- und Prozessdaten (1 Hz) | **Asynchron** (Message-Queue / Log-API) |
| **MA → DM** | Ist-% / Aktuatorstatus (1 Hz) | **Asynchron** (Log-API) |
| **CS → DM** | Strom-Messwerte (1 Hz) | **Asynchron** (Log-API) |
| **DM ↔ UI** | Anzeige von Logs / Exportstatus | **Asynchron** (Service-Aufruf / Event) |

---

### Hinweise zur Zuordnung

- **F1/F2:** MCU steuert MA; SI triggert Not-Halt mit Reaktionszeit ≤ 100 ms  
- **F3:** CS liefert alle 500 ms Messwerte, MCU bildet gleitenden Mittelwert (5 Werte)  
- **F4:** MCU zählt kumulierte RUN-Zeit, Wartungshinweis nach 48 h Betrieb  
- **F5 / NF4:** DM schreibt CSV (1 Hz), Datei-Rollover bei 1 MB  
- **NF2/NF3:** Reaktionszeit Befehle ≤ 200 ms, deterministische Zykluszeit 100 ms
