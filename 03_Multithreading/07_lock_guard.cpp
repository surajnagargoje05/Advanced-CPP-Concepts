#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

int availableStock = 100;
mutex stockMutex;

void processOrder(const string &orderName, int quantity){
    cout << orderName << " Started Processing order" << endl;
    lock_guard<mutex> lock(stockMutex);
    int currentStock = availableStock;
    cout << orderName << " read available stock: " << currentStock << endl;
    this_thread::sleep_for(chrono::milliseconds(100));
    if(currentStock > quantity){
        currentStock = currentStock - quantity;
        availableStock = currentStock;
        cout << orderName << " sold " << quantity << " products" << endl;
        cout << orderName << " updated stock to: " << availableStock << endl;
    }
    else{
        cout << orderName << " failed because stock is insufficient" << endl;
    }
    cout << orderName << " is releasing the stock mutex" << endl;
}

int main(){
    cout << "Initial warehouse stock: " << availableStock << endl;

    thread orderThread1(processOrder,"Order 1", 30);
    thread orderThread2(processOrder,"Order 2", 40);

    orderThread1.join();
    orderThread2.join();

    cout << endl;
    cout << "Expected remaining stock: 30" << endl;
    cout << "Actual remaining stock: " << availableStock << endl;

    return 0;
}