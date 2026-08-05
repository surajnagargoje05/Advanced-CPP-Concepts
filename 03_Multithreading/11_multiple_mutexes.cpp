#include <chrono>   // For sleep_for
#include <iostream> // For cout and endl
#include <mutex>    // For mutex and lock_guard
#include <string>   // For string
#include <thread>   // For thread

using namespace std;

int accountBalance = 1000; // Shared balance resource
string latestTransaction = "No transaction"; // Shared log resource

mutex balanceMutex; // Protects accountBalance
mutex logMutex; // Protects latestTransaction

void updateBalance(int amount){
    cout << "Balance thread is waiting for balance mutex" << endl;
    lock_guard<mutex> lock(balanceMutex); // Locks only balance resource
    cout << "Balance thread acquired balance mutex" << endl;
    int currentBalance = accountBalance;
    this_thread::sleep_for(chrono::milliseconds(500));
    accountBalance = currentBalance + amount;
    cout << "Balance updated to: " << accountBalance << endl;
} // balanceMutex is automatically unlocked here

void updateTransactionLog(const string& message){
    cout << "Log thread is waiting for log mutex" << endl;
    lock_guard<mutex> lock(logMutex); // Locks only log resource
    cout << "Log thread acquired log mutex" << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    latestTransaction = message;
    cout << "Transaction log updated: " << latestTransaction << endl;
} // logMutex is automatically unlocked here

void depositAmount(int amount){
    cout << "Deposit thread is waiting for balance mutex" << endl;
    balanceMutex.lock(); // First lock
    cout << "Deposit thread acquired balance mutex" << endl;
    cout << "Deposit thread is waiting for log mutex" << endl;
    logMutex.lock(); // Second lock
    cout << "Deposit thread acquired log mutex" << endl;
    accountBalance = accountBalance + amount;
    latestTransaction = "Deposited amount: " + to_string(amount);
    cout << "Deposit completed" << endl;
    cout << "Updated balance: " << accountBalance << endl;
    cout << "Updated log: " << latestTransaction << endl;
    logMutex.unlock(); // Unlock second mutex first
    cout << "Deposit thread released log mutex" << endl;
    balanceMutex.unlock(); // Unlock first mutex last
    cout << "Deposit thread released balance mutex" << endl;
}

int main(){
    cout << "Initial balance: " << accountBalance << endl;
    cout << "Initial log: " << latestTransaction << endl;
    cout << endl;

    thread balanceThread(updateBalance, 500);
    thread transactionThread(updateTransactionLog, "Balance update requested");

    balanceThread.join();
    transactionThread.join();

    cout << endl;
    cout << "After independent operations" << endl;
    cout << "Balance: " << accountBalance << endl;
    cout << "Log: " << latestTransaction << endl;
    cout << endl;

    thread depositThread(depositAmount, 300);

    depositThread.join();

    cout << endl;
    cout << "Final balance: " << accountBalance << endl;
    cout << "Final log: " << latestTransaction << endl;

    return 0;
}