# Sprint 1

### Sprint Planning

Zu Beginn des ersten Sprints habe ich die relevanten Anforderungen (Requirements) aus dem Gesamtsystem ausgewählt.  
Der Fokus lag dabei auf der Implementierung der **grundlegenden Kernfunktionen**, die für den späteren Betrieb des
Systems zwingend erforderlich sind. Ziel war es, eine lauffähige Basiskommunikation zwischen **User Interface**, 
**Motorsteuerung** und **Sicherheitssteuerung** herzustellen.

Im Zentrum des Sprints standen folgende Teilfunktionalitäten:

- Steuerung des Motors mit variabler Drehzahl (Grundregelung)
- Bedienung der Drehzahl über die Benutzeroberfläche (UI)
- Umsetzung einer Not-Halt-Funktion zur sofortigen Sicherheitsabschaltung
- Einhaltung der grundlegenden Sicherheits- und Reaktionszeiten gemäß den nicht-funktionalen Anforderungen

Anforderungen mit niedriger Priorität (z. B. Wartungslogik, Logging, Datenexport) wurden bewusst zurückgestellt, 
um zunächst eine stabile und getestete Kernfunktionalität zu gewährleisten.

**Requirements (Sprint 1):**

- **F1:** Motorsteuerung mit variabler Drehzahl  
- **F2:** Drehzahleinstellung über User-Interface  
- **F9:** Not-Halt / Sicherheitsabschaltung  
- **NF1:** Einhaltung Sicherheitsnorm (PL d)  
- **NF2:** Reaktionszeit und Bedienbarkeit des UI  
- **NF3:** Sicherheitsreaktionszeit des Systems   

**Sprintziel:**

1. Funktionsfähige Kernsteuerung des Motors über UI.
2. Implementierung und Test der Sicherheitsabschaltung (Not-Halt).
3. Nachweis der Reaktionszeiten der sicherheitsrelevanten Komponenten.

### Schritt 1: Recherche

Im Rahmen der Sprintplanung wurden gezielte Recherchen zu den Themen **PWM-Ansteuerung**, **Sicherheitssteuerungen**
und **UI-Update-Strategien** durchgeführt.  
Ziel war es, die Anforderungen an Zykluszeiten, Reaktionszeiten und Sicherheitsstufen (Performance Level d nach 
EN ISO 13849-1) technisch präzise umsetzen zu können.

Die wichtigsten Erkenntnisse:

- Für die PWM-Regelung wurde eine Zykluszeit von 1–5 ms als praktikabel ermittelt.  
- Die Sicherheitsabschaltung muss zweikanalig ausgelegt sein (Fail-Safe Prinzip).  
- UI-Updates in Intervallen von 500 ms gewährleisten gute Reaktionsfähigkeit ohne Überlastung.  

Diese Ergebnisse flossen direkt in das Architektur- und Designkonzept ein.


### Schritt 2: Architektur

Nach der Recherche wurde die Softwarearchitektur des Systems festgelegt.  
Das System wurde in fünf Hauptkomponenten gegliedert:

1. **MC – Motor-Controller**  
   (Regelung, PWM-Ansteuerung)
2. **UI – User-Interface-Service**  
   (Anzeige, Eingabe, Presets)
3. **SCU – Safety-Control-Unit**  
   (Not-Halt, Sicherheitsüberwachung)
4. **MS – Monitoring-Service**  
   (Zustandsbewertung, Wartungslogik)
5. **DM – Data-Management**  
   (Logging, Export)

Für Sprint 1 waren **MC**, **UI** und **SCU** relevant.  
Das gewählte Architekturmuster folgt einer **komponentenbasierten Schichtenarchitektur**, bei der Logik- und 
Darstellungsebene strikt getrennt sind.  

Das folgende Diagramm (separat abgelegt) visualisiert die Architektur und ihre Schnittstellen.


### Schritt 3: Design

Im Anschluss wurde das Software-Design auf Klassenebene modelliert.  
Ziel war es, die in Sprint 1 relevanten Klassen und deren Interaktionen präzise darzustellen.

Folgende zentrale Klassen wurden identifiziert:

| Komponente | Klasse | Aufgabe |
|-------------|--------|---------|
| **MC** | `MainControlUnit` | Regelung, Sollwertverarbeitung |
| **MC** | `PWMManager` | PWM-Generierung und Duty-Cycle-Berechnung |
| **UI** | `UserInterfaceService` | Bedienlogik, Anzeige der Drehzahl |
| **UI** | `PresetManager` | Verwaltung von Drehzahl-Presets |
| **SCU** | `EmergencyStopHandler` | Überwachung und Auslösung des Not-Halts |
| **SCU** | `SafetySupervisor` | Sicherheitsüberwachung, Statusausgabe |

Die Interaktion zwischen diesen Klassen wurde durch ein Sequenzdiagramm beschrieben, das die Abläufe von der 
Benutzereingabe bis zur Motorregelung zeigt.  
Ein Zustandsdiagramm illustriert den Ablauf des Not-Halt-Prozesses.


### Schritt 4: Implementierung



### Schritt 5: Test



### Schritt 6: Review & Retro



### Verknüpfung zur Traceability-Matrix

Die Traceability-Matrix für Sprint 1 dokumentiert die Umsetzung der oben genannten Requirements 
und zeigt die Zuordnung zwischen Anforderungen, Systemkomponenten und Designklassen.
