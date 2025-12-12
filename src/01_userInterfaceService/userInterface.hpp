#pragma once

#include <cstdint>
#include <string>

/// fasst den anzuzeigenden Maschinenstatus zusammen.
struct Status {
    bool safetyOk = false;
    bool motorEnabled = false;
    std::uint8_t dutyCyclePercent = 0;
    std::uint16_t currentMilliampere = 0;
    bool overcurrent = false;
    bool maintenanceDue = false;
};

/// UserInterfaceService
/// - kapselt Konsolenausgaben fr Benutzeraktionen
/// - wird in festen Intervallen aktualisiert (refreshIntervalMs)
class UserInterfaceService {
public:
    explicit UserInterfaceService(int refreshIntervalMs = 500);

    /// verarbeitet eine UI-Rckmeldung zur nderung der Drehzahl in 10%-Schritten
    void onSpeedPercentChange(int step10);

    /// gibt einen Startbefehl fr den Motor auf der Konsole aus
    void commandStart();

    /// meldet einen Stop-Befehl
    void commandStop();

    /// meldet einen Emergency-Stop-Befehl
    void commandEStop();

    /// zeigt den aktuellen Status auf der Konsole an
    void displayStatus(const Status& st);

    /// hebt einen Wartungshinweis hervor
    void showMaintenance(bool flag);

    int getRefreshIntervalMs() const { return refreshIntervalMs_; }

private:
    static std::string boolToText(const std::string& label, bool value);

    int refreshIntervalMs_;
};

/// LogViewer
/// - liefert eine textuelle Darstellung einer Log-Seite zur Anzeige
class LogViewer {
public:
    /// erzeugt einen String fr die angeforderte Seite und gibt ihn zur Diagnose aus
    std::string showPage(int page);
};
