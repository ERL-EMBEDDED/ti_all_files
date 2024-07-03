#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// Define the structure for the stack
typedef struct Stack {
    int top;
    int data[MAX_SIZE];
} Stack;

// Function to create a new stack
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->top = -1;
    return stack;
}

// Function to check if the stack is empty
int isEmpty(Stack* stack) {
    return (stack->top == -1);
}

// Function to check if the stack is full
int isFull(Stack* stack) {
    return (stack->top == MAX_SIZE - 1);
}

// Function to push an element onto the stack
void push(Stack* stack, int value) {
    if (isFull(stack)) {
        fprintf(stderr, "Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack->data[++stack->top] = value;
}

// Function to pop an element from the stack
int pop(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top--];
}

// Function to peek the top element of the stack without popping it
int peek(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top];
}

// Main function
int main() {
    Stack* stack = createStack();

    // Push some elements onto the stack
    push(stack, 10);
    push(stack, 20);
    push(stack, 30);

    // Print the top element of the stack
    printf("Top element of the stack: %d\n", peek(stack));

    // Pop an element from the stack
    printf("Popped element: %d\n", pop(stack));

    // Print the top element of the stack after popping
    printf("Top element of the stack after popping: %d\n", peek(stack));

    // Free memory
    free(stack);

    return 0;
}

