#include <chrono>   // For sleep_for
#include <iostream> // For cout and endl
#include <mutex>    // For mutex
#include <thread>   // For thread

using namespace std;

mutex balanceMutex;
mutex logMutex;

void depositAmount(){
    cout << "Deposit thread: waiting for balanceMutex" << endl;
    balanceMutex.lock(); // Locks balanceMutex first

    cout << "Deposit thread: acquired balanceMutex" << endl;
    this_thread::sleep_for(chrono::milliseconds(500)); // Allows other thread to lock logMutex

    cout << "Deposit thread: waiting for logMutex" << endl;
    logMutex.lock(); // Waits forever if report thread owns logMutex

    cout << "Deposit thread: acquired logMutex" << endl;

    logMutex.unlock();
    balanceMutex.unlock();

    cout << "Deposit thread completed" << endl;
}

void generateReport(){
    cout << "Report thread: waiting for logMutex" << endl;
    logMutex.lock(); // Locks logMutex first

    cout << "Report thread: acquired logMutex" << endl;
    this_thread::sleep_for(chrono::milliseconds(500)); // Allows other thread to lock balanceMutex

    cout << "Report thread: waiting for balanceMutex" << endl;
    balanceMutex.lock(); // Waits forever if deposit thread owns balanceMutex

    cout << "Report thread: acquired balanceMutex" << endl;

    balanceMutex.unlock();
    logMutex.unlock();

    cout << "Report thread completed" << endl;
}

int main(){
    cout << "Main thread started" << endl;

    thread depositThread(depositAmount);
    thread reportThread(generateReport);

    depositThread.join(); // Main waits because deposit thread never completes
    reportThread.join();

    cout << "Main thread completed" << endl;

    return 0;
}