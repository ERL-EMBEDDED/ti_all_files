#include <iostream>
#include <stdexcept> // For standard exception classes
#include "error_handling.h" // Include the header file for error handling functions

int main() {
    try {
        // Example code with potential errors
        int divisor = 0;

        // Check for division by zero
        if (divisor == 0) {
            // Throw an exception indicating division by zero
            throw std::runtime_error("Division by zero");
        }

        // More code...

    } catch (const std::exception& e) {
        // Catch exceptions and handle them
        handleException(e);
        return 1; // Exit the program with error code
    }

    return 0;
}

