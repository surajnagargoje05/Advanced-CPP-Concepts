#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
using namespace std;


queue<int> buffer;
mutex mtx;

void Producer(){
    for(int i = 0; i <= 5; i++){
        this_thread::sleep_for(chrono::milliseconds(100));
        mtx.lock();
        cout << "Producer produced: " << i << endl;
        buffer.push(i);
        mtx.unlock();
    }
}

void Consumer(){
    for(int i = 0; i <= 5; i++){
        this_thread::sleep_for(chrono::milliseconds(100));
        mtx.lock();
        if(!buffer.empty()){
            int receivedInt = buffer.front();
            buffer.pop();
            cout << "Consumer consumed: " << receivedInt << endl;
        }
        mtx.unlock();
    }
}
int main(){
    thread producerThread(Producer);
    thread consumerThread(Consumer);

    producerThread.join();
    consumerThread.join();


    cout << "Final buffer size: " << buffer.size() << endl;
    cout << "Done!" << endl;
}