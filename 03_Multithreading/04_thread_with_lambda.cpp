#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

int main(){
    cout <<"Main thread started ...." << endl << endl;

    thread simpleThread([](){
        //this_thread::sleep_for(chrono::seconds(2));
        cout << "Simple worker thread is running" << endl << endl;
    });
    simpleThread.join();

    thread paramaterThread([](int empId, const string &empName){
        cout << "Calling paramaterised thread" << endl;
        //this_thread::sleep_for(chrono::seconds(2));
        cout << "Employee ID : " << empId << " Employee Name : " << empName << endl << endl;
    }, 101, "Suraj Nagargoje");
    paramaterThread.join();

    int valueNumber = 100;
    cout << "Origional Number : " << valueNumber << endl;
    thread valueCaptureThread([valueNumber]() mutable{
        valueNumber = 200;
        cout << "[Value Capture] value inside thread " << valueNumber << endl;
    });
    valueCaptureThread.join();
    cout <<"Value Number After Thread : " << valueNumber << endl << endl;

    int referenceNumber = 30;
    cout << "Original referenceNumber: " << referenceNumber << endl;
    thread referenceCaptureThread([&referenceNumber](){ // Captures original variable
        referenceNumber = 100; // Changes the original variable
        cout << "[Reference Capture] Value inside thread: " << referenceNumber << endl;
    });
    referenceCaptureThread.join(); // Keeps referenceNumber alive
    cout << "referenceNumber after thread: " << referenceNumber << endl << endl;

    int pointerNumber = 40;
    int* numberPointer = &pointerNumber;
    cout << "Original pointerNumber: " << pointerNumber << endl;
    thread pointerCaptureThread([numberPointer](){ // Captures pointer by value
        if(numberPointer != nullptr){ // Checks pointer before dereferencing
            *numberPointer = 200; // Changes the original variable
            cout << "[Pointer Capture] Value inside thread: " << *numberPointer << endl;
        }
    });
    pointerCaptureThread.join(); // Waits for pointerCaptureThread
    cout << "pointerNumber after thread: " << pointerNumber << endl << endl;
        
    int firstNumber = 5;
    int secondNumber = 15;
    thread multipleCaptureThread([firstNumber, &secondNumber](){ // Value and reference capture
        cout << "[Mixed Capture] firstNumber: " << firstNumber << endl;
        secondNumber = 50; // Changes the original secondNumber
    });
    multipleCaptureThread.join(); // Waits for multipleCaptureThread
    cout << "secondNumber after thread: " << secondNumber << endl;
    cout << "Main thread completed" << endl;


    return 0;
}