#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int Tcpip_client(const char *server_ip, int server_port, char Tcpip_Receive_Message[]) {
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_address.sin_addr);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Receive a message from the server
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("Error receiving data");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Received message from server: %s\n", buffer);

    // Check if the received message is "400"
    if (strcmp(buffer, "400") == 0) {
        // Send "200" to the server
        const char *response = "200";
        ssize_t bytes_sent = send(client_socket, response, strlen(response), 0);
        if (bytes_sent == -1) {
            perror("Error sending data");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        printf("Sent response to the server: %s\n", response);

        // Receive Can_Data from the server
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error receiving data");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0';
        strcpy(Tcpip_Receive_Message, buffer);
        printf("Received Can_Data from server: %s\n", buffer);
    }

    // Close the connection
    close(client_socket);

    return EXIT_SUCCESS;
}

int runCommandAndGetOutput(const char *command, char *output, size_t outputSize) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("Error creating pipe");
        return EXIT_FAILURE;
    }

    pid = fork();

    if (pid == -1) {
        perror("Error forking process");
        close(pipefd[0]);
        close(pipefd[1]);
        return EXIT_FAILURE;
    }

    if (pid == 0) { // Child process
        close(pipefd[0]); // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the pipe
        close(pipefd[1]); // Close the write end of the pipe

        // Execute the command
        execl("/bin/sh", "sh", "-c", command, NULL);
        perror("Error executing command");
        _exit(EXIT_FAILURE);
    } else { // Parent process
        close(pipefd[1]); // Close the write end of the pipe
        ssize_t bytesRead = read(pipefd[0], output, outputSize - 1);
        close(pipefd[0]); // Close the read end of the pipe

        if (bytesRead == -1) {
            perror("Error reading from pipe");
            return EXIT_FAILURE;
        }

        output[bytesRead] = '\0'; // Null-terminate the output
    }

    return EXIT_SUCCESS;
}

int Can_communication(const char *canInterface, const int bitrate, const char *originalCanMessage) {
    const char* verificationMessage = "123#000400";
    const char* expectedVerificationResponse = "can0  00000123   [3]  00 02 00";
    char receivedMessage[100];  // Increase the buffer size to accommodate the received CAN message

    // Set down the can interface
    int result = system("ip link set can0 down");
    if (result != 0) {
        fprintf(stderr, "Error setting down can0 interface.\n");
        return EXIT_FAILURE;
    }

    // Set can interface type and bitrate
    char command[100];
    snprintf(command, sizeof(command), "ip link set %s type can bitrate %d", canInterface, bitrate);
    result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error setting up can0 interface.\n");
        return EXIT_FAILURE;
    }

    // Set up the can interface
    result = system("ip link set can0 up");
    if (result != 0) {
        fprintf(stderr, "Error setting up can0 interface.\n");
        return EXIT_FAILURE;
    }

    // Send verification CAN message
    snprintf(command, sizeof(command), "cansend %s %s", canInterface, verificationMessage);
    result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error sending verification CAN message.\n");
        return EXIT_FAILURE;
    }

    // Read the verification response directly into the buffer
    snprintf(command, sizeof(command), "candump %s", canInterface);
    printf("Waiting for verification response...\n");

    // Run the command and capture the output
    if (runCommandAndGetOutput(command, receivedMessage, sizeof(receivedMessage)) != EXIT_SUCCESS) {
        fprintf(stderr, "Error running command: %s\n", command);
        return EXIT_FAILURE;
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
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Verification response did not match expected response.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main() {
    char Tcpip_Receive_Message[1024];
    Tcpip_client("192.168.3.12", 9999, Tcpip_Receive_Message);
    printf("Tcpip_Receive_Message is : %s\n", Tcpip_Receive_Message);
    // Uncomment the line below if you want to test Can_communication
    Can_communication("can0", 250000, Tcpip_Receive_Message);
    return 0;
}

