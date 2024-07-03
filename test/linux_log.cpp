#include <iostream>
#include <map>
#include <string>
#include <ctime>

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

// Function to get the current hour as a string
std::string getCurrentHour() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    char buffer[3];
    std::strftime(buffer, sizeof(buffer), "%H", timeinfo);
    return std::string(buffer);
}

int main() {
    // Example usage
    std::string priorityString = "notice";

    // Check if the priority string exists in the map
    if (priorityMap.find(priorityString) != priorityMap.end()) {
        // Retrieve the priority enum value
        Priority priority = priorityMap.at(priorityString);
        
        // Log the priority
        std::cerr << "Logging priority: " << priorityString << std::endl;
        
        // Get current hour
        std::string currentHour = getCurrentHour();
        
        // Form the file name
        std::string fileName = "/var/log/backups/erl_" + currentHour + ".log";
        std::cerr << "Logging to file: " << fileName << std::endl;
        
        // Switch case to handle different priorities
        switch (priority) {
            case Priority::DEBUG:
                std::cout << "Debug information from programs\n";
                break;
            case Priority::INFO:
                std::cout << "Simple informational message - no intervention is required\n";
                break;
            case Priority::NOTICE:
                std::cout << "Condition that may require attention\n";
                break;
            case Priority::WARN:
                std::cout << "Warning\n";
                break;
            case Priority::ERR:
                std::cout << "Error\n";
                break;
            case Priority::CRIT:
                std::cout << "Critical condition\n";
                break;
            case Priority::ALERT:
                std::cout << "Condition that needs immediate intervention\n";
                break;
            case Priority::EMERG:
                std::cout << "Emergency condition\n";
                break;
        }
    } else {
        std::cerr << "Unknown priority: " << priorityString << std::endl;
    }

    return 0;
}

