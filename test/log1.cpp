#include <iostream>
#include <fstream>
#include <ctime>

void writeToLog(const std::string& message) {
    std::ofstream logfile;
    logfile.open("logfile.txt", std::ios_base::app); // Open the file in append mode
    if (logfile.is_open()) {
        // Get current time
        std::time_t currentTime = std::time(nullptr);
        // Convert current time to string format
        std::string timeString = std::ctime(&currentTime);
        // Remove newline character from the time string
        timeString.erase(timeString.length() - 1);

        // Write timestamp and message to the log file
        logfile << "[" << timeString << "] " << message << std::endl;
        
        logfile.close();
    } else {
        std::cerr << "Error: Unable to open logfile.txt for writing!" << std::endl;
    }
}

int main() {
    // Example usage
    writeToLog("This is a log message.");
    return 0;
}

