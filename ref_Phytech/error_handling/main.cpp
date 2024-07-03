#include <iostream>
#include "error_handling.h" // Include the header file for error handling functions

int main() {
    // Example code with potential errors
    int divisor = 0;

    // Check for division by zero
    if (divisor == 0) {  
        // Call the error handling function
        handleError(DIVISION_BY_ZERO, __LINE__);
        return 1; // Exit the program
    }

    // More code...

    return 0;
}

