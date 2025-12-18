#include "userInterface.hpp"

#include "mainControlUnit.hpp"
#include "safetyInput.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>

UserInterfaceService::UserInterfaceService(int refreshIntervalMs)
    : refreshIntervalMs_(refreshIntervalMs) {
}

void UserInterfaceService::onSpeedPercentChange(int step10) {
    const int clamped = std::max(0, std::min(step10, 10));
    const int percent = clamped * 10;
}

void UserInterfaceService::commandStart(int step10) {
    MainControlUnit& mcu = MainControlUnit::instance();
    mcu.setUiSpeedCommandStep(step10);
    mcu.tick();
}

void UserInterfaceService::commandStop() {
    
}

void UserInterfaceService::commandEStop() {
    
}

void UserInterfaceService::displayStatus(const Status& st) {
    std::cout << "[UI] Status-Refresh (alle " << refreshIntervalMs_ << " ms):" << std::endl;
    std::cout << "  " << boolToText("Safety", st.safetyOk) << std::endl;
    std::cout << "  " << boolToText("Motor", st.motorEnabled) << std::endl;
    std::cout << "  Duty-Cycle: " << static_cast<int>(st.dutyCyclePercent) << "%" << std::endl;
    std::cout << "  Stromaufnahme: " << st.currentMilliampere << " mA" << std::endl;
    std::cout << "  " << boolToText("berstrom", st.overcurrent) << std::endl;
    std::cout << "  " << boolToText("Wartung fllig", st.maintenanceDue) << std::endl;
}

void UserInterfaceService::showMaintenance(bool flag) {
    if (flag) {
        std::cout << "[UI] Wartungshinweis: Bitte Service einplanen." << std::endl;
    }
    else {
        std::cout << "[UI] Wartungshinweis zurckgesetzt." << std::endl;
    }
}

std::string UserInterfaceService::boolToText(const std::string& label, bool value) {
    std::ostringstream out;
    out << label << ": " << (value ? "OK" : "NICHT OK");
    return out.str();
}

std::string LogViewer::showPage(int page) {
    const int normalizedPage = std::max(0, page);
    std::ostringstream out;
    out << "[LogViewer] Zeige Seite " << normalizedPage << " des Logs an.";
    const std::string message = out.str();
    std::cout << message << std::endl;
    return message;
}

ConsoleUserInterface::ConsoleUserInterface(MainControlUnit& mcu, UserInterfaceService& uiService, LogViewer& logViewer)
    : mcu_(mcu)
    , uiService_(uiService)
    , logViewer_(logViewer)
    , currentSafety_(SafetyState::NotReady) {
    applySafetyToMcu(currentSafety_);
    mcu_.setSpeedStep(speedStep_);
}

