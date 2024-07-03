#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

void rotateLogs(const std::string& logDirectory, std::size_t maxLogSize, const std::string& backupDirectory, const std::string& logFilePath) {
    std::ofstream logFile(logFilePath, std::ios::app); // Open log file in append mode

    // Log start of rotation process
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "Log rotation started at " << std::ctime(&now);

    for (const auto& entry : fs::directory_iterator(logDirectory)) {
        if (fs::is_regular_file(entry) && entry.path().filename().string().find("kern.log.1") != std::string::npos) {
            const std::string& logFile = entry.path().string();
            if (fs::file_size(logFile) > maxLogSize) {
                // Create a timestamped backup directory
                std::tm timestamp;
                localtime_r(&now, &timestamp);
                char timestampStr[20];
                strftime(timestampStr, sizeof(timestampStr), "%Y%m%d%H%M%S", &timestamp);
                std::string backupDir = backupDirectory + "/" + entry.path().filename().string() + "_" + timestampStr;
                
                try {
                    fs::create_directories(backupDir);

                    // Move the log file to the backup directory
                    fs::rename(logFile, backupDir + "/" + entry.path().filename().string());

                    // Create a new empty log file with the same name
                    std::ofstream newLogFile(logFile); // Create the new empty log file
                    newLogFile << "Welcome to the erl log file!" << std::endl; // Write welcome message
                    newLogFile.close(); // Close the file
                } catch (const std::exception& e) {
                    logFile << "Error: " << e.what() << std::endl; // Log the error
                }
            }
        }
    }

    // Log end of rotation process
    now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "Log rotation completed at " << std::ctime(&now);

    // Close the log file
    logFile.close();
}

int main() {
    // Configuration
    std::string logDirectory = "/var/log/";
    std::size_t maxLogSize = 1000000; // 1 MB (adjust as needed)
    std::string backupDirectory = "/var/log/backups/";
    std::string logFilePath = "/var/log/rotation.log"; // Path to the log file

    // Rotate logs and log the process
    rotateLogs(logDirectory, maxLogSize, backupDirectory, logFilePath);

    return 0;
}

