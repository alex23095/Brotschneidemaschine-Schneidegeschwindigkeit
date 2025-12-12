#include "csvLogger.hpp"

#include <sstream>

CsvLogger::CsvLogger(FileDriver& driver, std::size_t maxBytes)
    : driver_{ driver }
    , maxBytes_{ maxBytes }
{
}

void CsvLogger::logStatus(const LogRecord& record)
{
    rotateLogIfNeeded();
    driver_.writeLine(formatRecord(record));
}

bool CsvLogger::rotateLogIfNeeded()
{
    if (checkFileSize() >= maxBytes_) {
        return driver_.rotate();
    }

    return false;
}

std::size_t CsvLogger::checkFileSize() const
{
    return driver_.sizeBytes();
}

std::string CsvLogger::boolToString(bool value)
{
    return value ? "true" : "false";
}

std::string CsvLogger::formatRecord(const LogRecord& record) const
{
    std::ostringstream oss;
    oss << record.timestamp << ','
        << boolToString(record.safetyOk) << ','
        << boolToString(record.motorEnabled) << ','
        << static_cast<int>(record.dutyCyclePercent) << ','
        << record.currentMilliampere << ','
        << boolToString(record.overcurrent) << ','
        << boolToString(record.maintenanceDue);
    return oss.str();
}