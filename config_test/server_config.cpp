#include "server_config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Utility function to trim whitespace from both ends of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::tuple<int, int, int, int, std::string> loadConfig(const std::string& filename) {
    string serverIP;
    int serverPort = 0;
    int backlog = 0;
    string logLevel;

    ifstream configFile(filename);
    if (!configFile.is_open()) {
        cerr << "Unable to open " << filename << endl;
        return {1, serverIP, serverPort, backlog, logLevel};  // Return error code 1 for file open error
    }

    string line;
    while (getline(configFile, line)) {
        // Trim whitespace from the line
        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        // Split by '=' to separate key and value
        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = trim(line.substr(0, pos));
            string value = trim(line.substr(pos + 1));

            // Print debug information
            cout << "Key: " << key << ", Value: " << value << endl;

            // Process each key-value pair
            if (key == "SERVER_IP") {
                serverIP = value;
            } else if (key == "SERVER_PORT") {
                serverPort = stoi(value);
            } else if (key == "BACKLOG") {
                backlog = stoi(value);
            } else if (key == "LOG") {
                logLevel = value;
            }
        }
    }
    configFile.close();

    // Check if all values were successfully read
    if (serverIP.empty() || serverPort == 0 || backlog == 0 || logLevel.empty()) {
        cerr << "Invalid or incomplete configuration" << endl;
        return {2, serverIP, serverPort, backlog, logLevel};  // Return error code 2 for invalid configuration
    }

    return {0, serverIP, serverPort, backlog, logLevel};  // Return error code 0 for success
}

