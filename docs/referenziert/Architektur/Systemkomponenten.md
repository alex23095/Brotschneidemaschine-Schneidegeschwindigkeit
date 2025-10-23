**Systemkomponenten**

- MC = Motor-Controller
- UI = User-Interface-Service
- SM = Sensor-Modul
- MS = Monitoring-Service
- DM = Data-Management
- SCU = Safety-Control-Unit

**Systemschnittstellen**
| Schnittstelle (Quelle → Ziel) | Zweck / Datentyp |
|----------------|------------------|
| **User Interface ↔ Main Control Unit** | Sollwerte, Betriebsmodus, Benutzeraktionen, Statusdaten |
| **Main Control Unit ↔ Motor Controller** | Drehzahlsollwert, Start/Stop, Statusrückmeldung |
| **Main Control Unit ↔ Sensor Modul** | Messdaten (Strom, Vibration), Diagnoseflags |
| **Sensor Modul ↔ Monitoring Service** | Aufbereitete Messwerte, Belastungsdaten |
| **Monitoring Service ↔ Main Control Unit** | Restlebensdauer, Wartungsempfehlung |
| **Monitoring Service ↔ User Interface** | Wartungshinweis, Lebensdaueranzeige |
| **Main Control Unit ↔ Safety Control Unit** | Betriebsstatus, Not-Halt-Signal |
| **Safety Control Unit ↔ Motor Controller** | Safe Torque Off, Freigabe / Sperre |
| **Main Control Unit ↔ Data Management** | Ereignisse, Fehlermeldungen, Betriebsdaten |
| **Data Management ↔ User Interface** | Anzeige von Logs, Export-Status | API /
| **Data Management ↔ Externer Datenträger (USB)** | CSV-Dateien, Log-Export |
