#include <iostream>
#include <fstream>
#include <chrono> // For time-related functionalities
#include <iomanip> // For formatting
#include <thread> // For multi-threading
#include <filesystem> // For file system operations

// Define a logging enum
enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

std::ofstream logfile;

// Get current timestamp in the format yyyy-mm-dd_hh-mm-ss
std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&now_c);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return oss.str();
}

// Function to rotate log file once per hour
void rotateLogFile() {
    std::string currentDateTime;
    while (true) {
        std::string newDateTime = getCurrentDateTime();
        if (newDateTime.substr(0, 13) != currentDateTime.substr(0, 13)) { // Check hour part only
            currentDateTime = newDateTime;
            if (logfile.is_open()) {
                logfile.close();
            }
            std::string logFileName = "/var/log/backups/log_" + currentDateTime + ".log";
            logfile.open(logFileName, std::ios_base::app);
            if (!logfile.is_open()) {
                std::cerr << "Error: Unable to open log file: " << logFileName << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::minutes(1)); // Check every minute
    }
}

// Define a logging function
void log(LogLevel level, const std::string& message) {
    std::string levelString;
    switch (level) {
        case LogLevel::INFO:
            levelString = "[INFO]";
            break;
        case LogLevel::WARNING:
            levelString = "[WARNING]";
            break;
        case LogLevel::ERROR:
            levelString = "[ERROR]";
            break;
    }

    // Log message with date and time
    logfile << getCurrentDateTime() << " " << levelString << ": " << message << std::endl;
}

int main() {
    // Create directory if it doesn't exist
    std::filesystem::create_directories("/var/log/backups");

    // Start a separate thread for log file rotation
    std::thread rotationThread(rotateLogFile);

    // Continuous logging in the main thread
    while (true) {
        // Example usage
        log(LogLevel::INFO, "This is an information message.");
        log(LogLevel::WARNING, "This is a warning message.");
        log(LogLevel::ERROR, "This is an error message.");

        // Sleep for a short interval
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Close the log file when done (although this part may never be reached)
    logfile.close();

    // Join the rotation thread
    rotationThread.join();

    return 0;
}

