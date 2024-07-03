#include <iostream>

int main() {
    int receiveValue = 13107;

    // Divide receiveValue by 65535 and multiply by 500
    double scaledValue = (static_cast<double>(receiveValue) / 65535.0) * 500.0;

    std::cout << "Scaled Value: " << scaledValue << std::endl;

    return 0;
}
void CanInterface::decodeAndSend() {

       int receiveValue = receiveCanData();

     // Divide receiveValue by 65535 and multiply by 500
    double Value = (static_cast<double>(receiveValue) / 65535.0) * 500.0;
    std::cout << "Scaled Value: " << Value << std::endl;


}

