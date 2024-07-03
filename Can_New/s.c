#include <sp.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int sock, rc;
    struct sockaddr_sp addr;
    char buffer[1024];

    /* Create an SP socket. */
    sock = socket(PF_SP, SOCK_PUB, 0);
    assert(sock != -1);

    /* Bind the socket to the endpoint */
    memset(&addr, 0, sizeof(addr));
    addr.ssp_family = AF_SP;
    strcpy(addr.ssp_endpoint, "tcp://127.0.0.1:1111");
    rc = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    assert(rc == 0);

    while (1) {
        /* Get current time */
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        struct tm *tm_info = localtime(&ts.tv_sec);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

        /* Input message */
        printf("Enter message to publish [%s]: ", time_str);
        fgets(buffer, sizeof(buffer), stdin);

        /* Publish the message */
        rc = send(sock, buffer, strlen(buffer), 0);
        assert(rc != -1);
    }

    return 0;
}