void ConsoleUserInterface::run() {
    bool running = true;

    while (running) {
        clearScreen();
        showMainMenu();

        int choice = 0;
        std::cin >> choice;

        switch (choice) {
        case 1:
            clearScreen();
            handleSafetyMenu();
            break;

        case 2:
            clearScreen();
            handleMotorMenu();
            break;

        case 3: // Motor starten
            if (!motorRunning_) {
                if (speedStep_ == 0) {
                    // Wenn noch nichts eingestellt war, nimm den gemerkten Wert
                    speedStep_ = storedSpeedStep_;
                }
                mcu_.setSpeedStep(speedStep_);
                motorRunning_ = true;
                clearScreen();
                uiService_.commandStart(speedStep_);
                waitSeconds(3);
            }
            break;

        case 4: // Motor stoppen
            if (motorRunning_) {
                // aktuellen Wert merken, damit wir beim Starten wieder dahin springen können
                storedSpeedStep_ = speedStep_;
                speedStep_ = 0;
                mcu_.setSpeedStep(0);
                motorRunning_ = false;
                clearScreen();
                uiService_.commandStop();
                waitSeconds(1);
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
            waitSeconds(3);
            break;
        }

        // Einen Steuerzyklus nach jedem Befehl ausführen
        mcu_.tick();
    }
}

void ConsoleUserInterface::showMainMenu() {
    std::cout << "==========================================\n";
    std::cout << "  Brotschneidemaschine\n";
    std::cout << "==========================================\n";
    std::cout << ">>>\tAktueller SafetyState:";

    SafetyState state = mcu_.getSafetyStatus();

    switch (state) {
    case SafetyState::NotReady: std::cout << "NotReady\t<<<\n"; break;
    case SafetyState::Safe: std::cout << "Safe\t<<<\n"; break;
    case SafetyState::EstopActive: std::cout << "EstopActive\t<<<\n"; break;
    case SafetyState::GuardOpen: std::cout << "GuardOpen\t<<<\n"; break;
    case SafetyState::Fault: std::cout << "Fault\t<<<\n"; break;
    }
    std::cout << ">>>\tIstdrehzahl:" << mcu_.currentSetpointRpm() << "\t\t\t<<<\n";
    std::cout << "\n";
    std::cout << "\n";

    std::cout << "=== Hauptmenue ===\n";
    std::cout << " 1: Safety-Menue\n";
    std::cout << " 2: Motor-Ansteuerung\n";
    std::cout << " 3: Motor starten\n";
    std::cout << " 4: Motor stoppen\n";
    std::cout << " 5: Daten-Handler\n";
    std::cout << " 0: Beenden\n";
    std::cout << "Auswahl: ";
}

void ConsoleUserInterface::handleSafetyMenu() {
    bool back = false;

    while (!back) {
        clearScreen();
        std::cout << "===\tSafety-Menue\t===\n";
        std::cout << ">>>\tAktueller SafetyState:";
        switch (currentSafety_) {
        case SafetyState::NotReady: std::cout << "NotReady\t<<<\n"; break;
        case SafetyState::Safe: std::cout << "Safe\t<<<\n"; break;
        case SafetyState::EstopActive: std::cout << "EstopActive\t<<<\n"; break;
        case SafetyState::GuardOpen: std::cout << "GuardOpen\t<<<\n"; break;
        case SafetyState::Fault: std::cout << "Fault\t<<<\n"; break;
        }

        std::cout << " 1: NotReady\n";
        std::cout << " 2: Safe\n";
        std::cout << " 3: EstopActive (Not-Halt)\n";
        std::cout << " 4: GuardOpen  (Schutztuer offen)\n";
        std::cout << " 5: Fault\n";
        std::cout << " 0: Zurueck\n";
        std::cout << "Auswahl: ";

        int choice = 0;
        std::cin >> choice;

        switch (choice) {
        case 1:
            currentSafety_ = SafetyState::NotReady;
            applySafetyToMcu(currentSafety_);
            break;
        case 2:
            currentSafety_ = SafetyState::Safe;
            applySafetyToMcu(currentSafety_);
            break;
        case 3:
            currentSafety_ = SafetyState::EstopActive;
            applySafetyToMcu(currentSafety_);
            uiService_.commandEStop();
            break;
        case 4:
            currentSafety_ = SafetyState::GuardOpen;
            applySafetyToMcu(currentSafety_);
            break;
        case 5:
            currentSafety_ = SafetyState::Fault;
            applySafetyToMcu(currentSafety_);
            break;
        case 0:
            back = true;
            break;
        default:
            clearScreen();
            std::cout << "Ungueltige Auswahl.\n";
            waitSeconds(2);
            break;
        }

        mcu_.tick();
    }
}

void ConsoleUserInterface::handleMotorMenu() {
    bool back = false;

    while (!back) {
        clearScreen();
        std::cout << "=== Motor-Ansteuerung ===\n";
        std::cout << ">>>\tAnzahl Schritte (0–10): " << speedStep_ << "\t\t<<<\n";
        std::cout << ">>>\tIstdrehzahl: " << mcu_.currentSetpointRpm() << "\t\t<<<\n";
        std::cout << ">>>\tSolldrehzahl: " << mcu_.targetSetpointRpm() << "\t\t<<<\n";
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
            if (speedStep_ < 10) {
                ++speedStep_;
                mcu_.setSpeedStep(speedStep_);
                storedSpeedStep_ = speedStep_; // gemerkter Wert fuer spaeteres Starten
                uiService_.onSpeedPercentChange(speedStep_);
            }
            break;

        case '-':
            clearScreen();
            if (speedStep_ > 0) {
                --speedStep_;
                mcu_.setSpeedStep(speedStep_);
                if (speedStep_ > 0) {
                    storedSpeedStep_ = speedStep_;
                }
                uiService_.onSpeedPercentChange(speedStep_);
            }
            break;

        case '0':
            clearScreen();
            back = true;
            break;

        default:
            clearScreen();
            std::cout << "Ungueltige Eingabe.\n";
            waitSeconds(3);
            break;
        }

        mcu_.tick();
    }
}

void ConsoleUserInterface::handleDataHandlerMenu() {
    bool back = false;

    while (!back) {
        clearScreen();
        std::cout << "===\tDaten-Handler\t===\n";
        std::cout << " 1: Aktuellen Systemzustand loggen\n";
        std::cout << " 2: Log loeschen\n";
        std::cout << " 3: Log-Seite ansehen\n";
        std::cout << " 0: Zurueck\n";
        std::cout << "Auswahl: ";

        int choice = 0;
        std::cin >> choice;

        switch (choice) {
        case 1:
            clearScreen();
            //persistence.logCurrentState(mcu_);  // <-- MUSST DU IN PersistenceManager IMPLEMENTIEREN
            std::cout << "Systemzustand geloggt.\n";
            waitSeconds(3);
            break;

        case 2:
            clearScreen();
            // persistence.clearLog();            // <-- ebenfalls im PersistenceManager implementieren
            std::cout << "Log geloescht.\n";
            waitSeconds(3);
            break;

        case 3: {
            clearScreen();
            std::cout << "Welche Seite moechtest du sehen? ";
            int page = 0;
            std::cin >> page;
            logViewer_.showPage(page);
            waitSeconds(3);
            break;
        }

        case 0:
            clearScreen();
            back = true;
            break;

        default:
            clearScreen();
            std::cout << "Ungueltige Auswahl.\n";
            waitSeconds(3);
            break;
        }
    }
}

void ConsoleUserInterface::applySafetyToMcu(SafetyState state) {
    SafetyInput::Inputs inputs{};

    switch (state) {
    case SafetyState::Safe:
        inputs.estopNc = true;
        inputs.guardDoorNc = true;
        inputs.safetyReset = true;
        break;
    case SafetyState::EstopActive:
        inputs.estopNc = false;
        inputs.guardDoorNc = true;
        inputs.safetyReset = false;
        break;
    case SafetyState::GuardOpen:
        inputs.estopNc = true;
        inputs.guardDoorNc = false;
        inputs.safetyReset = false;
        break;
    case SafetyState::Fault:
        inputs.estopNc = false;
        inputs.guardDoorNc = false;
        inputs.safetyReset = false;
        break;
    case SafetyState::NotReady:
    default:
        inputs.estopNc = false;
        inputs.guardDoorNc = false;
        inputs.safetyReset = false;
        break;
    }

    mcu_.setSafetyInputs(inputs);
}

void ConsoleUserInterface::clearScreen() const {
    // Variante 1: OS-basiert
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleUserInterface::waitSeconds(int seconds) const {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}