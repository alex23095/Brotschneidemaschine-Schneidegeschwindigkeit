#include "csvLogger.hpp"
#include <filesystem>
#include <fstream>

CsvLogger::CsvLogger(const std::string& filename, std::uint64_t maxFileSize)
    : filename_(filename), maxFileSize_(maxFileSize) {
}

bool CsvLogger::checkFileSize() const {
    namespace fs = std::filesystem;

    if (!fs::exists(filename_)) {
        return false;   // Datei existiert noch nicht → nicht zu groß
    }

    return fs::file_size(filename_) >= maxFileSize_;
}

void CsvLogger::rotateLogIfNeeded() {
    namespace fs = std::filesystem;

    if (!checkFileSize()) {
        return; // nichts zu tun
    }

    // Zielname für rotiertes Log (minimal: _1)
    const std::string rotatedName = filename_ + "_1";

    // Falls bereits vorhanden, überschreiben wir es
    if (fs::exists(rotatedName)) {
        fs::remove(rotatedName);
    }

    // Aktuelles Log umbenennen
    if (fs::exists(filename_)) {
        fs::rename(filename_, rotatedName);
    }

    // Neue leere Logdatei anlegen
    std::ofstream out(filename_, std::ios::out);
}

void CsvLogger::logStatus(const Status& st) {
    // 1) ggf. rotieren
    rotateLogIfNeeded();

    namespace fs = std::filesystem;

    // 2) Header nur schreiben, wenn Datei neu/leer ist
    const bool needHeader = (!fs::exists(filename_)) || (fs::file_size(filename_) == 0);

    // 3) Zeile anhängen
    std::ofstream out(filename_, std::ios::app);
    if (!out) return; // Sprint-3-minimal: wenn's nicht geht, einfach nichts tun

    if (needHeader) {
        out << "runtimeMs;runState;safetyState;speedSetpointPct;speedActualRpm;filteredCurrentmA;maintenanceDue\n";
    }

    out << st.runtimeMs << ';'
        << static_cast<int>(st.runState) << ';'
        << static_cast<int>(st.safetyState) << ';'
        << st.speedSetpointPct << ';'
        << st.speedActualRpm << ';'
        << st.filteredCurrentmA << ';'
        << (st.maintenanceDue ? 1 : 0)
        << '\n';
}
