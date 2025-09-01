#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <chrono>

using namespace std;

mutex mtx;
int ticketsAvailable = 10;

void bookTickets(string user){
    while(true){
        int ticketsToBook = rand() % 3 + 1;
        {
            lock_guard<mutex> lock(mtx);
            if(ticketsAvailable <= 0){
                break;
            }

            if(ticketsAvailable < ticketsToBook){
                ticketsToBook = ticketsAvailable;
            }
            ticketsAvailable -= ticketsToBook;
            cout << user << " booked " << ticketsToBook
                 << " tickets. Remaining: " << ticketsAvailable << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100));

    }
    cout << user << " finished booking." << endl;
}


int main(){
    srand(time(0));

    // Create 3 user threads
    thread user1(bookTickets, "User1");
    thread user2(bookTickets, "User2");
    thread user3(bookTickets, "User3");

    // Wait for all threads to finish
    user1.join();
    user2.join();
    user3.join();

    cout << "All tickets booked. Final tickets remaining: " << ticketsAvailable << endl;



    return 0;
}