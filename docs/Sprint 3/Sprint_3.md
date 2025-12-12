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
