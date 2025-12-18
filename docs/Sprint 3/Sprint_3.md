# Sprint 3

### Sprint Planning

Im dritten Sprint liegt der Fokus auf der **Umsetzung der noch offenen
Persistenz-Anforderungen**.  
Nach Sprint 2 sind alle Kernfunktionen (Steuerung, Safety, Monitoring,
Wartung) funktional vorhanden und in die Architektur integriert.
Sprint 3 schließt das System nun durch die **Implementierung von Logging
und Dateiverwaltung** ab.

Der Schwerpunkt liegt damit auf der Nachvollziehbarkeit des Systemverhaltens
über die Zeit sowie auf der Einhaltung von Speicher- und Dateigrenzen.

Der Fokus lag daher auf:

- Umsetzung des Loggings von Status- und Messdaten  
- Einführung einer kontrollierten Dateigrößenüberwachung  
- Umsetzung eines Log-Rollover-Mechanismus  
- Integration der Persistenzfunktion in den bestehenden Systemablauf  
- Sicherstellung, dass Logging keinen Einfluss auf Safety- oder Regelpfade hat  

**Requirements:**

- **F5:** Logging von Status- und Messdaten  
- **NF5:** Begrenzung der Dateigröße und Log-Rollover  

**Sprintziel:**

1. Persistente Ablage zentraler Systemdaten  
2. Kontrollierter Umgang mit begrenztem Speicher  
3. Vollständiger Abschluss aller offenen Requirements  
4. Systemverhalten auch über längere Laufzeiten nachvollziehbar machen  

---

### Schritt 1: Analyse der offenen Anforderungen

Zu Beginn von Sprint 3 wurde anhand der Traceability-Matrix geprüft,
welche Requirements nach Sprint 2 noch nicht umgesetzt waren.
Dabei zeigte sich, dass ausschließlich die Persistenzanforderungen
(**F5** und **NF5**) offen waren.

Alle notwendigen Eingangsdaten für das Logging
(Systemstatus, Messwerte, Wartungsinformationen) liegen bereits
im System vor und können ohne zusätzliche Erweiterungen der
Steuerlogik genutzt werden.

---

### Schritt 2: Architektur (Sprint 3)

Die bestehende Architektur aus Sprint 2 bleibt unverändert bestehen.
Sprint 3 nutzt die vorbereitete Persistenz-Schicht, um die Logging-Funktionalität
konkret umzusetzen.

1. **Control Logic Layer**  
   - Bereitstellung der relevanten Status- und Messdaten

2. **Persistence Layer**  
   (`CsvLogger`, `FileDriver`)  
   - Speicherung von Status- und Messdaten  
   - Überwachung der Dateigröße  
   - Durchführung eines Log-Rollovers bei Erreichen definierter Grenzen  

**Änderungen in Sprint 3:**

- Persistenz-Schicht erstmals funktional genutzt  
- Keine neuen Abhängigkeiten zwischen bestehenden Komponenten  
- Keine Änderungen an Control- oder Hardware-Schichten notwendig  

---

### Schritt 3: Design

Das Design in Sprint 3 konkretisiert die zuvor nur modellierte
Persistenzfunktionalität.

**Relevante Klassen in Sprint 3:**

| Komponente | Klasse | Aufgabe |
|-----------|--------|---------|
| **DM** | `CsvLogger` | Protokollierung von Status- und Messdaten |
| **DM** | `FileDriver` | Dateizugriff, Größenprüfung und Rollover |
| **MCU** | `MainControlUnit` | Bereitstellung der zu loggenden Systemdaten |

Die Persistenzkomponenten arbeiten nachgelagert und beeinflussen
weder die Regelung noch sicherheitsrelevante Abläufe.

---

### Ergebnis Sprint 3

Mit Sprint 3 sind nun **alle funktionalen und nichtfunktionalen
Requirements des Projekts umgesetzt**:

- Status- und Messdaten werden persistent protokolliert  
- Logdateien bleiben durch Rollover kontrollierbar  
- Das System ist auch über längere Laufzeiten analysierbar  
- Die Architektur aus Sprint 2 hat sich als vollständig und tragfähig erwiesen  

Sprint 3 stellt damit den **funktionalen Abschluss des Projekts** dar.

