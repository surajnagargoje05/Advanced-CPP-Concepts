#include <iostream>
#include <thread>
#include <chrono>
#include <queue>

using namespace std;


queue<int> buffer;
void Producer(){
    for(int i = 0; i <= 5; i++){
        cout << "Producer produced: " << i << endl;
        buffer.push(i);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void Consumer(){
    for(int i = 0; i <= 5; i++){
        if(!buffer.empty()){
            int receivedInt = buffer.front();
            buffer.pop();
            cout << "Consumer consumed: " << receivedInt << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
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