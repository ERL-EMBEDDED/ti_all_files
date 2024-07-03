#include <iostream>
#include <memory>
#include <thread>

class MyClass {
public:
    MyClass(int value) : data(value) {
        std::cout << "Constructor called. Value: " << data << std::endl;
    }
    
    ~MyClass() {
        std::cout << "Destructor called. Value: " << data << std::endl;
    }

    void printData() const {
        std::cout << "Data: " << data << std::endl;
    }

private:
    int data;
};

void threadFunction(std::unique_ptr<MyClass> ptr) {
    // Accessing member function using the arrow operator
    ptr->printData();
}

int main() {
    // Creating a unique pointer to MyClass
    std::unique_ptr<MyClass> ptr(new MyClass(42));

    // Creating two threads and passing the unique pointer as an argument
    std::thread thread1(threadFunction, std::move(ptr));
    std::thread thread2(threadFunction, std::make_unique<MyClass>(99));

    // Joining the threads with the main thread
    thread1.join();
    thread2.join();

    // Main thread can still access ptr if needed, but it's no longer owning the resource

    return 0;
}

