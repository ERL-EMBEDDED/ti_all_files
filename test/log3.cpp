#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h> // For stat

// Define an enum to represent the priorities
enum class Priority {
    DEBUG,
    INFO,
    NOTICE,
    WARN,
    ERR,
    CRIT,
    ALERT,
    EMERG
};

// Map to associate priority strings with their enum values
const std::map<std::string, Priority> priorityMap = {
    {"debug", Priority::DEBUG},
    {"info", Priority::INFO},
    {"notice", Priority::NOTICE},
    {"warn", Priority::WARN},
    {"err", Priority::ERR},
    {"crit", Priority::CRIT},
    {"alert", Priority::ALERT},
    {"emerg", Priority::EMERG}
};

// Forward declaration of logMessage function
void logMessage(const std::string& message, const std::string& fileName);

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
        std::cout << "Message logged to file: " << fileName << std::endl;
    } else {
        std::cerr << "Error: Unable to open log file '" << fileName << "' for writing." << std::endl;
    }
}

int main() {
    std::string currentFileName = ""; // Initialize currentFileName to an empty string
    
    // Example usage
    std::string priorityString = "notice";

    // Check if the priority string exists in the map
    if (priorityMap.find(priorityString) != priorityMap.end()) {
        // Retrieve the priority enum value
        Priority priority = priorityMap.at(priorityString);
        
        // Log the priority
        std::cerr << "Logging priority: " << priorityString << std::endl;
        
        // Switch case to handle different priorities
        switch (priority) {
            case Priority::DEBUG:
                currentFileName = "/var/log/debug.log";
                logDebug("Application started.", currentFileName);
                std::cout << "Debug information from programs\n";
                break;
            case Priority::INFO:
                currentFileName = "/var/log/info.log";
                logMessage("Some other message.", currentFileName);
                std::cout << "Simple informational message - no intervention is required\n";
                break;
            case Priority::NOTICE:
                currentFileName = "/var/log/notice.log";
                logMessage("Condition that may require attention", currentFileName);
                std::cout << "Condition that may require attention\n";
                break;
            case Priority::WARN:
                currentFileName = "/var/log/warn.log";
                std::cout << "Warning\n";
                break;
            case Priority::ERR:
                currentFileName = "/var/log/error.log";
                logError("Backup process initiated.", currentFileName);
                std::cout << "Error\n";
                break;
            case Priority::CRIT:
                currentFileName = "/var/log/critical.log";
                logCritical("Backup completed successfully.", currentFileName);
                std::cout << "Critical condition\n";
                break;
            case Priority::ALERT:
                currentFileName = "/var/log/alert.log";
                std::cout << "Condition that needs immediate intervention\n";
                break;
            case Priority::EMERG:
                currentFileName = "/var/log/emergency.log";
                std::cout << "Emergency condition\n";
                break;
        }
    } else {
        std::cerr << "Unknown priority: " << priorityString << std::endl;
    }
    
    while (true) {
        // Get current hour
        std::string currentHour = getCurrentHour();
        
        // Form the file name with current hour
        std::string fileName = "/var/log/backups/erl_" + currentHour + ".log";
        
        // If the file name changes, update current file name and log file
        if (fileName != currentFileName) {
            // Update current file name
            currentFileName = fileName;
        } else {
            // Log message into the same file every second
            logDebug("Some other message.", currentFileName);
        }
        
        // Sleep for 1 second
        sleep(1);
    }
    
    return 0;
}

