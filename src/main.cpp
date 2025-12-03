#include <iostream>
#include "mainControlUnit.hpp"
#include "safetyInput.hpp"
#include <thread>
#include <chrono>

#include <cstdlib>
//#include "persistenceManager.hpp"

// ---------------------------------------------------------
// Hilfsfunktionen (nur Deklaration)
// ---------------------------------------------------------
void showMainMenu(MainControlUnit& mcu);

void handleSafetyMenu(MainControlUnit& mcu, SafetyState& currentSafety);
void handleMotorMenu(MainControlUnit& mcu, int& speedStep, int& storedSpeedStep);
void handleDataHandlerMenu();

void applySafetyToMcu(MainControlUnit& mcu, SafetyState state);

void clearScreen() {
    // Variante 1: OS-basiert
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// ---------------------------------------------------------
// main
// ---------------------------------------------------------
int main() {
    MainControlUnit& mcu = MainControlUnit::instance();
    //PersistenceManager persistence;  // ggf. an deine API anpassen

    bool running = true;

    // UI-Zustände
    int  speedStep = 0;  // aktueller UI-Speed 0–10
    int  storedSpeedStep = 5;  // "gemerkter" Wert für Motor-Start
    bool motorRunning = false;
    SafetyState currentSafety = SafetyState::NotReady;

    // ggf. initial Safety in die MCU schreiben
    applySafetyToMcu(mcu, currentSafety);
    mcu.setUiSpeedCommandStep(speedStep);

    while (running) {
        clearScreen();
        showMainMenu(mcu);

        int choice = 0;
        std::cin >> choice;

        switch (choice) {
        case 1:
            clearScreen();
            handleSafetyMenu(mcu, currentSafety);
            break;

        case 2:
            clearScreen();
            handleMotorMenu(mcu, speedStep, storedSpeedStep);
            break;

        case 3: // Motor starten
            if (!motorRunning) {
                if (speedStep == 0) {
                    // Wenn noch nichts eingestellt war, nimm den gemerkten Wert
                    speedStep = storedSpeedStep;
                }
                mcu.setUiSpeedCommandStep(speedStep);
                motorRunning = true;
                clearScreen();
                std::cout << "Motor gestartet (Speed-Step = " << speedStep << ")\n";
                std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            }
            else {
                clearScreen();
                std::cout << "Motor laeuft bereits.\n";
                std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            }
            break;

        case 4: // Motor stoppen
            if (motorRunning) {
                // aktuellen Wert merken, damit wir beim Starten wieder dahin springen können
                storedSpeedStep = speedStep;
                speedStep = 0;
                mcu.setUiSpeedCommandStep(0);
                motorRunning = false;
                clearScreen();
                std::cout << "Motor gestoppt.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1)); // 3 s verweilen
            }
            else {
                clearScreen();
                std::cout << "Motor ist bereits aus.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1)); // 3 s verweilen
            }
            break;

        case 5:
            clearScreen();
            handleDataHandlerMenu();
            break;

        case 0:
            running = false;
            break;

        default:
            clearScreen();
            std::cout << "Ungueltige Auswahl.\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            break;
        }

        // Einen Steuerzyklus nach jedem Befehl ausführen (kannst du anpassen)
        mcu.tick();
    }

    return 0;
}

// ---------------------------------------------------------
// Hauptmenü anzeigen
// ---------------------------------------------------------
void showMainMenu(MainControlUnit& mcu) {
    std::cout << "=== Brotschneidemaschine Hauptmenue ===\n";
    std::cout << ">>>\tIstdrehzahl: " << mcu.currentSetpointRpm() << "\t\t<<<\n";
    std::cout << ">>>\tSolldrehzahl: " << mcu.targetSetpointRpm() << "\t\t<<<\n";
    std::cout << " 1: Safety\n";
    std::cout << " 2: Motor-Ansteuerung\n";
    std::cout << " 3: Motor starten\n";
    std::cout << " 4: Motor stoppen\n";
    std::cout << " 5: Daten-Handler\n";
    std::cout << " 0: Beenden\n";
    std::cout << "Auswahl: ";
}

// ---------------------------------------------------------
// Safety-Untermenü
// ---------------------------------------------------------
void handleSafetyMenu(MainControlUnit& mcu, SafetyState& currentSafety) {
    bool back = false;

    while (!back) {
        std::cout << "\n--- Safety-Menue ---\n";
        std::cout << "Aktueller SafetyState: ";
        switch (currentSafety) {
        case SafetyState::NotReady:    std::cout << "NotReady\n";    break;
        case SafetyState::Safe:        std::cout << "Safe\n";        break;
        case SafetyState::EstopActive: std::cout << "EstopActive\n"; break;
        case SafetyState::GuardOpen:   std::cout << "GuardOpen\n";   break;
        case SafetyState::Fault:       std::cout << "Fault\n";       break;
        }

        std::cout << " 1) NotReady\n";
        std::cout << " 2) Safe\n";
        std::cout << " 3) EstopActive (Not-Halt)\n";
        std::cout << " 4) GuardOpen  (Schutztuer offen)\n";
        std::cout << " 5) Fault\n";
        std::cout << " 0) Zurueck\n";
        std::cout << "Auswahl: ";

        int choice = 0;
        std::cin >> choice;

        switch (choice) {
        case 1:
            currentSafety = SafetyState::NotReady;
            applySafetyToMcu(mcu, currentSafety);
            break;
        case 2:
            currentSafety = SafetyState::Safe;
            applySafetyToMcu(mcu, currentSafety);
            break;
        case 3:
            currentSafety = SafetyState::EstopActive;
            applySafetyToMcu(mcu, currentSafety);
            break;
        case 4:
            currentSafety = SafetyState::GuardOpen;
            applySafetyToMcu(mcu, currentSafety);
            break;
        case 5:
            currentSafety = SafetyState::Fault;
            applySafetyToMcu(mcu, currentSafety);
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Ungueltige Auswahl.\n";
            break;
        }

        mcu.tick();
    }
}

// ---------------------------------------------------------
// Motor-Ansteuerung-Untermenü
// ---------------------------------------------------------
void handleMotorMenu(MainControlUnit& mcu, int& speedStep, int& storedSpeedStep) {
    bool back = false;

    while (!back) {
        clearScreen();
        std::cout << "\n=== Motor-Ansteuerung ===\n";
        std::cout << ">>>\tAnzahl Schritte (0–10): " << speedStep << "\t\t<<<\n";
        std::cout << ">>>\tIstdrehzahl: " << mcu.currentSetpointRpm() << "\t\t<<<\n";
        std::cout << ">>>\tSolldrehzahl: " << mcu.targetSetpointRpm() << "\t\t<<<\n";
        std::cout << "\n";
        std::cout << " +: Speed erhoehen\n";
        std::cout << " -: Speed verringern\n";
        std::cout << " 0: Zurueck\n";
        std::cout << "Eingabe (+/-/0): ";

        char cmd;
        std::cin >> cmd;

        switch (cmd) {
        case '+':
            clearScreen();
            if (speedStep < 10) {
                ++speedStep;
                mcu.setUiSpeedCommandStep(speedStep);
                storedSpeedStep = speedStep; // gemerkter Wert fuer spaeteres Starten
            }
            else {
                std::cout << "Maximaler Speed-Step erreicht.\n";
                std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            }
            break;

        case '-':
            clearScreen();
            if (speedStep > 0) {
                --speedStep;
                mcu.setUiSpeedCommandStep(speedStep);
                if (speedStep > 0) {
                    storedSpeedStep = speedStep;
                }
            }
            else {
                std::cout << "Speed-Step ist bereits 0.\n";
                std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            }
            break;

        case '0':
            clearScreen();
            back = true;
            break;

        default:
            clearScreen();
            std::cout << "Ungueltige Eingabe.\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            break;
        }

        mcu.tick();
    }
}

// ---------------------------------------------------------
// Daten-Handler-Untermenü (PersistenceManager)
// ---------------------------------------------------------
void handleDataHandlerMenu() {
    bool back = false;

    while (!back) {
        clearScreen();
        std::cout << "\n===\tDaten-Handler\t===\n";
        std::cout << " 1: Aktuellen Systemzustand loggen\n";
        std::cout << " 2: Log loeschen\n";
        std::cout << " 0: Zurueck\n";
        std::cout << "Auswahl: ";

        int choice = 0;
        std::cin >> choice;

        switch (choice) {
        case 1:
            clearScreen();
            //persistence.logCurrentState(mcu);  // <-- MUSST DU IN PersistenceManager IMPLEMENTIEREN
            std::cout << "Systemzustand geloggt.\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            break;

        case 2:
            clearScreen();
           // persistence.clearLog();            // <-- ebenfalls im PersistenceManager implementieren
            std::cout << "Log geloescht.\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            break;

        case 0:
            clearScreen();
            back = true;
            break;

        default:
            clearScreen();
            std::cout << "Ungueltige Auswahl.\n";
            std::this_thread::sleep_for(std::chrono::seconds(3)); // 3 s verweilen
            break;
        }
    }
}

// ---------------------------------------------------------
// Safety-State -> MCU-Eingaenge
// ---------------------------------------------------------
/// Diese Funktion musst du an deine echte Safety-Logik anpassen.
/// Idee:
///  - Du erzeugst hier ein SafetyInput::Inputs inputs{};
///  - setzt die Roh-Eingaenge passend zu state
///  - und rufst dann mcu.setSafetyInputs(inputs);
void applySafetyToMcu(MainControlUnit& mcu, SafetyState state) {
    // TODO:
    // SafetyInput::Inputs inputs{};
    //
    // switch (state) {
    //   case SafetyState::Safe:
    //       inputs.emergencyStop = false;
    //       inputs.guardClosed   = true;
    //       break;
    //   case SafetyState::EstopActive:
    //       inputs.emergencyStop = true;
    //       inputs.guardClosed   = true;
    //       break;
    //   case SafetyState::GuardOpen:
    //       inputs.emergencyStop = false;
    //       inputs.guardClosed   = false;
    //       break;
    //   ...
    // }
    //
    // mcu.setSafetyInputs(inputs);

    (void)mcu;
    (void)state;
}
