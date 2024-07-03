#include <stdio.h>

// Define a constant using a macro
#define MAX_SIZE 100

// Define a macro for a function-like operation
#define SQUARE(x) ((x) * (x))

int main() {
    int data[MAX_SIZE];

    // Use the constant defined by the macro
    printf("Maximum size: %d\n", MAX_SIZE);

    // Use the macro for a function-like operation
    int num = 5;
    printf("Square of %d is %d\n", num, SQUARE(num));

    return 0;
}

