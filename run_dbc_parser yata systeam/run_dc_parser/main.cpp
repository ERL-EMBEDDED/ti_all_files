#include "dbc_parser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip> // For std::hex
#include <string>


int main() {
    int id =0x28a;
    std::string FILE_NAME="/home/erl/Documents/run_dbc_parser yata systeam/Batch3_202108.dbc";
    std::string jsonString = confic_yatacan(id,FILE_NAME);
    if (!jsonString.empty()) {
        
        std::cout<< "Dbc file read"<<std::endl;
    
        std::cout << jsonString << std::endl;
    } else {
        std::cerr << "Message with ID 0x28a not found." << std::endl;
    }

    return 0;
}

