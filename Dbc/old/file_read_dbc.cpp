#include <iostream>
#include <fstream>
#include <dbc/DBC.h>
#include <dbc/DBCParser.h>

void read_dbc(const std::string &file_path) {
    try {
        DBC::File dbcFile;
        DBC::DBCParser parser;

        if (!parser.ParseFile(file_path, dbcFile)) {
            throw std::runtime_error("Failed to parse DBC file");
        }

        for (const auto &message : dbcFile.messages) {
            std::cout << "Message Name: " << message.name << std::endl;
            std::cout << "  ID (hex): " << std::hex << message.id << std::dec << std::endl;
            std::cout << "  Length: " << static_cast<int>(message.size) << std::endl;
            std::cout << "  Signals:" << std::endl;

            for (const auto &signal : message.signals) {
                std::cout << "    Name: " << signal.name << std::endl;
                std::cout << "    Start Bit: " << static_cast<int>(signal.startBit) << std::endl;
                std::cout << "    Length: " << static_cast<int>(signal.size) << std::endl;
                std::cout << "    Byte Order: " << (signal.byteOrder == DBC::ByteOrder::BigEndian ? "BigEndian" : "LittleEndian") << std::endl;
                std::cout << "    Signed: " << (signal.isSigned ? "True" : "False") << std::endl;
                std::cout << "    Scale: " << signal.factor << std::endl;
                std::cout << "    Offset: " << signal.offset << std::endl;
                std::cout << "    Minimum: " << signal.min << std::endl;
                std::cout << "    Maximum: " << signal.max << std::endl;
                std::cout << "    Unit: " << signal.unit << std::endl;
                std::cout << "    Choices: ";
                for (const auto &choice : signal.choices) {
                    std::cout << static_cast<int>(choice.first) << " -> " << choice.second << "; ";
                }
                std::cout << std::endl << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    std::string dbc_file_path = "/home/ti/Dbc/Batch3_202108.dbc";
    read_dbc(dbc_file_path);
    return 0;
}

