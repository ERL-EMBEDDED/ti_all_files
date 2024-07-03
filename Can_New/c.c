#include <sp.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int sock, rc;
    struct sockaddr_sp addr;
    char buffer[1024];
    ssize_t nbytes;

    /* Create an SP socket. */
    sock = socket(PF_SP, SOCK_SUB, 0);
    assert(sock != -1);

    /* Connect it to the supplied endpoint */
    memset(&addr, 0, sizeof(addr));
    addr.ssp_family = AF_SP;
    strcpy(addr.ssp_endpoint, "tcp://127.0.0.1:1111");
    rc = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    assert(rc == 0);

    while (1) {
        /* Get current time */
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        struct tm *tm_info = localtime(&ts.tv_sec);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

        /* Receive a message */
        nbytes = recv(sock, buffer, sizeof(buffer), 0);
        assert(nbytes >= 0);

        /* Print the content with time */
        buffer[nbytes] = '\0';
        printf("[%s] Received: %s\n", time_str, buffer);
    }

    return 0;
}

