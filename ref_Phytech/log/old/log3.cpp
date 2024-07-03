#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>

std::string getCurrentHour() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H", localTime);
    
    return std::string(buffer);
}

void logDebug(const std::string& message, const std::string& fileName) {
    logMessage("[DEBUG] " + message, fileName);
}

void logError(const std::string& message, const std::string& fileName) {
    logMessage("[ERROR] " + message, fileName);
}

void logCritical(const std::string& message, const std::string& fileName) {
    logMessage("[CRITICAL] " + message, fileName);
}

void logMessage(const std::string& message, const std::string& fileName) {
    // Get current date and time
    std::string dateTime = getCurrentHour();
    
    // Open log file in append mode
    std::ofstream logFile(fileName, std::ios_base::app);
    
    // Write log message with timestamp to the file
    if (logFile.is_open()) {
        logFile << dateTime << ": " << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Error: Unable to open log file." << std::endl;
    }
}

int main() {
    std::string currentFileName = ""; // Initialize currentFileName to an empty string
    
    while (true) {
        // Get current hour
        std::string currentHour = getCurrentHour();
        
        // Form the file name with current hour
        std::string fileName = "/var/log/backups/erl_" + currentHour + ".log";
        
        // If the file name changes, update current file name and log file
        if (fileName != currentFileName) {
            // Update current file name
            currentFileName = fileName;
            
            // Log messages into the new file
            logDebug("Application started.", currentFileName);
            logDebug("Backup process initiated.", currentFileName);
            logDebug("Backup completed successfully.", currentFileName);
        } else {
            // Log message into the same file every second
            logDebug("Some other message.", currentFileName);
        }
        
        // Sleep for 1 second
        sleep(1);
    }
    
    return 0;
}

