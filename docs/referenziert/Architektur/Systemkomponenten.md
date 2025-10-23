**Systemkomponenten**

- MC = Motor-Controller
- UI = User-Interface-Service
- SM = Sensor-Modul
- MS = Monitoring-Service
- DM = Data-Management
- SCU = Safety-Control-Unit

**Systemschnittstellen**
| Schnittstelle | Quelle → Ziel | Zweck / Datentyp |
|----------------|---------------|------------------|
| **User Interface ↔ Hauptsteuerung** | User Interface ↔ Main Control Unit | Sollwerte, Betriebsmodus, Benutzeraktionen, Statusdaten |
| **Hauptsteuerung ↔ Motorsteuerung** | Main Control Unit ↔ Motor Controller | Drehzahlsollwert, Start/Stop, Statusrückmeldung |
| **Hauptsteuerung ↔ Sensorsystem** | Main Control Unit ↔ Sensor Modul | Messdaten (Strom, Vibration), Diagnoseflags |
| **Sensorsystem ↔ Zustandsüberwachung** | Sensor Modul ↔ Monitoring Service | Aufbereitete Messwerte, Belastungsdaten |
| **Zustandsüberwachung ↔ Hauptsteuerung** | Monitoring Service ↔ Main Control Unit | Restlebensdauer, Wartungsempfehlung |
| **Zustandsüberwachung ↔ User Interface** |Monitoring Service ↔ User Interface | Wartungshinweis, Lebensdaueranzeige |
| **Hauptsteuerung ↔ Safety-Control-Unit** | Main Control Unit ↔ Safety Control Unit | Betriebsstatus, Not-Halt-Signal |
| **Safety-Control-Unit ↔ Motorsteuerung** | Safety Control Unit ↔ Motor Controller | Safe Torque Off, Freigabe / Sperre |
| **Hauptsteuerung ↔ Data Management** | Main Control Unit ↔ Data Management | Ereignisse, Fehlermeldungen, Betriebsdaten |
| **Data Management ↔ User Interface** | Data Management ↔ User Interface | Anzeige von Logs, Export-Status | API /
| **Data Management ↔ USB-Schnittstelle** | Data Management ↔ Externer Datenträger (USB) | CSV-Dateien, Log-Export |
