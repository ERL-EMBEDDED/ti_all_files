#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>

class DBCSignal {
public:
    std::string name;
    int startBit;
    int length;
    std::string byteOrder;
    bool isSigned;
    double scale;
    double offset;
    double minimum;
    double maximum;
    std::string unit;
    std::unordered_map<int, std::string> choices;
};

class DBCMessage {
public:
    int id;
    std::string name;
    int length;
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

        try {
            if (line.find("BO_") == 0) { // Message definition
                iss >> token; // "BO_"
                iss >> token; // Message ID
                int id = std::stoi(token);
                std::string name;
                iss >> name;
                DBCMessage message;
                message.id = id;
                iss >> message.length;
                message.name = name;
                messages[id] = message;
            } else if (line.find("SG_") == 0) { // Signal definition
                iss >> token; // "SG_"
                std::string name;
                iss >> name;
                iss >> token; // ":"
                std::string startBitLength;
                iss >> startBitLength;
                size_t separator = startBitLength.find('|');
                if (separator != std::string::npos) {
                    int startBit = std::stoi(startBitLength.substr(0, separator));
                    int length = std::stoi(startBitLength.substr(separator + 1));
                    DBCSignal signal;
                    signal.name = name;
                    signal.startBit = startBit;
                    signal.length = length;

                    // Additional signal attributes parsing
                    // You need to parse and store the additional attributes according to your DBC file format
                    
                    if (!messages.empty()) {
                        messages.begin()->second.signals[name] = signal;
                    } else {
                        std::cerr << "Signal definition found without a preceding message in line: " << line << std::endl;
                    }
                }
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number format in line: " << line << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Number out of range in line: " << line << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in line: " << line << " - " << e.what() << std::endl;
        }
    }

    return true;
}

void read_dbc(const std::string& filePath) {
    try {
        DBCFile dbcFile;
        if (!dbcFile.load(filePath)) {
            std::cerr << "Failed to load DBC file" << std::endl;
            return;
        }

        // Iterate through all messages
        for (const auto& messagePair : dbcFile.getMessages()) {
            const DBCMessage& message = messagePair.second;
            std::cout << "Message Name: " << message.name << std::endl;
            std::cout << "  ID (hex): " << std::hex << message.id << std::endl;
            std::cout << "  Length: " << message.length << std::endl;
            std::cout << "  Signals:" << std::endl;

            // Iterate through all signals of the current message
            for (const auto& signalPair : message.signals) {
                const DBCSignal& signal = signalPair.second;
                std::cout << "    Name: " << signal.name << std::endl;
                std::cout << "    Start Bit: " << signal.startBit << std::endl;
                std::cout << "    Length: " << signal.length << std::endl;
                // Print other signal attributes here as needed
                std::cout << std::endl;
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
    }
}

int main() {
    std::string dbcFilePath = "/home/ti/Dbc/Batch3_202108.dbc";
    read_dbc(dbcFilePath);
    return 0;
}
