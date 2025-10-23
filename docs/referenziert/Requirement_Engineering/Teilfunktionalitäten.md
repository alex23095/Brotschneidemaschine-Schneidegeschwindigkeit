## 2.1 Funktionale Requirements

| Nr. | Requirement (messbar formuliert) | Herkunft |
|-----|----------------------------------|-----------|
| 1 | Schneidemotor mit einer variablen Drehzahl zwischen **500 U/min und 3000 U/min**, regelbar in **10 %-Schritten**
| 2 | Die Drehzahl wird über das User-Interface eingestellt; Änderungen werden innerhalb von **1 s** an die Motorsteuerung übertragen
| 3 | Das System erfasst kontinuierlich die **Stromaufnahme (±2 %)** und **Vibrationsamplitude (±0,01 g)** der Schneide zur Verschleißüberwachung
| 4 | Die Restlebensdauer der Schneide wird aus **Betriebszeit (min)**, **Schnittanzahl** und **Belastungsdaten** berechnet; die Berechnung erfolgt mindestens **alle 10 s**
| 5 | Das System generiert eine Wartungsempfehlung, sobald die berechnete Restlebensdauer **unter 10 %** des Sollwertes fällt, und zeigt sie als Pop-up auf dem Display an
| 6 | Das User-Interface zeigt die **aktuellen Betriebsmodi, Sollwerte und gespeicherten Presets** an; Änderungen werden innerhalb von **2 s** übernommen
| 7 | Alle Sicherheits- und Betriebsereignisse werden mit **Zeitstempel (±1 s Genauigkeit)**, **Fehlercode** und **Beschreibung** in einer Logdatei gespeichert
| 8 | Das Fehlerprotokoll wird auf dem Display angezeigt und kann als **CSV-Datei** über **USB 2.0** auf **FAT32-Datenträger** exportiert werden; der Exportvorgang dauert **< 5 s pro 1000 Einträge**
| 9 | Bei Betätigung des Not-Halts stoppt der Schneidemotor und alle Antriebe innerhalb von **< 0,5 s** vollständig

---

## 2.2 Nicht-funktionale Requirements

| Nr. | Requirement (messbar formuliert) | Kategorie | Herkunft |
|-----|----------------------------------|------------|-----------|
| 1 | Das System erfüllt die Sicherheitsanforderungen gemäß **EN ISO 13849-1 Performance Level d**. | Sicherheit | 1.1 |
| 2 | Die Bedienoberfläche reagiert auf Eingaben mit **max. 0,3 s Verzögerung** und bleibt bei **10 mm dicken Handschuhen** vollständig bedienbar. | Usability | 1.4 |
| 3 | Beim Auslösen der Schutzausrüstung werden alle gefährdenden Bewegungen innerhalb von **0,5 s ± 0,05 s** gestoppt. | Leistung/Sicherheit | 1.1 |
| 4 | Die Anzeige auf dem User-Interface aktualisiert Betriebsparameter innerhalb von **2 s**, wenn Eingangsdaten geändert werden. | Performance | 1.4 |
| 5 | Die USB-Schnittstelle erkennt und liest **FAT32-formatierte Datenträger** mit einer Größe bis **32 GB** und einer Dateigröße bis **4 GB**. | Kompatibilität | 1.5 |
