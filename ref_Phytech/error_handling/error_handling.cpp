#include <iostream>
#include "error_handling.h"

// Function definition
void handleError(ErrorCode code, int lineNumber) {
    switch (code) {
        case INVALID_INPUT:
            std::cerr << "Error: Invalid input at line " << lineNumber << ". Please enter a valid number." << std::endl;
            break;
        case FILE_NOT_FOUND:
            std::cerr << "Error: File not found at line " << lineNumber << ". Please check the file path and try again." << std::endl;
            break;
        case DIVISION_BY_ZERO:
            std::cerr << "Error: Division by zero at line " << lineNumber << ". Please ensure the divisor is not zero." << std::endl;
            break;
        // Add more cases for other error codes as needed
        default:
            std::cerr << "Unknown error occurred at line " << lineNumber << "." << std::endl;
    }
}

