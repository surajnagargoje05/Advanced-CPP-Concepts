#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;


queue<int> buffer;
mutex mtx;
condition_variable cv;
const int maxValues = 10;

void Producer(){
    for(int i = 0; i <= maxValues; i++){
        this_thread::sleep_for(chrono::milliseconds(100));
        {
            lock_guard<mutex> lock(mtx);
            cout << "Producer produced: " << i << endl;
            buffer.push(i);
        }
        cv.notify_all();
    }
}

void Consumer(){
    for(int i = 0; i <= maxValues; i++){
        this_thread::sleep_for(chrono::milliseconds(100));
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, []{return !buffer.empty();});
            if(!buffer.empty()){
                int receivedInt = buffer.front();
                buffer.pop();
                cout << "Consumer consumed: " << receivedInt << endl;
            }
        }

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