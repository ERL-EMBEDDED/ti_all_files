#include <cstdlib>  // For system()

int main() {
    // Execute Linux commands
    system("ip link set can0 down");
    system("ip link set can0 type can bitrate 250000");
    system("ip link set can0 up");
    system("cansend can0 123#00003333");
    return 0;
}

