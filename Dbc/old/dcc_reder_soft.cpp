#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

// Define struct to represent signal information
struct Signal {
    std::string name;
    int start_bit;
    int bit_length;
    bool is_big_endian;
    bool is_signed;
    double scale;
    double offset;
    double minimum;
    double maximum;
    std::string unit;
};

// Define struct to represent message information
struct Message {
    std::string name;
    int id_hex;
    int length;
    std::vector<Signal> signals;
};

// Function to parse .dbc file and extract messages and signals
std::vector<Message> parseDbcFile(const std::string& filename) {
    std::vector<Message> messages;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return messages;
    }
    std::string line;
    Message currentMessage;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        if (token == "BO_") {
            if (!currentMessage.name.empty()) {
                messages.push_back(currentMessage);
            }
            // Parsing ID, Name, and Length
            iss >> currentMessage.id_hex;
            iss >> currentMessage.name;
            std::string colon;
            iss >> currentMessage.length;
            // Resetting signals for new message
            currentMessage.signals.clear();
        } else if (token == "SG_") {
            Signal signal;
            // Parsing Signal Name
            std::string sgName;
            iss >> sgName;
            // Handling case where signal name contains spaces
            while (sgName.back() == ':') {
                std::string temp;
                iss >> temp;
                sgName += " " + temp;
            }
            signal.name = sgName;

            // Parsing the signal details
            std::string signalDetails;
            std::getline(iss >> std::ws, signalDetails);
            
            size_t endianPos = signalDetails.find("@");
            if (endianPos != std::string::npos) {
                signal.is_big_endian = true;
            } else {
                signal.is_big_endian = false;
            }

            // Find if '+' exists in the signal details
            size_t signedPos = signalDetails.find("+");
            if (signedPos != std::string::npos) {
                signal.is_signed = false;
            } else {
                signal.is_signed = false;
            }
            
            size_t bitStartPos = signalDetails.find(':');
            size_t bitEndPos = signalDetails.find('@');
            if (bitStartPos != std::string::npos && bitEndPos != std::string::npos) {
                std::string bitlengthStr = signalDetails.substr(bitStartPos + 1, bitEndPos - bitStartPos - 1);
                //std::cout << "  start_bit/length: " << bitlengthStr << std::endl;
                size_t bitlengthpipePos = bitlengthStr.find('|');
                if (bitlengthpipePos != std::string::npos) {
                    std::string bitStr = bitlengthStr.substr(0, bitlengthpipePos);
                    std::string lengthStr = bitlengthStr.substr(bitlengthpipePos + 1);
                    signal.start_bit = std::stod(bitStr);
                    signal.bit_length = std::stod(lengthStr);
                }
            }
            
            size_t unitStartPos = signalDetails.find('"');
            size_t unitEndPos = signalDetails.rfind('"');
            if (unitStartPos != std::string::npos && unitEndPos != std::string::npos) {
                signal.unit = signalDetails.substr(unitStartPos + 1, unitEndPos - unitStartPos - 1);
                signalDetails.erase(unitStartPos, unitEndPos - unitStartPos + 1);
                //std::cout << "  unit: " << signal.unit << std::endl;
            }
            size_t openParenPos = signalDetails.find('(');
            size_t closeParenPos = signalDetails.find(')');
            if (openParenPos != std::string::npos && closeParenPos != std::string::npos) {
                std::string scaleOffsetStr = signalDetails.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                //std::cout << "  Scale/Offset String: " << scaleOffsetStr << std::endl;
                size_t commaPos = scaleOffsetStr.find(',');
                if (commaPos != std::string::npos) {
                    std::string scaleStr = scaleOffsetStr.substr(0, commaPos);
                    std::string offsetStr = scaleOffsetStr.substr(commaPos + 1);
                    signal.scale = std::stod(scaleStr);
                    signal.offset = std::stod(offsetStr);
                }
                signalDetails.erase(openParenPos, closeParenPos - openParenPos + 1);
            }
            size_t bracketStartPos = signalDetails.find('[');
            size_t bracketEndPos = signalDetails.find(']');
            if (bracketStartPos != std::string::npos && bracketEndPos != std::string::npos) {
                std::string minMaxStr = signalDetails.substr(bracketStartPos + 1, bracketEndPos - bracketStartPos - 1);
                //std::cout << "  Min/Max String: " << minMaxStr << std::endl;
                size_t minMaxpipePos = minMaxStr.find('|');
                if (minMaxpipePos != std::string::npos) {
                    std::string minStr = minMaxStr.substr(0, minMaxpipePos);
                    std::string maxStr = minMaxStr.substr(minMaxpipePos + 1);
                    signal.minimum = std::stod(minStr);
                    signal.maximum = std::stod(maxStr);
                }
            }
            

            // Adding signal to current message
            currentMessage.signals.push_back(signal);
        }
    }
    if (!currentMessage.name.empty()) {
        messages.push_back(currentMessage);
    }
    file.close();
    return messages;
}
int main() {
    // Replace "your_dbc_file.dbc" with the path to your DBC file
    std::string dbcFilename = "/home/ti/Dbc/Batch3_202108.dbc";
    std::vector<Message> messages = parseDbcFile(dbcFilename);
    
    // Output the parsed messages and signals
    for (const auto& message : messages) {
        std::cout << "Message Name: " << message.name << std::endl;
        std::cout << "ID (hex): 0x" << std::hex << message.id_hex << std::dec << std::endl;
        std::cout << "Length: " << message.length << std::endl;
        std::cout << "Signals:" << std::endl;
        for (const auto& signal : message.signals) {
            std::cout << "  Name: " << signal.name << std::endl;
            std::cout << "  Start Bit: " << signal.start_bit << std::endl;
            std::cout << "  Length: " << signal.bit_length << std::endl;
            std::cout << "  Byte Order: " << (signal.is_big_endian ? "big_endian" : "little_endian") << std::endl;
            std::cout << "  Signed: " << (signal.is_signed ? "True" : "False") << std::endl;
            std::cout << "  Scale: " << signal.scale << std::endl;
            std::cout << "  Offset: " << signal.offset << std::endl;
            std::cout << "  Minimum: " << signal.minimum << std::endl;
            std::cout << "  Maximum: " << signal.maximum << std::endl;
            std::cout << "  Unit: " << signal.unit << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}

