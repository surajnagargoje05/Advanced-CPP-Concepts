#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

class BankAccount{
    private:
        string accountName;
        int balance;
        mutex accountMutex;

    public:
        BankAccount(const string &name, int amount){
            accountName = name;
            balance = amount;
        }

        string getName() const{
            return accountName;
        }

        int getBalance() const{
            return balance;
        }

        mutex& getMutex(){
            return accountMutex;
        }

        void withdraw(int amount){
            balance = balance - amount;
        }

        void deposite(int amount){
            balance = balance + amount;
        }
};



void transferMoney(BankAccount &source, BankAccount &destination, int amount){
    cout << "Transfer started: " << source.getName() << " -> " << destination.getName() << endl;
    cout << "Waiting to acquire both account mutexes" << endl;
    cout << "Both account mutexes acquired" << endl;
    {
        scoped_lock lock(source.getMutex(), destination.getMutex());
        cout << "Both account mutexes acquired" << endl;
        if(source.getBalance() >= amount){
            source.withdraw(amount);
            destination.deposite(amount);
            cout << "Transferred amount: " << amount << endl;
            cout << source.getName() << " balance: " << source.getBalance() << endl;
            cout << destination.getName() << " balance: " << destination.getBalance() << endl;
        }
        else{
            cout << "Transfer failed because balance is insufficient" << endl;
        }
    }
    cout << "Transfer completed: " << source.getName() << " -> " << destination.getName() << endl;
}
int main(){
    BankAccount surajAccount("Suraj Account", 5000);
    BankAccount rahulAccount("Rahul Account", 4000);

    cout << "Initial balances" << endl;
    cout << surajAccount.getName() << ": " << surajAccount.getBalance() << endl;
    cout << rahulAccount.getName() << ": " << rahulAccount.getBalance() << endl;
    cout << endl;

    thread thread1(transferMoney, ref(surajAccount), ref(rahulAccount), 1000);
    thread thread2(transferMoney, ref(rahulAccount), ref(surajAccount), 500);

    thread1.join();
    thread2.join();

    cout << endl;
    cout << "Final balances" << endl;
    cout << surajAccount.getName() << ": " << surajAccount.getBalance() << endl;
    cout << rahulAccount.getName() << ": " << rahulAccount.getBalance() << endl;
    cout << "Main thread completed successfully" << endl;

    return 0;
}