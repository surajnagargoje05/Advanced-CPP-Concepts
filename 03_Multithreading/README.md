# C++ Multithreading Learning

This repository is created to learn and revise C++ multithreading concepts.

* Theory will be maintained in this `README.md`
* Practical examples will be stored in separate `.cpp` files
* Files will follow a numbered learning sequence

Example:

```text
01_basic_thread_creation.cpp
02_thread_with_function.cpp
03_join_and_detach.cpp
04_mutex_basic.cpp
```

---
# What Is a Thread?

A thread is the smallest unit of execution inside a running program.

When a C++ program starts, the operating system creates a process, and the `main()` function runs on the **main thread**.

```text
Process
└── Main thread
```

We can create additional threads to perform different tasks.

```text
Process
├── Main thread
├── Worker thread 1
└── Worker thread 2
```

Each thread executes its own sequence of instructions.

For example:

```text
Main thread   : Controls the main program flow
Worker thread : Performs another task
```

Multiple threads can make progress concurrently. On a multi-core processor, different threads may run at the same time on different CPU cores. This is called **parallelism**.

```text
CPU Core 1 → Main thread
CPU Core 2 → Worker thread
```

Threads inside the same process share resources such as heap memory, global variables, objects, files, and buffers. However, every thread has its own stack, local variables, and execution state.

In simple words:

```text
A process is a running program.
A thread is an execution path inside that program.
Multiple threads can perform different tasks concurrently,
and they may run in parallel on multiple CPU cores.
```
---

# 1. Why Do We Need Threading?

By default, a C++ program runs using one main thread.

In a single-threaded program, tasks execute one after another.

```text
Read data
Process data
Send data
Write logs
```

If one task takes more time or waits for input, all other tasks must wait.

With multithreading, different tasks can run concurrently.

```text
Thread 1: Read data
Thread 2: Process data
Thread 3: Send data
Thread 4: Write logs
```

## Main Benefits

* Perform multiple tasks concurrently
* Keep the application responsive
* Handle blocking operations like socket or serial read
* Use multiple CPU cores
* Separate different responsibilities

Example from a robotics application:

```text
Thread 1: Read serial data
Thread 2: Monitor safety
Thread 3: Communicate with server
```

Threading does not always make a program faster. It can also introduce problems like race conditions, deadlocks, and data corruption.

## Related File

```text
01_basic_thread_creation.cpp
```
# 2. How to Create a Thread

In C++, we create a thread using the `std::thread` class.

Required header:

```cpp
#include <thread>
```

## Thread with Non-Parameterized Function

```cpp
void printMessage();

std::thread thread1(printMessage);
```

The new thread executes the `printMessage()` function.

## Thread with Parameterized Function

```cpp
void printNumber(int number);

std::thread thread2(printNumber, 10);
```

The arguments are passed after the function name.

## Waiting for Threads

```cpp
thread1.join();
thread2.join();
```

`join()` makes the main thread wait until the worker thread completes.

## Related File

```text
01_thread_creation.cpp
```
# 3. `join()` and `detach()`

Remember that `main()` also runs as a thread.

When we create another thread, the program has:

```text
Main thread
Worker thread
```

## `join()`

```cpp
worker.join();
```

When the main thread reaches `join()`, it waits at that line until the worker thread completes.

```cpp
worker.join();

std::cout << "Suraj is developer\n";
```

If the worker takes 5 seconds, the print statement also waits for approximately 5 seconds.

Execution flow:

```text
Worker starts
Main reaches join()
Main waits
Worker completes
Main continues
"Suraj is developer" prints
Main returns
```

```text
join() -> Wait here until the worker completes.
```

## `detach()`

```cpp
worker.detach();
```

When the main thread reaches `detach()`, it does not wait for the worker.

```cpp
worker.detach();

std::cout << "Suraj is developer\n";
```

The print statement executes immediately.

Execution flow:

```text
Worker starts
Main calls detach()
Main continues immediately
"Suraj is developer" prints
Main continues
```

```text
detach() -> Do not wait; execute the next line immediately.
```

## What Happens When `main()` Returns?

Consider this code:

```cpp
void task()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Worker completed\n";
}

int main()
{
    std::thread worker(task);

    worker.detach();

    std::cout << "Suraj is developer\n";

    return 0;
}
```

Possible output:

```text
Suraj is developer
```

The main thread does not wait for the detached thread.

When `return 0` executes:

```text
Main thread completes
Complete process ends
Detached thread is stopped
```

Therefore, `Worker completed` may never print because the worker needs 5 seconds, but `main()` finishes immediately.

Adding sleep in `main()` may allow the detached thread to finish:

```cpp
worker.detach();

std::cout << "Suraj is developer\n";

std::this_thread::sleep_for(std::chrono::seconds(6));

return 0;
```

However, this is only for demonstration. Using sleep is not a proper replacement for `join()`.

## Simple Difference

```text
join()   -> Main waits for the worker.
detach() -> Main does not wait for the worker.
```

```text
With join:
Worker completes -> Main completes -> Process ends

With detach:
Main may complete first -> Process ends -> Worker may remain incomplete
```

## Important

A thread should normally be joined or detached before its `std::thread` object is destroyed.

If neither is called, the program calls:

```cpp
std::terminate();
```

Use `detach()` carefully because the detached thread must not access data that may be destroyed, and the program may end before the detached thread completes.

## Related File

```text
02_join_and_detach.cpp
```
---

# 4. Thread Internal Working

Before learning synchronization concepts, it is important to understand how threads work internally at the operating-system level.

This topic includes:

* Process and thread relationship
* How `std::thread` creates a thread internally
* System calls used by threads
* Shared and separate thread memory
* Thread stack and heap
* CPU registers and instruction pointer
* Thread scheduling and states
* Context switching
* Concurrency and parallelism
* Internal working of `sleep_for()`, `join()`, and `detach()`
* Basic reason behind race conditions and mutexes

For complete details, refer to:

[Thread Internal Working](docs/thread-internal-working.md)
