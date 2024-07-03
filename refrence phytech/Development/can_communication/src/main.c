// main.c
#include <stdio.h>
#include "can_communication.h"

int main() {
    const char *canInterface = "can0";
    int bitrate = 250000;
    const char *originalCanMessage = "123#000400";

    int result = runCanCommunication(canInterface, bitrate, originalCanMessage);

    if (result != 0) {
        fprintf(stderr, "Error in runCanCommunication.\n");
        return 1;
    }

    return 0;
}

