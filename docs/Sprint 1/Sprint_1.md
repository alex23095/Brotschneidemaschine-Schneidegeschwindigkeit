# Sprint 1

### Sprint Planning

Zu Beginn des ersten Sprints wurden die relevanten Anforderungen (Requirements) aus dem Gesamtsystem ausgewählt.  
Der Fokus lag auf der Implementierung der **grundlegenden Kernfunktionen**, die für den späteren Betrieb der
Brotschneidemaschine zwingend erforderlich sind.  
Ziel war es, eine lauffähige Kommunikation zwischen **User Interface**, **Steuerlogik** und **Sicherheitsabschaltung** herzustellen.

Im Zentrum des Sprints standen folgende Teilfunktionalitäten:

- Steuerung des Schneidemotors in 10 %-Stufen (Sollwertvorgabe)
- Bedienung der Schneidegeschwindigkeit über das User-Interface
- Umsetzung der Not-Halt-Funktion (Safety-Input ≤ 100 ms)
- Einhaltung der Reaktions- und Zykluszeiten gemäß nicht-funktionaler Anforderungen

Anforderungen mit niedriger Priorität (z. B. Wartungslogik, Monitoring, Logging) wurden bewusst zurückgestellt,
um zunächst eine stabile und getestete Kernfunktionalität sicherzustellen.

**Requirements (Sprint 1):**

- **F1:** Steuerung des Schneidemotors (10 %-Stufen)  
- **F2:** Not-Halt / Sicherheitsabschaltung  
- **NF1:** Implementierung in C++ auf STM32  
- **NF2:** Reaktionszeit ≤ 200 ms  
- **NF3:** Feste Zykluszeit 100 ms  
- **NF6:** Genauigkeit ± 5 % Soll-/Ist-Drehzahl  
- **NF7:** Funktionssicherheit im Temperaturbereich 0–45 °C  

**Sprintziel:**

1. Funktionsfähige Steuerung des Schneidemotors über das UI  
2. Implementierung und Test der Sicherheitsabschaltung (Not-Halt)  
3. Nachweis der Reaktionszeiten und Zykluszeiten  
4. Basistest der Drehzahlgenauigkeit und Umgebungstoleranz  

---

### Schritt 1: Recherche

Im Rahmen der Sprintplanung wurden gezielte Recherchen zu den Themen **PWM-Ansteuerung**, **Safety-Schaltungen**
und **UI-Aktualisierungsstrategien** durchgeführt.  
Ziel war es, die technischen Anforderungen an Reaktionszeiten, deterministische Ausführung und Sicherheitsverhalten präzise umzusetzen.

**Wesentliche Erkenntnisse:**

- Für die PWM-Steuerung wurde eine Zykluszeit von **1–5 ms** als praxisgerecht ermittelt.  
- Die Sicherheitsabschaltung erfolgt **zweikanalig (Fail-Safe-Prinzip)** mit direkter Hardware-Abschaltung.  
- UI-Updates im **500 ms-Intervall** sichern gute Reaktionsfähigkeit bei geringer Prozessorlast.  
- C++ auf STM32 bietet Echtzeit-Eignung und klare Trennung von Logik- und Hardware-Schichten.  

Diese Ergebnisse flossen direkt in Architektur- und Designentscheidungen ein.

---

### Schritt 2: Architektur

Auf Basis der Recherche wurde die Softwarearchitektur in **fünf Hauptkomponenten** gegliedert:

1. **MCU – Main Control Unit**  
   (Steuerungslogik, Zustandsmaschine, Zyklus 100 ms)  
2. **UI – UserInterfaceService**  
   (Anzeige, Eingabe Start/Stop/Soll-%)  
3. **SI – SafetyInput**  
   (Not-Halt, Sicherheitsüberwachung)  
4. **MA – MotorActuator**  
   (PWM-Ausgabe, Duty-Cycle-Umsetzung)  
5. **DM – CsvLogger**  
   (Protokollierung / Datenhaltung – nicht Teil von Sprint 1)

Für Sprint 1 waren **MCU**, **UI** und **SI/MA** relevant.  
Das gewählte Architekturmuster folgt einer **komponentenbasierten Schichtenarchitektur**, bei der Bedien-, Logik-
und Hardwareebene klar getrennt sind.

Ein entsprechendes Komponentendiagramm (separat abgelegt) visualisiert Struktur und Schnittstellen.

---

### Schritt 3: Design

Im Anschluss wurde das Software-Design auf Klassenebene modelliert.
Ziel war es, die in Sprint 1 relevanten Klassen und deren Interaktionen präzise darzustellen.

**Zentrale Klassen in Sprint 1:**

| Komponente | Klasse | Aufgabe |
|-------------|--------|---------|
| **MCU** | `MainControlUnit` | Hauptsteuerung, Koordination der Teilfunktionen |
| **MCU** | `SetpointManager` | Umsetzung des Sollwerts (10 %-Schritte), Rampenfunktion |
| **UI** | `UserInterfaceService` | Bedienlogik, Anzeige Soll-/Ist-Geschwindigkeit |
| **MA** | `MotorActuator` | PWM-Ansteuerung des Schneidemotors |
| **SI** | `SafetyInput` | Erfassung und Weitergabe des Not-Halt-Signals |

Ein Sequenzdiagramm zeigt den Ablauf von der **Benutzereingabe bis zur Motorsteuerung**,  
ein Zustandsdiagramm illustriert die Übergänge **RUN ↔ STOP ↔ FAULT** sowie die Reaktion auf **Not-Halt**.

---

### Schritt 4: Implementierung



### Schritt 5: Test



### Schritt 6: Review & Retro

