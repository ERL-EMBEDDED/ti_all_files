#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iomanip>

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
    DBCMessage currentMessage;
    bool messageStarted = false;

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
                name = name.substr(0, name.size() - 1); // Remove trailing ':'
                int length;
                iss >> length;

                currentMessage = DBCMessage{id, name, length};
                messageStarted = true;
                messages[id] = currentMessage;

            } else if (line.find("SG_") == 0 && messageStarted) { // Signal definition
                iss >> token; // "SG_"
                std::string name;
                iss >> name;
                name = name.substr(0, name.size() - 1); // Remove trailing ':'
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
                    std::string byteOrderSigned;
                    iss >> byteOrderSigned;
                    signal.byteOrder = byteOrderSigned[0] == '0' ? "big_endian" : "little_endian";
                    signal.isSigned = byteOrderSigned[1] == '-';

                    std::string scaleOffset;
                    iss >> scaleOffset;
                    size_t scaleSep = scaleOffset.find(',');
                    signal.scale = std::stod(scaleOffset.substr(1, scaleSep - 1)); // Remove '('
                    signal.offset = std::stod(scaleOffset.substr(scaleSep + 1, scaleOffset.size() - scaleSep - 2)); // Remove ')'

                    std::string minMax;
                    iss >> minMax;
                    size_t minMaxSep = minMax.find('|');
                    signal.minimum = std::stod(minMax.substr(1, minMaxSep - 1)); // Remove '['
                    signal.maximum = std::stod(minMax.substr(minMaxSep + 1, minMax.size() - minMaxSep - 2)); // Remove ']'

                    iss >> signal.unit;

                    // Parse choices if any
                    std::string choiceToken;
                    while (iss >> choiceToken) {
                        if (choiceToken.find("Vector__XXX") != std::string::npos) break;
                        size_t choiceSep = choiceToken.find('=');
                        if (choiceSep != std::string::npos) {
                            int key = std::stoi(choiceToken.substr(0, choiceSep));
                            std::string value = choiceToken.substr(choiceSep + 1);
                            signal.choices[key] = value;
                        }
                    }

                    messages[currentMessage.id].signals[name] = signal;
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

        for (const auto& messagePair : dbcFile.getMessages()) {
            const DBCMessage& message = messagePair.second;
            for (const auto& signalPair : message.signals) {
                const DBCSignal& signal = signalPair.second;
                std::cout << "Name: " << signal.name << std::endl;
                std::cout << "Start Bit: " << signal.startBit << std::endl;
                std::cout << "Length: " << signal.length << std::endl;
                std::cout << "Byte Order: " << signal.byteOrder << std::endl;
                std::cout << "Signed: " << (signal.isSigned ? "True" : "False") << std::endl;
                std::cout << "Scale: " << signal.scale << std::endl;
                std::cout << "Offset: " << signal.offset << std::endl;
                std::cout << "Minimum: " << signal.minimum << std::endl;
                std::cout << "Maximum: " << signal.maximum << std::endl;
                std::cout << "Unit: " << signal.unit << std::endl;

                if (!signal.choices.empty()) {
                    std::cout << "Choices:" << std::endl;
                    for (const auto& choice : signal.choices) {
                        std::cout << "  " << choice.first << " : " << choice.second << std::endl;
                    }
                } else {
                    std::cout << "Choices: None" << std::endl;
                }
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
    std::string dbcFilePath = "/home/ti/Dbc/Batch3_202108.dbc"; // Replace with the actual path to your DBC file
    read_dbc(dbcFilePath);
    return 0;
}

