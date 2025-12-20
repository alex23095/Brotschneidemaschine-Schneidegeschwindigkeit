#include "userInterface.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>

void UserInterface::clearScreen() const {
    system("cls");
}

void UserInterface::showHeader(const Status& st) const {
    //SafetyState ausgabefähig machen
    const char* safetyText = "Unbekannt";

    if (st.safetyState == SafetyState::Safe) {
        safetyText = "Safe";
    }
    else if (st.safetyState == SafetyState::EstopActive) {
        safetyText = "E-Stop aktiv";
    }
    else if (st.safetyState == SafetyState::GuardOpen) {
        safetyText = "Guard offen";
    }
    std::cout << "==========================================\n";
    std::cout << "  Brotschneidemaschine\n";
    std::cout << "==========================================\n";
    std::cout << ">>>     Aktueller SafetyState:"<< safetyText<< "\n";
    std::cout << ">>>     Solldrehzahl:" << st.speedSetpointPct << " %<<<\n";
    std::cout << ">>>     Istdrehzahl:" << st.speedActualRpm << " rpm<<<\n";
    std::cout << ">>>     Motorstrom: " << st.filteredCurrentmA << " mA<<<\n";
    std::cout << ">>>     Wartung faellig: " << (st.maintenanceDue ? "ja" : "nein")<< "<<<\n";

    std::cout << "\n";
}

void UserInterface::showMainMenu() const {
    std::cout << "=== Hauptmenue ===\n";
    std::cout << " 1: Safety-Menue\n";
    std::cout << " 2: Motor-Ansteuerung\n";
    std::cout << " 3: Motor starten\n";
    std::cout << " 4: Motor stoppen\n";
    std::cout << " 5: Wartung / Service\n";
    std::cout << " 6: Daten-Handler\n";
    std::cout << " 7: Simulation\n";
    std::cout << " 0: Beenden\n";
    std::cout << "Auswahl: ";
}

void UserInterface::showSafetyMenu() const {
    std::cout << "=== Safety-Menue ===\n";
    std::cout << " 1: Safe\n";
    std::cout << " 2: EstopActive\n";
    std::cout << " 3: GuardOpen\n";
    std::cout << " 0: Zurueck\n";
    std::cout << "Auswahl: ";
}

void UserInterface::showMotorMenu() const {
    std::cout << "=== Motor-Ansteuerung ===\n";
    std::cout << " +: Speed erhoehen\n";
    std::cout << " -: Speed verringern\n";
    std::cout << " 0: Zurueck\n";
    std::cout << "Eingabe (+/-/0): ";
}

void UserInterface::showMaintenanceMenu(const Status& st) const {
     std::cout << "=== Wartung / Service ===\n";
     std::cout << "aktuelle Betriebsdauer" << st.runtimeMs << " ms\n";
     std::cout << " 1: Wartungsintervall setzen (ms)\n";
     std::cout << " 2: Self-Test durchführen\n";
     std::cout << " 0: Zurueck\n";
     std::cout << "Auswahl: ";

}
void UserInterface::showMaintenanceConfig(const Status& st) const {
    std::cout << "=== Wartung / Service ===\n";
    std::cout << "aktuelle Betriebsdauer" << st.runtimeMs << " ms\n";
    std::cout << "Wartungsintervall in ms:";

}

void UserInterface::showDataHandlerMenu() const {
    std::cout << "=== Daten-Handler ===\n";
    std::cout << " 1: Aktuellen Systemzustand loggen\n";
    std::cout << " 0: Zurueck\n";
    std::cout << "Auswahl: ";
}
int UserInterface::readInt() const {
    int v = 0;
    std::cin >> v;
    return v;
}

char UserInterface::readChar() const {
    char c = '\0';
    std::cin >> c;
    return c;
}

bool UserInterface::tryReadKey(char& out) {
    if (_kbhit()) {
        out = static_cast<char>(_getch()); // sofort (kein Enter)
        return true;
    }
    return false;
}

void UserInterface::waitSeconds(int seconds) const {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
