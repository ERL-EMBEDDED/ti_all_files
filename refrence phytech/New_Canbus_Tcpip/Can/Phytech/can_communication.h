// can_communication.h
#ifndef CAN_COMMUNICATION_H
#define CAN_COMMUNICATION_H

struct ThreadArgs {
    const char *canInterface;
    int bitrate;
    const char *originalCanMessage;
};

int runCanCommunication(const char *canInterface, int bitrate, const char *originalCanMessage);
void *CanThread(void *args);

#endif // CAN_COMMUNICATION_H

