#include <iostream>
#include <memory>
#include "global.h"

// Function declarations
void function1() {
    *actualVehicleSpeed = 10;
    std::cout << "Value: " << *actualVehicleSpeed << std::endl;
    std::cout << "Data: " << *expectedVehicleSpeed << std::endl;
}

void function2() {
    *expectedVehicleSpeed = 20;
    std::cout << "Value: " << *actualVehicleSpeed << std::endl;
    std::cout << "Data: " << *expectedVehicleSpeed << std::endl;
}

int main() {
    // Call function1 with smart pointers
    function1();
    
    // Call function2 with smart pointers
    function2();
    
    return 0;
}

