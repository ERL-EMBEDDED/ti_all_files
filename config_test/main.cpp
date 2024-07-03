#include "server_config.h"
#include <iostream>

using namespace std;

int main() {
    auto [errCode, serverIp, serverPort, backlog, logLevel] = loadConfig("config.txt");
    if (errCode != 0) {
        cerr << "Failed to load configuration, error code: " << errCode << endl;
        return errCode;
    }

    cout << "Configuration loaded successfully:\n";
    cout << "Server Ip: " << serverIp << "\n";
    cout << "Server Port: " << serverPort << "\n";
    cout << "Backlog: " << backlog << "\n";
    cout << "Log Level: " << logLevel << "\n";

    return 0;
}

