#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

void rotateLogs(const std::string& logDirectory, std::size_t maxLogSize, const std::string& backupDirectory) {
    for (const auto& entry : fs::directory_iterator(logDirectory)) {
        if (fs::is_regular_file(entry) && entry.path().filename().string().find("erl") != std::string::npos) {
            const std::string& logFile = entry.path().string();
            if (fs::file_size(logFile) > maxLogSize) {
                // Create a timestamped backup directory
                auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
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
                    std::ofstream newLogFile(logFile, std::ios::app); // Open in append mode
                    if (newLogFile.is_open()) {
                        newLogFile << "Welcome to the erl log file!" << std::endl;
                        newLogFile.close();
                    } else {
                        std::cerr << "Error: Unable to open log file for writing." << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    // Handle the error gracefully (e.g., log it, continue processing, etc.)
                }
            }
        }
    }
}

int main() {
    // Configuration
    std::string logDirectory = "/var/log/";
    std::size_t maxLogSize = 1000000; // 1 MB (adjust as needed)
    std::string backupDirectory = "/var/log/backups/";

    // Rotate logs
    rotateLogs(logDirectory, maxLogSize, backupDirectory);

    return 0;
}

