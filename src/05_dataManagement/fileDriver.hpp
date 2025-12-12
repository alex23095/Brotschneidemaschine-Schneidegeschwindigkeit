#pragma once

#include <cstddef>
#include <string>

/// FileDriver
/// - verwaltet Dateizugriffe für Logging
/// - stellt Hilfsfunktionen für Größenbestimmung und Rollover bereit
class FileDriver {
public:
    explicit FileDriver(std::string basePath, std::size_t maxBytes = 1024 * 1024);

    /// Schreibt eine Zeile (mit abschließendem Newline) in die aktuelle Datei.
    bool writeLine(const std::string& line);

    /// Aktuelle Dateigröße in Bytes.
    std::size_t sizeBytes() const;

    /// Startet den Log-Rollover: aktuelle Datei wird umbenannt, neue leere Datei angelegt.
    bool rotate();

    /// Zugriff auf den aktuell verwendeten Pfad.
    const std::string& currentPath() const { return currentPath_; }

    /// Maximale Dateigröße, bevor ein Rollover ausgelöst werden soll.
    std::size_t maxBytes() const { return maxBytes_; }

private:
    void ensureFileExists();

    std::string basePath_;
    std::string currentPath_;
    std::size_t maxBytes_;
    unsigned int rotationIndex_;
};