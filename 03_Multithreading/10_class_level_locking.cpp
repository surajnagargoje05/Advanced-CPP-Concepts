#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
using namespace std;

class BankAccount{
    private:
        string accName;
        int balance;
        static mutex classMutex;
    
    public:
        BankAccount(const string &name, int initialBalance){
            accName = name;
            balance = initialBalance;
        }

        void deposit(int amount){
            cout << accName << " is waiting for the class mutex" << endl;
            lock_guard<mutex> lock(classMutex);
            cout << accName << " acquired its class mutex" << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
            balance = balance + amount;
            cout << accName << " deposited: " << amount << endl;
            cout << accName << " updated balance: " << balance << endl;
            cout << accName << " is releasing the class mutex" << endl;
        }

        void showBalance(){
            lock_guard<mutex> lock(classMutex);
            cout << accName << " final balance: " << balance << endl;
        }
};

mutex BankAccount::classMutex;

int main(){
    BankAccount surajAccount("Suraj Account", 1000);
    BankAccount rahulAccount("Rahul Account", 2000);

    thread thread1(&BankAccount::deposit, &surajAccount, 500);
    thread thread2(&BankAccount::deposit, &rahulAccount, 700);

    thread1.join();
    thread2.join();

    surajAccount.showBalance();
    rahulAccount.showBalance();

    return 0;
}