// can_communication.h
#ifndef CAN_COMMUNICATION_H
#define CAN_COMMUNICATION_H

struct can_ThreadArgs {
    const char *canInterface;
    int bitrate;
    const char *originalCanMessage;
    const char* verificationMessage;
    const char* expectedVerificationResponse;
};

int runCanCommunication(const char *canInterface, int bitrate, const char *originalCanMessage, const char* verificationMessage,const char* expectedVerificationResponse );
void *CanThread(void *args);

#endif // CAN_COMMUNICATION_H

