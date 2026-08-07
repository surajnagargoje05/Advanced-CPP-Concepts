#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
using namespace std;

queue<int> buffer;
mutex mtx;
condition_variable cv;
bool isProduced = false;

bool isProductionCompleted = false;
void producer(){
    for(int i = 0; i <= 20 ; i++){
        this_thread::sleep_for(chrono::milliseconds(100));
        {
            lock_guard<mutex> lock(mtx);
            buffer.push(i);
            cout << "Produced : " << i << endl;
            isProduced = true;
        }
        cv.notify_one();
    }
    {
        lock_guard<mutex> lock(mtx);
        isProductionCompleted = true;
    }
    cv.notify_one();
}

void consumer(){
    while(true){
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [](){
            return isProduced || isProductionCompleted;
        });
        if(isProductionCompleted && buffer.empty()){
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
        int data = buffer.front();
        buffer.pop();
        cout << "   Consumed : " << data << endl;
        if(buffer.empty()){
            isProduced = false;
        }
    }
}

int main(){

    thread producerThread(producer);
    thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();
    return 0;
}