#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

class DBCSignal {
public:
    std::string name;
    int startBit;
    int length;
};

class DBCMessage {
public:
    int id;
    std::string name;
    std::unordered_map<std::string, DBCSignal> signals;
};

class DBCFile {
public:
    bool load(const std::string& filePath);
    const std::unordered_map<int, DBCMessage>& getMessages() const { return messages; }

private:
    std::unordered_map<int, DBCMessage> messages;
};

bool DBCFile::load(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open DBC file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        if (line.find("BO_") == 0) { // Message definition
            iss >> token; // "BO_"
            iss >> token; // Message ID
            try {
                int id = std::stoi(token);
                std::string name;
                iss >> name;
                DBCMessage message;
                message.id = id;
                message.name = name;
                messages[id] = message;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid message ID in line: " << line << std::endl;
                continue;
            }
        } else if (line.find("SG_") == 0) { // Signal definition
            iss >> token; // "SG_"
            std::string name;
            iss >> name;
            iss >> token; // ":"
            std::string startBitLength;
            iss >> startBitLength;
            size_t separator = startBitLength.find('|');
            if (separator != std::string::npos) {
                try {
                    int startBit = std::stoi(startBitLength.substr(0, separator));
                    int length = std::stoi(startBitLength.substr(separator + 1));
                    DBCSignal signal;
                    signal.name = name;
                    signal.startBit = startBit;
                    signal.length = length;
                    if (!messages.empty()) {
                        messages.begin()->second.signals[name] = signal;
                    } else {
                        std::cerr << "Signal definition found without a preceding message in line: " << line << std::endl;
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid signal definition in line: " << line << std::endl;
                    continue;
                }
            }
        }
    }

    return true;
}

int main() {
    try {
        DBCFile dbcFile;
        if (!dbcFile.load("/home/ti/Dbc/Batch3_202108.dbc")) {
            std::cerr << "Failed to load DBC file" << std::endl;
            return 1;
        }

        for (const auto& messagePair : dbcFile.getMessages()) {
            const DBCMessage& message = messagePair.second;
            std::cout << "Message ID: " << message.id << ", Name: " << message.name << std::endl;

            for (const auto& signalPair : message.signals) {
                const DBCSignal& signal = signalPair.second;
                std::cout << "\tSignal Name: " << signal.name
                          << ", Start Bit: " << signal.startBit
                          << ", Length: " << signal.length << std::endl;
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
    }

    return 0;
}
