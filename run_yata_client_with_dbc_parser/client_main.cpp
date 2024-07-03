// client_main.cpp
#include "client.h"

int main() {
    epsClient client;
    client.initSocket();
    if (client.connectToServer()) {
        
        client.run();
    }
   // client.closeConnection();
    return 0;
}

