#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

// Define error codes
enum ErrorCode {
    INVALID_INPUT = 1,
    FILE_NOT_FOUND,
    DIVISION_BY_ZERO
};

// Function declaration
void handleError(ErrorCode code, int lineNumber);

#endif // ERROR_HANDLING_H

