#include <chrono>     // For sleep_for
#include <functional> // For std::ref
#include <iostream>   // For std::cout and std::endl
#include <string>     // For std::string
#include <thread>     // For std::thread

void updateByValue(int number){
    number = 50; // Changes only the thread's copy
    std::cout << "[Value] Value inside thread: " << number << std::endl;
}

void printDetails(std::string name, int experience){
    std::cout << "[Multiple] Name: " << name
              << ", Experience: " << experience
              << " years" << std::endl;
}

void updateByReference(int& number){
    number = 100; // Changes the original variable
    std::cout << "[Reference] Value inside thread: " << number << std::endl;
}

void updateByPointer(int* number){
    if(number != nullptr){ // Check pointer before dereferencing
        *number = 200; // Changes the original variable
        std::cout << "[Pointer] Value inside thread: " << *number << std::endl;
    }
}

void detachedValueTask(int number){
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate delayed work
    std::cout << "[Detached Value] Copied value: " << number << std::endl;
}

int main(){
    int valueNumber = 10;
    int referenceNumber = 20;
    int pointerNumber = 30;

    std::cout << "Original valueNumber: " << valueNumber << std::endl;
    std::thread valueThread(updateByValue, valueNumber); // Passes a copy
    valueThread.join(); // Waits until valueThread completes
    std::cout << "valueNumber after thread: " << valueNumber << std::endl;
    std::cout << std::endl;

    std::thread detailsThread(printDetails, "Suraj", 5); // Passes multiple values
    detailsThread.join(); // Waits until detailsThread completes
    std::cout << std::endl;

    std::cout << "Original referenceNumber: " << referenceNumber << std::endl;
    std::thread referenceThread(updateByReference, std::ref(referenceNumber)); // Passes original variable by reference
    referenceThread.join(); // Keeps referenceNumber alive until thread completes
    std::cout << "referenceNumber after thread: " << referenceNumber << std::endl;
    std::cout << std::endl;

    std::cout << "Original pointerNumber: " << pointerNumber << std::endl;
    std::thread pointerThread(updateByPointer, &pointerNumber); // Passes address of original variable
    pointerThread.join(); // Keeps pointerNumber alive until thread completes
    std::cout << "pointerNumber after thread: " << pointerNumber << std::endl;
    std::cout << std::endl;
    
    std::thread detachedThread(detachedValueTask, 500); // Detached thread receives its own copy
    detachedThread.detach(); // Main thread does not wait

    std::cout << "Main continues immediately after detach" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Only to allow detached thread to print

    std::cout << "Main completed" << std::endl;

    return 0;
}