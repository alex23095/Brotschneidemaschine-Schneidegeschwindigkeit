# Systemkomponenten

| **Komponente** | **Bezeichnung** | **Hauptaufgabe / Funktion** | **Kommunikation mit** |
|----------------|----------------|------------------------------|------------------------|
| **MC** | Motor-Controller | Regelt den Schneidemotor (Drehzahl, Start/Stop, Sicherheitsabschaltung). | MCU, SCU |
| **UI** | User-Interface-Service | Visualisiert Systemzustände, ermöglicht Benutzereingaben und Anzeige von Wartungshinweisen. | MCU, MS, DM |
| **SM** | Sensor-Modul | Erfasst physikalische Messgrößen (Strom, Vibration) und liefert Rohdaten. | MCU, MS |
| **MS** | Monitoring-Service | Analysiert Sensordaten, berechnet Schneidenverschleiß und Restlebensdauer, erzeugt Wartungsempfehlungen. | SM, MCU, UI |
| **DM** | Data-Management | Speichert, verwaltet und exportiert Logdaten, Fehlerprotokolle und CSV-Dateien über USB. | MCU, UI, Externer Datenträger |
| **SCU** | Safety-Control-Unit | Überwacht sicherheitsrelevante Zustände (z. B. Not-Halt) und steuert Safe-Torque-Off-Signale. | MCU, MC |

                 ┌────────────────────┐
                 │   User Interface   │
                 │        (UI)        │
                 └─────────┬──────────┘
                           │
                           │ Benutzerinteraktion / Anzeige
                           │
        ┌──────────────────┴──────────────────┐
        │          Main Control Unit          │
        │             (MCU/Logik)             │
        └──────────────────┬──────────────────┘
                           │
      ┌──────────┬─────────┼───────────┬────────────┐
      │          │         │           │            │
      ▼          ▼         ▼           ▼            ▼
      MC         SM        MS          DM          SCU  
  

# Systemschnittstellen mit Kommunikationssyntax

| **Schnittstelle (Quelle → Ziel)** | **Zweck / Datentyp** | **Kommunikationsart / Syntax** |
|----------------------------------|-----------------------|--------------------------------|
| **User Interface ↔ Main Control Unit** | Sollwerte, Betriebsmodus, Benutzeraktionen, Statusdaten | **Asynchron** (Event-getriebene Kommunikation über GUI-Framework / IPC) |
| **Main Control Unit ↔ Motor Controller** | Drehzahlsollwert, Start/Stop, Statusrückmeldung | **Synchron** (serielle Kommunikation, z. B. UART/CAN mit zyklischer Abfrage) |
| **Main Control Unit ↔ Sensor Modul** | Messdaten (Strom, Vibration), Diagnoseflags | **Asynchron** (Datenstrom per I²C/SPI-Bus, Interrupt- oder DMA-basiert) |
| **Sensor Modul ↔ Monitoring Service** | Aufbereitete Messwerte, Belastungsdaten | **Asynchron** (Publisher/Subscriber-Pattern, internes Messaging) |
| **Monitoring Service ↔ Main Control Unit** | Restlebensdauer, Wartungsempfehlung | **Synchron** (Funktionsaufruf / Shared Memory Zugriff) |
| **Monitoring Service ↔ User Interface** | Wartungshinweis, Lebensdaueranzeige | **Asynchron** (Event-Nachricht über UI-Controller oder MQTT-intern) |
| **Main Control Unit ↔ Safety Control Unit** | Betriebsstatus, Not-Halt-Signal | **Sicherheitsgerichtet, synchron** (zweikanalige Hardwareverbindung, z. B. 24 V I/O mit Diagnose) |
| **Safety Control Unit ↔ Motor Controller** | Safe Torque Off, Freigabe / Sperre | **Sicherheitsgerichtet, synchron** (direkte Hardwareabschaltung / STO-Signal) |
| **Main Control Unit ↔ Data Management** | Ereignisse, Fehlermeldungen, Betriebsdaten | **Asynchron** (Interprozesskommunikation, FIFO oder Message Queue) |
| **Data Management ↔ User Interface** | Anzeige von Logs, Export-Status | **Asynchron** (REST-/Local API oder Shared Memory Event-Interface) |
| **Data Management ↔ Externer Datenträger (USB)** | CSV-Dateien, Log-Export | **Synchron / blockierend** (Dateisystemzugriff über FAT32-Treiber) |

