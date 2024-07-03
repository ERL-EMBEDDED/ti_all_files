// can_communication.c
#include "can_communication.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int runCanCommunication(const char *canInterface, int bitrate, const char *originalCanMessage ,const char* verificationMessage, const char* expectedVerificationResponse ) {
    //const char* verificationMessage = "123#000400";
    //const char* expectedVerificationResponse = "can0  00000123   [3]  00 02 00";
    char receivedMessage[100];

    // Set down the can interface
    int result = system("ip link set can0 down");
    if (result != 0) {
        fprintf(stderr, "Error setting down can0 interface.\n");
        return 1;
    }

    // Set can interface type and bitrate
    char command[100];
    snprintf(command, sizeof(command), "ip link set %s type can bitrate %d", canInterface, bitrate);
    result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error setting up can0 interface.\n");
        return 1;
    }

    // Set up the can interface
    result = system("ip link set can0 up");
    if (result != 0) {
        fprintf(stderr, "Error setting up can0 interface.\n");
        return 1;
    }

    // Send verification CAN message
    snprintf(command, sizeof(command), "cansend %s %s", canInterface, verificationMessage);
    result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error sending verification CAN message.\n");
        return 1;
    }

    // Read the verification response
    snprintf(command, sizeof(command), "candump %s", canInterface);
    printf("Waiting for verification response...\n");

    // Capture the output of the candump command
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        return 1;
    }

    // Read the verification response from the pipe
    if (fgets(receivedMessage, sizeof(receivedMessage), fp) == NULL) {
        perror("Error reading verification response");
        pclose(fp);
        return 1;
    }

    // Print the received CAN message
    printf("Received CAN message: %s\n", receivedMessage);

    // Compare the received verification response
    if (strstr(receivedMessage, expectedVerificationResponse) != NULL) {
        printf("Verification Success \n");

        // Verification response matched, send the original CAN message
        snprintf(command, sizeof(command), "cansend %s %s", canInterface, originalCanMessage);
        result = system(command);
        if (result != 0) {
            fprintf(stderr, "Error sending original CAN message.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Verification response did not match expected response.\n");
        return 1;
    }

    pclose(fp);
    return 0;
}

void *CanThread(void *args) {
    struct can_ThreadArgs *threadArgs = (struct can_ThreadArgs *)args;
    runCanCommunication(threadArgs->canInterface, threadArgs->bitrate, threadArgs->originalCanMessage, threadArgs->verificationMessage, threadArgs->expectedVerificationResponse);
    return NULL;
}

