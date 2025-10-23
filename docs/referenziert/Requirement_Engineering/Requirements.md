## 2. Requirements (abgeleitet aus dem Lastenheft)

### 2.1 Funktionale Requirements

| Nr. | Requirement | Herkunft (Lastenheft) |
|-----|--------------|------------------------|
| F1 | Das System muss den Schneidemotor mit variabler Drehzahl betreiben können. | 1.2 |
| F2 | Die Drehzahl soll über ein User-Interface einstellbar sein. | 1.2 |
| F3 | Das System muss eine Schneiden-Verschleiß-Überwachung durchführen. | 1.3 |
| F4 | Das System soll aus Betriebszeit, Schnittanzahl und Belastungsdaten die Restlebensdauer der Schneide berechnen. | 1.3 |
| F5 | Das System soll Wartungsempfehlungen auf Basis der Restlebensdauer ausgeben. | 1.3 |
| F6 | Das User-Interface muss Betriebsmodus, Sollwerte und Presets anzeigen und ändern können. | 1.4 |
| F7 | Das System soll Sicherheits- und Betriebsereignisse mit Zeitstempel, Fehlercode und Beschreibung protokollieren. | 1.5 |
| F8 | Das Fehlerprotokoll soll auf dem Display angezeigt und über USB exportiert werden können. | 1.5 |
| F9 | Die Not-Halt-Funktion soll alle gefahrbringenden Bewegungen sofort stoppen. | 1.1 |

---

### 2.2 Nicht-funktionale Requirements

| Nr. | Requirement | Kategorie | Herkunft |
|-----|--------------|------------|-----------|
| N1 | Das System muss den aktuellen Sicherheitsnormen entsprechen. | Sicherheit | 1.1 |
| N2 | Die Bedienung soll intuitiv und hygienisch möglich sein (auch mit Handschuhen). | Usability | 1.4 |
| N3 | Das System muss eine Reaktionszeit unter 0,5 Sekunden beim Auslösen der Schutzausrüstung gewährleisten. | Leistung/Sicherheit | 1.1 |
| N4 | Die Anzeige am User-Interface soll in weniger als 2 Sekunden aktualisiert werden. | Performance | 1.4 |
| N5 | Die USB-Schnittstelle muss gängige FAT32-formatierte Datenträger unterstützen. | Kompatibilität | 1.5 |
