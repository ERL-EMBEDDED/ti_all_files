#include <iostream>
#include <thread>
#include <mutex>

// Define global variables
int global_var1 = 0;
int global_var2 = 0;

// Define mutexes for each global variable
std::mutex mutex1;
std::mutex mutex2;

// Function for thread 1
void thread1_function() {
    // Lock mutex1 before accessing global_var1
    mutex1.lock();
    // Critical section: Read and write to global_var1
    global_var1++;
    std::cout << "Thread 1: global_var1 = " << global_var1 << std::endl;
    // Unlock mutex1
    mutex1.unlock();

    // Lock mutex2 before accessing global_var2
    mutex2.lock();
    // Critical section: Read and write to global_var2
    global_var2++;
    std::cout << "Thread 1: global_var2 = " << global_var2 << std::endl;
    // Unlock mutex2
    mutex2.unlock();
}

// Function for thread 2
void thread2_function() {
    // Lock mutex2 before accessing global_var2
    mutex2.lock();
    // Critical section: Read and write to global_var2
    global_var2++;
    std::cout << "Thread 2: global_var2 = " << global_var2 << std::endl;
    // Unlock mutex2
    mutex2.unlock();

    // Lock mutex1 before accessing global_var1
    mutex1.lock();
    // Critical section: Read and write to global_var1
    global_var1++;
    std::cout << "Thread 2: global_var1 = " << global_var1 << std::endl;
    // Unlock mutex1
    mutex1.unlock();
}

int main() {
    // Create threads
    std::thread t1(thread1_function);
    std::thread t2(thread2_function);

    // Join threads with the main thread
    t1.join();
    t2.join();

    return 0;
}

