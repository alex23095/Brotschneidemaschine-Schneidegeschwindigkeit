# Verwendete Design-Patterns

Dieses Kapitel beschreibt die im Software-Design eingesetzten Entwurfsmuster (Design-Patterns) und deren Nutzen im Gesamtsystem.

---

## **1. Strategy Pattern – Austauschbare Regelalgorithmen**

Im Softwaredesign wird das **Strategy Pattern** eingesetzt, um unterschiedliche Regelverfahren (z. B. PI- oder PID-Regler) flexibel und austauschbar zu gestalten.  
Die `MainControlUnit` verwendet hierzu eine Schnittstelle `IControlAlgorithm`, über die konkrete Strategien wie `PIController` oder `PIDController` implementiert werden können.  
Dadurch kann das Regelverhalten zur Laufzeit angepasst werden, ohne dass Änderungen am Steuerungskern notwendig sind.  
Dieses Muster erhöht die **Modularität**, **Testbarkeit** und **Wiederverwendbarkeit** der Regelkomponenten.

**Vorteile:**
- Austauschbare Regelstrategien ohne Codeänderung im Kern
- Leicht erweiterbar für neue Reglerarten
- Unterstützt Unit-Tests durch klare Schnittstellen

---

## **2. State Pattern – Betriebsmodi des Systems**

Für die Abbildung der verschiedenen Betriebsmodi („Automatik“, „Manuell“, „Wartung“) wird das **State Pattern** verwendet.  
Jeder Modus wird als eigener Zustand (`AutoState`, `ManualState`, `MaintenanceState`) implementiert, der ein gemeinsames Interface `IModeState` nutzt.  
Die `MainControlUnit` wechselt dynamisch zwischen diesen Zuständen, wodurch sich das Verhalten des Systems (z. B. zulässige Eingaben oder Freigaben) automatisch an den aktuellen Modus anpasst.  
Das Muster sorgt für eine klare Trennung des zustandsspezifischen Verhaltens und reduziert komplexe `if`- bzw. `switch`-Strukturen.

**Vorteile:**
- Saubere Trennung von Zustandslogik
- Einfaches Hinzufügen neuer Betriebsmodi
- Verständlicher Systemablauf durch klar definierte Zustände

---

## **3. Observer Pattern – Status- und Ereignisbenachrichtigung**

Zur Entkopplung der Komponenten und zur Verteilung von Statusinformationen kommt das **Observer Pattern** zum Einsatz.  
Ein zentraler `EventBus` oder `StatusManager` informiert registrierte Abonnenten (z. B. `UserInterfaceService`, `LogViewer`) über Änderungen im Systemzustand.  
Dadurch können Benutzeroberfläche und Protokollierung auf neue Daten reagieren, ohne dass direkte Abhängigkeiten zur Steuerlogik bestehen.  
Dieses Muster unterstützt eine **lose Kopplung**, fördert die **Erweiterbarkeit** und ermöglicht eine klare Trennung zwischen Datenquelle und Anzeige.

**Vorteile:**
- Lose Kopplung zwischen Sender und Empfänger
- Einfache Erweiterung um neue Beobachter
- Saubere Trennung von Logik und Darstellung

---

## **4. Facade Pattern – Vereinfachte Hardwareabstraktion**

Zur Vereinheitlichung des Zugriffs auf die Hardware-Schicht wird ein **Facade Pattern** angewendet.  
Ein `HardwareAbstractionLayer` kapselt dabei die direkte Kommunikation mit PWM-Treibern, Sensoren und Schutzmechanismen.  
Die darüberliegende Anwendungslogik greift nur über standardisierte Methoden zu, was den **Austausch oder Test der Hardware** erleichtert.  
Dieses Muster trägt zur **Kapselung** und **Portierbarkeit** des Systems bei.

**Vorteile:**
- Einheitliche Schnittstelle zur Hardware
- Leichtere Testbarkeit (Mocking möglich)
- Reduzierte Abhängigkeiten zwischen Schichten

---

**Fazit:**  
Durch den gezielten Einsatz der Design-Patterns **Strategy**, **State**, **Observer** und **Facade** wird das Gesamtsystem modular, wartbar und erweiterbar gestaltet.  
Jedes Muster adressiert dabei gezielt wiederkehrende Entwurfsprobleme innerhalb der Steuer- und UI-Architektur.
