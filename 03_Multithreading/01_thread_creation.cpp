#include <iostream>
#include <thread>
using namespace std;

void printMessage(){
    cout << "Non Paramaterised function is running ...." << endl;
}

void printNumber(int num, const string &message){
    cout << "Message : " << message << " Number : " << num << endl;
}

int main(){
    thread thread1(printMessage);

    thread thread2(printNumber, 100, "Dummy Message ");

    thread1.join();
    thread2.join();
    
    return 0;
}