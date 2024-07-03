#include <iostream>
#include <dbc_parser_cpp/DBCFileLoader.h>

int main() {
    dbc::DBCFileLoader loader;
    dbc::Network dbc_network = loader.loadDBCFile("/home/ti/Dbc/Batch3_202108.dbc");

    for (const auto& message : dbc_network.messages) {
        std::cout << "Message Name: " << message.name << std::endl;
        std::cout << "  ID (hex): " << std::hex << message.id << std::dec << std::endl;
        std::cout << "  Length: " << message.size << std::endl;
        std::cout << "  Signals:" << std::endl;

        for (const auto& signal : message.signals) {
            std::cout << "    Name: " << signal.name << std::endl;
            std::cout << "    Start Bit: " << signal.startBit << std::endl;
            std::cout << "    Length: " << signal.size << std::endl;
            std::cout << "    Byte Order: " << (signal.byteOrder == dbc::ByteOrder::BigEndian ? "BigEndian" : "LittleEndian") << std::endl;
            std::cout << "    Signed: " << (signal.signedFlag ? "True" : "False") << std::endl;
            std::cout << "    Scale: " << signal.factor << std::endl;
            std::cout << "    Offset: " << signal.offset << std::endl;
            std::cout << "    Minimum: " << signal.min << std::endl;
            std::cout << "    Maximum: " << signal.max << std::endl;
            std::cout << "    Unit: " << signal.unit << std::endl;
            std::cout << "    Choices: ";
            for (const auto& choice : signal.choices) {
                std::cout << choice.first << " -> " << choice.second << "; ";
            }
            std::cout << std::endl << std::endl;
        }
    }

    return 0;
}

