#include "dbc_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
            iss >> colon >> currentMessage.length;
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
            signal.is_big_endian = (endianPos != std::string::npos);

            // Find if '+' exists in the signal details
            size_t signedPos = signalDetails.find("+");
            signal.is_signed = (signedPos == std::string::npos);
            
            size_t bitStartPos = signalDetails.find(':');
            size_t bitEndPos = signalDetails.find('@');
            if (bitStartPos != std::string::npos && bitEndPos != std::string::npos) {
                std::string bitlengthStr = signalDetails.substr(bitStartPos + 1, bitEndPos - bitStartPos - 1);
                size_t bitlengthpipePos = bitlengthStr.find('|');
                if (bitlengthpipePos != std::string::npos) {
                    std::string bitStr = bitlengthStr.substr(0, bitlengthpipePos);
                    std::string lengthStr = bitlengthStr.substr(bitlengthpipePos + 1);
                    signal.start_bit = std::stoi(bitStr);
                    signal.bit_length = std::stoi(lengthStr);
                }
            }
            
            size_t unitStartPos = signalDetails.find('"');
            size_t unitEndPos = signalDetails.rfind('"');
            if (unitStartPos != std::string::npos && unitEndPos != std::string::npos) {
                signal.unit = signalDetails.substr(unitStartPos + 1, unitEndPos - unitStartPos - 1);
                signalDetails.erase(unitStartPos, unitEndPos - unitStartPos + 1);
            }
            size_t openParenPos = signalDetails.find('(');
            size_t closeParenPos = signalDetails.find(')');
            if (openParenPos != std::string::npos && closeParenPos != std::string::npos) {
                std::string scaleOffsetStr = signalDetails.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
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
// Function to convert a Signal object to a JSON string
std::string signalToJson(const Signal& signal) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"name\":\"" << signal.name << "\",";
    oss << "\"start_bit\":" << signal.start_bit << ",";
    oss << "\"bit_length\":" << signal.bit_length << ",";
    oss << "\"is_big_endian\":" << (signal.is_big_endian ? "true" : "false") << ",";
    oss << "\"is_signed\":" << (signal.is_signed ? "true" : "false") << ",";
    oss << "\"scale\":" << signal.scale << ",";
    oss << "\"offset\":" << signal.offset << ",";
    oss << "\"minimum\":" << signal.minimum << ",";
    oss << "\"maximum\":" << signal.maximum << ",";
    oss << "\"unit\":\"" << signal.unit << "\"";
    oss << "}";
    return oss.str();
}

// Function to convert a Message object to a JSON string
std::string messageToJson(const Message& message) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"name\":\"" << message.name << "\",";
    oss << "\"id_hex\":\"0x" << std::hex << message.id_hex << "\",";
    oss << "\"length\":" << message.length << ",";
    oss << "\"signals\":[";
    for (size_t i = 0; i < message.signals.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }
        oss << signalToJson(message.signals[i]);
    }
    oss << "]";
    oss << "}";
    return oss.str();
}

std::string confic_yatacan(int id,std::string FILE_NAME) {

    std::vector<Message> messages = parseDbcFile(FILE_NAME);

    // Check if the parsing was successful
    if (messages.empty()) {
        std::cerr << "Failed to parse the DBC file or no messages found." << std::endl;
        return "";
    }

    for (const auto& message : messages) {
        if (message.id_hex == id) {  // Assuming message.id_hex is an integer
            return messageToJson(message);
        }
    }

    return "";
}
