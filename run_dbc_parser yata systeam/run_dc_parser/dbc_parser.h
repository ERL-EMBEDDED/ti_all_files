#ifndef DBC_PARSER_H
#define DBC_PARSER_H

#include <string>
#include <vector>

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
std::vector<Message> parseDbcFile(const std::string& filename);
std::string messageToJson(const Message& message);
std::string signalToJson(const Signal& signal);
std::string confic_yatacan(int id,std::string FILE_NAME);
#endif // DBC_PARSER_H

