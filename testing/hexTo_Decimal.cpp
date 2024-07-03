#include <iostream>
#include <string>

int main() {
    std::string hexValue = "3333"; // Hexadecimal representation of 3333
    int decimalValue = std::stoi(hexValue, 0, 16);
    std::cout << "Decimal equivalent of hexadecimal " << hexValue << " is: " << decimalValue << std::endl;
    return 0;
}

