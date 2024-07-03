#include <cstdlib>  // For system()

int main() {
    // Execute Linux commands
    system("ip link set can0 down");
    system("ip link set can0 type can bitrate 250000");
    system("ip link set can0 up");
    // Define CAN message components
    unsigned int can_id = 0x123;
    unsigned char data1 = 0x33;
    unsigned char data2 = 0x33;

    // Construct the cansend command dynamically
    std::ostringstream command_stream;
    command_stream << "cansend can0 " << std::hex << can_id << "#";
    command_stream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(data1);
    command_stream << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(data2);
    std::string command = command_stream.str();

    // Execute the constructed command
    std::cout << "Executing command: " << command << std::endl;
    system(command.c_str());
    return 0;
}

