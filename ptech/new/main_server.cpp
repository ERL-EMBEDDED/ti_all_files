#include "server.h"

// Definition of host and etherNERIPort
const char *host = "192.168.3.12";
int etherNERIPort = 8888;

int main() {
    server myServer;
    myServer.initSocketConnection();
    myServer.run();
    return 0;
}

