#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
using namespace std;

queue<int> buffer;
mutex mtx;
condition_variable notEmpty;
condition_variable notFull;
const size_t BUFFER_SIZE = 5;
bool productionCompleted = false;


void producer(){
    for(int i = 0; i <= 20; i++){
        this_thread::sleep_for(chrono::milliseconds(50));
        unique_lock<mutex> lock(mtx);
        notFull.wait(lock, [](){
            return buffer.size() < BUFFER_SIZE;
        });
        buffer.push(i);
        cout << "Produced : " << i << " Buffer size : " << buffer.size() << endl;
        lock.unlock();
        notEmpty.notify_one();
    }
    {
        lock_guard<mutex> lock(mtx);
        productionCompleted = true;
    }
    notEmpty.notify_one();
}

void consumer(){
    while(true){
        unique_lock<mutex> lock(mtx);
        notEmpty.wait(lock, [](){
            return !buffer.empty() || productionCompleted;
        });
        if(buffer.empty() && productionCompleted){
            break;
        }
        int value = buffer.front();
        cout << "   Consumed : " << value << "  Buffer size : " << buffer.size() << endl;
        buffer.pop();
        lock.unlock();
        notFull.notify_one();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}
int main(){

    thread producerThread(producer);
    thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();
    return 0;
}