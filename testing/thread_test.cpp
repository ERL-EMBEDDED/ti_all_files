#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx; // Mutex for protecting access to the global variable
int global_var = 0;

// Function to modify the global variable
void write_to_global() {
    for (int i = 0; i < 1000000; ++i) {
        // Lock the mutex before modifying the global variable
        std::lock_guard<std::mutex> lock(mtx);
        ++global_var;
    }
}

// Function to read from the global variable
void read_from_global() {
    for (int i = 0; i < 1000000; ++i) {
        // Lock the mutex before accessing the global variable
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << global_var << std::endl;
    }
}

int main() {
    // Create threads
    std::thread t1(write_to_global);
    std::thread t2(read_from_global);

    // Join threads with the main thread
    t1.join();
    t2.join();

    std::cout << "Done" << std::endl;

    return 0;
}

