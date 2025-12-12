#include "fileDriver.hpp"

#include <filesystem>
#include <fstream>

FileDriver::FileDriver(std::string basePath, std::size_t maxBytes)
    : basePath_{ std::move(basePath) }
    , currentPath_{ basePath_ }
    , maxBytes_{ maxBytes }
    , rotationIndex_{ 0U }
{
    ensureFileExists();
}

bool FileDriver::writeLine(const std::string& line)
{
    ensureFileExists();

    std::ofstream out(currentPath_, std::ios::app);
    if (!out) {
        return false;
    }

    out << line << '\n';
    return true;
}

std::size_t FileDriver::sizeBytes() const
{
    namespace fs = std::filesystem;

    std::error_code ec;
    const auto size = fs::file_size(currentPath_, ec);
    if (ec) {
        return 0U;
    }

    return static_cast<std::size_t>(size);
}

bool FileDriver::rotate()
{
    namespace fs = std::filesystem;

    // nothing to rotate yet; ensure target exists
    if (!fs::exists(currentPath_)) {
        ensureFileExists();
        return false;
    }

    ++rotationIndex_;
    const auto rotatedPath = basePath_ + "." + std::to_string(rotationIndex_) + ".bak";

    std::error_code ec;
    fs::rename(currentPath_, rotatedPath, ec);
    if (ec) {
        return false;
    }

    std::ofstream newFile(currentPath_, std::ios::trunc);
    return static_cast<bool>(newFile);
}

void FileDriver::ensureFileExists()
{
    namespace fs = std::filesystem;

    if (fs::exists(currentPath_)) {
        return;
    }

    std::ofstream createFile(currentPath_, std::ios::app);
    (void)createFile;
}