# Review & Retro  
## Sprint 3

---

## Vergleich Architektur / Design vs. Implementierung Sprint 3

| Geplant (Architektur / Design) | Implementiert in Sprint 3 | Abweichung |
|--------------------------------|---------------------------|------------|
| CSV-Logging mit 1 Hz über Persistence-Schicht | CSV-Logging vollständig umgesetzt über `CsvLogger` | Frequenz fest implementiert (nicht parametrierbar) |
| Nutzung eines Dateisystem-Abstraktionslayers | Direkte Nutzung von `std::filesystem` (C++17) | Keine zusätzliche Abstraktionsklasse eingeführt |
| Logging robust gegen Neustart & bestehende Dateien | Existenzprüfung, Neu-Anlage und Fortführung umgesetzt | Keine Log-Rotation implementiert |
| UI-Komponente als eigenständiges Modul | `UserInterface` als separates Modul integriert | UI weiterhin rein funktional (keine echte Anzeige) |
| Vollständiger End-to-End-Datenfluss (UI → MCU → Persistence) | Steuer- und Logging-Fluss vollständig realisiert | – |
| Toolchain-Anpassung dokumentieren (C++17) | C++17 aktiv gesetzt und genutzt | Anpassung erst spät im Sprint erfolgt |

---

## Was lief gut?

- CSV-Export funktioniert stabil und nachvollziehbar im laufenden Betrieb.  
- `std::filesystem` erfolgreich integriert und produktiv genutzt.  
- UserInterface erstmals als **echtes Modul** im Projekt vorhanden.  
- End-to-End-Fluss (UI → MCU → Setpoint → Motor → Logging) klar nachvollziehbar.  
- Projekt- und Ordnerstruktur nun stabil und sprintübergreifend nutzbar.  
- Compiler- und Build-Probleme systematisch analysiert und sauber gelöst.  

---

## Was lief nicht gut?

- Zeitverlust durch fehlendes Wissen über C++-Sprachstandard (C++14 vs. C++17).  
- Fehlermeldungen zu `std::filesystem` zunächst schwer einzuordnen.  
- CSV-Design (Header, Felder, Format) erst spät finalisiert.  
- Logging-Strategie (Überschreiben / Anhängen / Rotation) nicht vollständig geklärt.  
- UI bleibt funktional minimal, keine Visualisierung oder Zustandsanzeige.  

---

## Lessons Learned

- Sprachstandard ist Teil der Architekturentscheidung, nicht nur ein Compiler-Detail.  
- Neue Funktionalität (z. B. Dateizugriff) ⇒ Toolchain früh prüfen.  
- Persistenz- und Logging-Konzepte früh sauber spezifizieren.  
- Ordnerstruktur, Include-Pfade und Build-Settings gehören zusammen betrachtet.  
- End-to-End-Flows helfen enorm beim Debugging komplexer Fehler.  
- Dokumentation parallel zur Implementierung pflegen, nicht erst am Sprintende.

---

## Baseline Sprint 3

- **Funktional erweitert:**  
  - CSV-Logging (1 Hz), Persistenz über Dateisystem, UI-Anbindung.

- **Implementierte/erweiterte Komponenten:**  
  - `CsvLogger`, `PersistenceManager`, `UserInterface`,  
    Erweiterungen in `MainControlUnit` zur Logging-Integration.

- **Teststatus:**  
  - Umfangreiche manuelle Modul- und Integrationstests.  
  - Regressionstests für Sprint 1 und Sprint 2 vollständig durchgeführt.  
  - Keine automatisierten Tests.

- **Dokumentationsstand:**  
  - Testdokumentation Sprint 3 vollständig.  
  - Traceability-Matrix aktualisiert.  
  - Architektur und Implementierung weitgehend konsistent (UI & Logging nicht mehr nur konzeptionell).

---

## Gesamtfazit Sprint 3

Sprint 3 stellt einen klaren Reife-Sprung dar:  
Erstmals existiert eine durchgängige Persistenzlösung mit realem Dateizugriff sowie ein klarer End-to-End-Datenfluss inklusive UI-Anbindung.  
Die Systembasis ist nun stabil genug, um in kommenden Sprints Themen wie Testautomatisierung, UI-Erweiterung oder Logging-Rotation gezielt anzugehen.

