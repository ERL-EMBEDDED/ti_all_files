#include <iostream>
#include "canlib.h"
#include <libdbc.h>

void read_dbc(const std::string& file_path) {
    // Initialize the CANlib library
    canInitializeLibrary();

    // Create a DBC database handle
    DBC_HANDLE dbcHandle = dbcCreate();
    if (dbcHandle == nullptr) {
        std::cerr << "Failed to create DBC handle" << std::endl;
        return;
    }

    // Load the DBC file
    int status = dbcLoadFile(dbcHandle, file_path.c_str());
    if (status != canOK) {
        std::cerr << "Failed to load DBC file" << std::endl;
        dbcDestroy(dbcHandle);
        return;
    }

    // Get the number of messages in the DBC file
    int numMessages = dbcGetMessageCount(dbcHandle);
    std::cout << "Number of messages: " << numMessages << std::endl;

    // Iterate over each message
    for (int i = 0; i < numMessages; ++i) {
        DBC_MESSAGE message;
        status = dbcGetMessageByIndex(dbcHandle, i, &message);
        if (status != canOK) {
            std::cerr << "Failed to get message" << std::endl;
            continue;
        }

        std::cout << "Message Name: " << message.name << std::endl;
        std::cout << "  ID (hex): " << std::hex << message.id << std::dec << std::endl;
        std::cout << "  Length: " << message.dlc << std::endl;

        // Get the number of signals in the message
        int numSignals = dbcGetSignalCount(dbcHandle, message.id);
        std::cout << "  Signals:" << std::endl;

        // Iterate over each signal
        for (int j = 0; j < numSignals; ++j) {
            DBC_SIGNAL signal;
            status = dbcGetSignalByIndex(dbcHandle, message.id, j, &signal);
            if (status != canOK) {
                std::cerr << "Failed to get signal" << std::endl;
                continue;
            }

            std::cout << "    Name: " << signal.name << std::endl;
            std::cout << "    Start Bit: " << signal.startBit << std::endl;
            std::cout << "    Length: " << signal.bitLength << std::endl;
            std::cout << "    Byte Order: " << (signal.byteOrder == DBC_LITTLE_ENDIAN ? "Little Endian" : "Big Endian") << std::endl;
            std::cout << "    Signed: " << (signal.isSigned ? "Yes" : "No") << std::endl;
            std::cout << "    Scale: " << signal.factor << std::endl;
            std::cout << "    Offset: " << signal.offset << std::endl;
            std::cout << "    Minimum: " << signal.min << std::endl;
            std::cout << "    Maximum: " << signal.max << std::endl;
            std::cout << "    Unit: " << signal.unit << std::endl;

            // Check if the signal has choices (enumerations)
            if (signal.choices != nullptr) {
                std::cout << "    Choices: ";
                for (const auto& choice : *signal.choices) {
                    std::cout << choice.first << " = " << choice.second << ", ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }

    // Destroy the DBC database handle
    dbcDestroy(dbcHandle);
}

int main() {
    std::string dbcFilePath = "/home/ti/Dbc/Batch3_202108.dbc";
    read_dbc(dbcFilePath);

    return 0;
}

