#include <iostream>
#include <thread>

// Function for the first thread
void threadFunction1() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 1 executing..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulate some work
    }
}

// Function for the second thread
void threadFunction2() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Simulate some work
    }
}

int main() {
    // Create two threads
    std::thread t1(threadFunction1);
    std::thread t2(threadFunction2);

    // Join the threads with the main thread
    t1.join();
    t2.join();

    std::cout << "Threads joined. Exiting..." << std::endl;

    return 0;
}

