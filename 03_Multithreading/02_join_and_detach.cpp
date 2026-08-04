#include <chrono>
#include <iostream>
#include <thread>

void joinedTask()
{
    std::cout << "[JOIN] Worker thread started\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "[JOIN] Worker thread completed\n";
}

void detachedTask()
{
    std::cout << "[DETACH] Worker thread started\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "[DETACH] Worker thread completed\n";
}

int main()
{
    std::cout << "Main thread started\n\n";

    std::cout << "Creating joined thread\n";

    std::thread thread1(joinedTask);

    std::cout << "Main thread is calling join()\n";

    thread1.join();                                     // Main thread waits here for thread1 to complete

    std::cout << "Suraj is developer - printed after join\n\n";     // This line executes only after thread1 completes

    std::cout << "Creating detached thread\n";

    std::thread thread2(detachedTask);

    thread2.join();                                   // Main thread does not wait for thread2

    std::cout << "Suraj is developer - printed immediately after detach\n";      // This line executes immediately

    std::cout << "Main thread is continuing\n";

    // Added only so that we can see detached thread completion
    // std::this_thread::sleep_for(std::chrono::seconds(6));

    std::cout << "Main thread completed\n";

    return 0;
}