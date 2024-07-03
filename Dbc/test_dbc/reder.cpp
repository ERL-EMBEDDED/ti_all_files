#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

// Define structures to hold the DBC information
struct Signal {
    std::string name;
    std::string unit;
    double factor;
    double offset;
    double minValue;
    double maxValue;
    int startBit;
    int length;
    bool isSigned;
    // Add more attributes as needed
};

struct Message {
    int id;
    std::string name;
    int dlc;
    std::map<std::string, Signal> signals;
};

// Function to trim whitespace from a string
std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to parse signal definition
Signal parseSignal(const std::string &line) {
    Signal signal;
    std::istringstream ss(line);
    std::string token;

    ss >> token; // Skip "SG_"
    ss >> signal.name;
    ss >> token; // Skip " : "

    std::getline(ss, token, '|');
    signal.startBit = std::stoi(trim(token));

    std::getline(ss, token, '@');
    signal.length = std::stoi(trim(token));

    // Skip the rest for simplicity. You can parse additional fields similarly
    return signal;
}

// Function to parse message definition
Message parseMessage(const std::string &line) {
    Message message;
    std::istringstream ss(line);
    std::string token;

    ss >> token; // Skip "BO_"
    ss >> message.id;
    ss >> message.name;
    ss >> token; // Skip ":"
    ss >> message.dlc;

    // Skip the rest for simplicity. You can parse additional fields similarly
    return message;
}

// Function to read DBC file
void readDBC(const std::string &filename, std::map<int, Message> &messages) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    Message currentMessage;

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (line.substr(0, 3) == "BO_") {
            currentMessage = parseMessage(line);
            messages[currentMessage.id] = currentMessage;
        } else if (line.substr(0, 3) == "SG_") {
            Signal signal = parseSignal(line);
            messages[currentMessage.id].signals[signal.name] = signal;
        }
    }

    file.close();
}

int main() {
    std::map<int, Message> messages;
    readDBC("/home/ti/Dbc/test_dbc/example.dbc", messages);

    // Print parsed messages and signals
    for (const auto &pair : messages) {
        const Message &msg = pair.second;
        std::cout << "Message ID: " << msg.id << ", Name: " << msg.name << ", DLC: " << msg.dlc << std::endl;
        for (const auto &sigPair : msg.signals) {
            const Signal &sig = sigPair.second;
            std::cout << "  Signal Name: " << sig.name << ", Start Bit: " << sig.startBit << ", Length: " << sig.length << std::endl;
        }
    }

    return 0;
}
