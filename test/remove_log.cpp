#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // Directory containing the log files
    std::string directory = "path_to_directory_containing_logs";

    // Pattern to match for log files
    std::string pattern = "erl_2024-04-05_18.log";

    try {
        // Iterate over files in the directory
        for (const auto& entry : fs::directory_iterator(directory)) {
            // Check if the file matches the pattern
            if (fs::is_regular_file(entry.path()) && entry.path().filename().string().find(pattern) != std::string::npos) {
                // Remove the file
                fs::remove(entry.path());
                std::cout << "Removed file: " << entry.path() << std::endl;
            }
        }
        std::cout << "Deletion completed successfully." << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

