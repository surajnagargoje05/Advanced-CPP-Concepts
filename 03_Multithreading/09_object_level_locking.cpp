#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
using namespace std;

class BankAccount{
    private:
        string accName;
        int balance;
        mutex accMutex;
    
    public:
        BankAccount(const string &name, int initialBalance){
            accName = name;
            balance = initialBalance;
        }

        void deposite(int amount){
            cout << accName << " is waiting for its object mutex" << endl;
            lock_guard<mutex> lock(accMutex);
            cout << accName << " acquired its object mutex" << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
            balance = balance + amount;
            cout << accName << " deposited: " << amount << endl;
            cout << accName << " updated balance: " << balance << endl;
        }

        void showBalance(){
            lock_guard<mutex> lock(accMutex);
            cout << accName << " final balance: " << balance << endl;
        }
};
int main(){
    BankAccount surajAccount("Suraj Account", 1000);
    BankAccount rahulAccount("Rahul Account", 2000);

    thread thread1(&BankAccount::deposite, &surajAccount, 500);
    thread thread2(&BankAccount::deposite, &rahulAccount, 700);

    thread1.join();
    thread2.join();

    surajAccount.showBalance();
    rahulAccount.showBalance();

    return 0;
}