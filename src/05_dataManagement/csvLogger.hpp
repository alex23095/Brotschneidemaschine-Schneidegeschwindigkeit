#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "fileDriver.hpp"

/// Struktur mit allen relevanten Werten fr einen Logeintrag.
struct LogRecord {
    std::string timestamp;
    bool safetyOk;
    bool motorEnabled;
    std::uint8_t dutyCyclePercent;
    std::uint16_t currentMilliampere;
    bool overcurrent;
    bool maintenanceDue;
};

/// CsvLogger
/// - formatiert Status- und Messdaten als CSV-Zeile
/// - delegiert Dateihandling an FileDriver und prft Dateigre auf Rollover
class CsvLogger {
public:
    explicit CsvLogger(FileDriver& driver, std::size_t maxBytes = 1024 * 1024);

    /// protokolliert einen Statusdatensatz als CSV
    void logStatus(const LogRecord& record);

    /// prft die Dateigre und lsst bei Bedarf rotate() ausfhren.
    bool rotateLogIfNeeded();

    /// liefert die aktuelle Dateigre fr Diagnose/Testzwecke.
    std::size_t checkFileSize() const;

private:
    static std::string boolToString(bool value);
    std::string formatRecord(const LogRecord& record) const;

    FileDriver& driver_;
    std::size_t maxBytes_;
};