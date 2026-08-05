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

For complete details, refer to: [Thread Internal Working](docs/thread-internal-working.md)

---

# 5. Passing Arguments to a Thread

Arguments are passed after the function name while creating a thread.

## Passing by Value

```cpp
void printNumber(int number);

std::thread worker(printNumber, 10);
```

`std::thread` stores a copy of the argument.

Changes made inside the thread do not affect the original variable.

```text
Original variable = 10
Thread copy       = 50
Original variable remains 10
```

If the thread is detached, the copied value remains available to the thread even after the original local variable is destroyed.

However, if `main()` returns and the process ends before the detached thread finishes, the thread is terminated and may not complete its task.

```text
Detached thread receives a copy
        ↓
Original variable may be destroyed safely
        ↓
But process may end before thread completes
```

## Passing Multiple Arguments

```cpp
void printDetails(std::string name, int experience);

std::thread worker(printDetails, "Suraj", 5);
```

Arguments must be passed in the same order as the function parameters.

## Passing by Reference

By default, `std::thread` copies arguments.

To pass the original variable by reference, use `std::ref()`.

Required header:

```cpp
#include <functional>
```

Example:

```cpp
void updateValue(int& value);

int number = 10;

std::thread worker(updateValue, std::ref(number));
```

Changes made inside the thread affect the original variable.

```text
Original variable = 10
Thread changes it = 100
Original variable becomes 100
```

The referenced variable must remain alive until the thread finishes.

If the variable is destroyed while the thread is still using it, the reference becomes a **dangling reference**.

Accessing a dangling reference causes **undefined behaviour**.

Possible results include:

```text
Incorrect value
Program crash
Memory corruption
Apparently correct execution
```

Using `join()` is normally safe because it makes the current thread wait before the local variable is destroyed.

Be careful when using references with detached threads.

## Passing by Pointer

A pointer can also be passed to a thread.

```cpp
void updateValue(int* value);

int number = 10;

std::thread worker(updateValue, &number);
```

The pointer itself is copied into the thread, but both the main thread and worker thread point to the same original variable.

```text
Main thread pointer   ──┐
                       ├── Original variable
Worker thread pointer ──┘
```

Changes made through the pointer affect the original variable.

```cpp
void updateValue(int* value){
    *value = 100;
}
```

The pointed object must remain alive until the thread finishes.

If the object is destroyed first, the pointer becomes a **dangling pointer**, and accessing it causes undefined behaviour.

## Detached Thread Comparison

### Detached Thread with Value

```cpp
int number = 10;

std::thread worker(printNumber, number);
worker.detach();
```

The thread owns a copy of `number`, so destroying the original variable does not create a dangling reference.

But if `main()` returns, the complete process ends and the detached thread may remain incomplete.

### Detached Thread with Reference

```cpp
int number = 10;

std::thread worker(updateReference, std::ref(number));
worker.detach();
```

If `number` is destroyed while the detached thread is still using it, the thread accesses a dangling reference.

This is unsafe.

### Detached Thread with Pointer

```cpp
int number = 10;

std::thread worker(updatePointer, &number);
worker.detach();
```

If `number` is destroyed while the detached thread is still using its address, the pointer becomes dangling.

This is also unsafe.

## Simple Comparison

```text
Pass by value:
Thread receives a copy.
Original variable lifetime is not required.
Process must still remain alive for the thread to finish.

Pass by reference:
Thread accesses the original variable.
Original variable must remain alive.

Pass by pointer:
Pointer is copied, but it points to the original object.
The pointed object must remain alive.
```

## Important

Passing by reference or pointer does not automatically make shared access thread-safe.

If multiple threads read and modify the same variable concurrently, synchronization such as a mutex or atomic variable may be required.

## Related File

```text
03_passing_arguments_to_thread.cpp
```
---

# 6. Creating Threads Using Lambda Functions

## Why Use Lambda Functions with Threads?

Normally, we create a separate function and pass it to `std::thread`.

```cpp
void task(){
    std::cout << "Task is running" << std::endl;
}

std::thread worker(task);
```

For a small task that is used only once, creating a separate function may be unnecessary.

A lambda allows us to write the thread task directly while creating the thread.

```cpp
std::thread worker([](){
    std::cout << "Task is running" << std::endl;
});
```

Lambda functions are useful with threads when:

* The task is small
* The task is required only once
* We do not want to create a separate function
* We need to use variables available in the current function
* We want the thread logic close to the thread creation code

This makes small thread tasks easier to read and maintain.

## Lambda Syntax

```cpp
[capture](parameters){
    // Thread task
};
```

The parts are:

```text
capture     -> Variables taken from the surrounding scope
parameters  -> Values passed while creating the thread
body        -> Work performed by the thread
```

## Lambda Without Parameters

```cpp
std::thread worker([](){
    std::cout << "Worker thread is running" << std::endl;
});
```

The lambda does not receive any parameters and does not capture any external variable.

## Lambda With Parameters

```cpp
std::thread worker([](int number){
    std::cout << "Number: " << number << std::endl;
}, 10);
```

The value `10` is passed to the lambda parameter `number`.

## Capturing a Variable by Value

```cpp
int number = 10;

std::thread worker([number](){
    std::cout << number << std::endl;
});
```

`[number]` stores a copy of `number` inside the lambda.

To modify the copied value inside the lambda, use `mutable`.

```cpp
std::thread worker([number]() mutable{
    number = 50;
});
```

The original variable outside the lambda is not changed.

## Capturing a Variable by Reference

Syntax:

```cpp
int number = 10;

std::thread worker([&number](){
    number = 100;
});
```

`[&number]` accesses the original variable.

The original variable must remain alive until the thread completes.

## Capturing a Pointer

Syntax:

```cpp
int number = 10;
int* pointer = &number;

std::thread worker([pointer](){
    *pointer = 100;
});
```

The pointer is captured by value, but it still points to the original variable.

The pointed object must remain alive until the thread completes.

## Common Capture Forms

```text
[]                   -> Capture nothing
[number]             -> Capture number by value
[&number]            -> Capture number by reference
[pointer]            -> Capture pointer by value
[=]                  -> Capture used external variables by value
[&]                  -> Capture used external variables by reference
[first, &second]     -> Capture first by value and second by reference
```

## Important

Capturing by reference or pointer does not make access thread-safe.

If multiple threads modify the same data at the same time, synchronization may be required.

## Related File

```text
04_thread_with_lambda.cpp
```
---

# 7. Race Condition

A race condition occurs when multiple threads access the same shared data at the same time, and at least one thread modifies it.

The final result depends on which thread executes first.

## Simple Example

```cpp
int counter = 0;

void increment(){
    counter++;
}
```

Suppose two threads execute `counter++`.

Expected flow:

```text
Initial counter = 0

Thread 1 increments → 1
Thread 2 increments → 2
```

Expected result:

```text
2
```

But `counter++` is not a single operation. Internally, it performs:

```text
Read counter
Add 1
Write counter
```

A possible execution is:

```text
Thread 1 reads 0
Thread 2 reads 0

Thread 1 writes 1
Thread 2 writes 1
```

Actual result:

```text
1
```

One update is lost. This is called a **race condition**.

Race conditions happen when threads modify shared data without synchronization.

The result may sometimes be correct and sometimes incorrect, depending on thread scheduling.

## Solution

Shared data can be protected using:

```text
mutex
lock_guard
atomic
```

These concepts will be covered next.

## Related File

The practical file uses a realistic warehouse inventory example:

```text
05_race_condition.cpp
```
