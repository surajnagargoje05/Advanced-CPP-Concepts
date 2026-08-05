#include <chrono>   // For sleep_for
#include <iostream> // For cout and endl
#include <mutex>    // For mutex
#include <thread>   // For thread

using namespace std;

mutex balanceMutex;
mutex logMutex;

void depositAmount(){
    cout << "Deposit thread: waiting for balanceMutex" << endl;
    balanceMutex.lock(); // First lock

    cout << "Deposit thread: acquired balanceMutex" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));

    cout << "Deposit thread: waiting for logMutex" << endl;
    logMutex.lock(); // Second lock

    cout << "Deposit thread: acquired logMutex" << endl;
    cout << "Deposit thread is updating balance and log" << endl;

    logMutex.unlock(); // Unlock second mutex first
    balanceMutex.unlock(); // Unlock first mutex last

    cout << "Deposit thread completed" << endl;
}

void generateReport(){
    cout << "Report thread: waiting for balanceMutex" << endl;
    balanceMutex.lock(); // Same first lock as deposit thread

    cout << "Report thread: acquired balanceMutex" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));

    cout << "Report thread: waiting for logMutex" << endl;
    logMutex.lock(); // Same second lock as deposit thread

    cout << "Report thread: acquired logMutex" << endl;
    cout << "Report thread is reading balance and log" << endl;

    logMutex.unlock(); // Unlock second mutex first
    balanceMutex.unlock(); // Unlock first mutex last

    cout << "Report thread completed" << endl;
}

int main(){
    cout << "Main thread started" << endl;

    thread depositThread(depositAmount);
    thread reportThread(generateReport);

    depositThread.join();
    reportThread.join();

    cout << "Main thread completed successfully" << endl;

    return 0;
}