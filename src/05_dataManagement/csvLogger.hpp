#pragma once
#include <string>
#include <cstdint>
#include <cstdint>
#include "status.hpp"

class CsvLogger {
public:
    CsvLogger(const std::string& filename,
        std::uint64_t maxFileSize);

    bool checkFileSize() const;

    void rotateLogIfNeeded();

    void logStatus(const Status& st);

private:
    std::string filename_;
    std::uint64_t maxFileSize_;
};
