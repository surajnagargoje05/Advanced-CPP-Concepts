#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void Producer(){
    for(int i = 0; i <= 5; i++){
        cout << "Producer produced: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void Consumer(){
    for(int i = 0; i <= 5; i++){
        cout << "Consumer produced: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
int main(){
    thread producerThread(Producer);
    thread consumerThread(Consumer);

    producerThread.join();
    consumerThread.join();
}