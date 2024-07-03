#include <iostream>
#include <stdexcept> // For standard exception classes
#include "error_handling.h"

// Function definition
void handleException(const std::exception& e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
}

