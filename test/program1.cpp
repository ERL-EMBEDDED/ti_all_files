#include <iostream>

// Declaration of the function defined in program2.cpp
extern void function_in_program2();

int main() {
    std::cout << "Calling function in program2 from program1..." << std::endl;
    
    // Call the function defined in program2.cpp
    function_in_program2();
    
    return 0;
}

