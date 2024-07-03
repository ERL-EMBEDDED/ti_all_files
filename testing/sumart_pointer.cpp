#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass(int value) : data(value) {
        std::cout << "Constructing MyClass with data: " << data << std::endl;
    }

    ~MyClass() {
        std::cout << "Destructing MyClass with data: " << data << std::endl;
    }

    void printData() {
        std::cout << "Data: " << data << std::endl;
    }

private:
    int data;
};

int main() {
    // Creating a unique pointer
    std::unique_ptr<MyClass> uniquePtr(new MyClass(42));
    uniquePtr->printData();

    // Creating a shared pointer
    std::shared_ptr<MyClass> sharedPtr1(new MyClass(100));
    std::shared_ptr<MyClass> sharedPtr2 = sharedPtr1;
    sharedPtr1->printData();
    sharedPtr2->printData();

    // Creating a weak pointer
    std::weak_ptr<MyClass> weakPtr = sharedPtr1;
    // Check if the shared pointer still exists
    if (auto sharedPtr3 = weakPtr.lock()) {
        std::cout << "WeakPtr is still pointing to MyClass with data: ";
        sharedPtr3->printData();
    } else {
        std::cout << "WeakPtr is expired." << std::endl;
    }

    // Unique pointer automatically deletes its memory when it goes out of scope
    // Shared pointers delete the memory when all shared pointers pointing to it are destroyed

    return 0;
}

