#pragma once
#include <string>
#include "status.hpp"


class UserInterface {
public:
    // Bildschirm löschen (OS abhängig)
    void clearScreen() const;
    
    //Header zeichnen
    void showHeader(const Status& st) const;

    // Menüs zeichnen
    void showMainMenu() const;
    void showSafetyMenu() const;
    void showMotorMenu() const;
    void showMaintenanceMenu(const Status& st) const;
    void showMaintenanceConfig(const Status& st) const;
    void showDataHandlerMenu() const;

    // Eingaben lesen
    int  readInt() const;
    char readChar() const;

    bool tryReadKey(char& out);   // nicht-blockierend

    // kleine Pause (optional, damit man Meldungen sieht)
    void waitSeconds(int seconds) const;
};
