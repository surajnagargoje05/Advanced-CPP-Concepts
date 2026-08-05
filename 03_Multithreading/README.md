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
---

# 8. Mutex

A mutex is used to protect shared data from being accessed by multiple threads at the same time.

Mutex means **mutual exclusion**.

It allows only one thread at a time to enter the protected section of code.

## Why Do We Need a Mutex?

Suppose two threads modify the same variable:

```cpp
int counter = 0;

void increment(){
    counter++;
}
```

Both threads may read and update `counter` at the same time, causing a race condition.

A mutex prevents this by allowing only one thread to modify the shared variable at a time.

## Required Header

```cpp
#include <mutex>
```

## Basic Syntax

```cpp
mutex mtx;

mtx.lock();

// Protected shared-data operation

mtx.unlock();
```

The code between `lock()` and `unlock()` is called a **critical section**.

## How It Works

```text
Thread 1 locks the mutex
Thread 1 enters the critical section

Thread 2 tries to lock the mutex
Thread 2 waits because the mutex is already locked

Thread 1 completes its work
Thread 1 unlocks the mutex

Thread 2 gets the mutex
Thread 2 enters the critical section
```

Only the thread that successfully locks the mutex should unlock it.

## Example

```cpp
mutex mtx;
int counter = 0;

void increment(){
    mtx.lock();

    counter++;

    mtx.unlock();
}
```

Now only one thread can execute `counter++` at a time.

## Important Problem with Manual `lock()` and `unlock()`

If the function returns early or an exception occurs before `unlock()`, the mutex may remain locked.

```cpp
mtx.lock();

if(error){
    return; // Mutex is never unlocked
}

mtx.unlock();
```

Other threads may then wait forever.

This can cause a deadlock.

Because of this, `lock_guard` is usually safer than manually calling `lock()` and `unlock()`.

`lock_guard` will be covered in the next topic.

## Related File

The practical example fixes the warehouse inventory race condition using a mutex:

```text
06_mutex.cpp
```
---

# 9. `lock_guard`

Manual `lock()` and `unlock()` are risky because we may forget to unlock the mutex.

Example:

```cpp
stockMutex.lock();

if(error){
    return; // Mutex remains locked
}

stockMutex.unlock();
```

If the function returns early or an exception occurs, `unlock()` may not execute.

This can make other threads wait forever.

## Why Use `lock_guard`?

`lock_guard` automatically:

```text
Locks the mutex when created
Unlocks the mutex when the scope ends
```

Required header:

```cpp
#include <mutex>
```

Basic syntax:

```cpp
lock_guard<mutex> lock(mtx);
```

Example:

```cpp
mutex mtx;

void update(){
    lock_guard<mutex> lock(mtx);

    // Critical section
}
```

The mutex is locked when `lock` is created.

When the function or block ends, `lock_guard` is destroyed and the mutex is automatically unlocked.

## Scope Example

```cpp
{
    lock_guard<mutex> lock(mtx);

    // Mutex is locked here
}

// Mutex is automatically unlocked here
```

## Main Benefit

```text
Manual lock/unlock:
Developer must unlock manually.

lock_guard:
Mutex is unlocked automatically.
```

`lock_guard` is safer for simple critical sections.

## Important

`lock_guard` cannot be manually unlocked before the scope ends.

When more control is required, `unique_lock` can be used.

## Related File

The practical example protects warehouse stock using `lock_guard`:

```text
07_lock_guard.cpp
```
---

# 10. `unique_lock`

## Why Do We Need `unique_lock` When `lock_guard` Already Exists?

`lock_guard` is useful when we want to lock a mutex at the beginning of a scope and unlock it automatically at the end.

```cpp
{
    lock_guard<mutex> lock(mtx);

    // Mutex remains locked in this complete scope
}
```

This is simple and safe.

However, sometimes we do not want the mutex to remain locked for the complete function or scope.

We may need to:

```text
Lock the mutex later
Unlock the mutex before the scope ends
Lock the mutex again
Transfer lock ownership
Wait using condition_variable
```

`lock_guard` does not provide these operations.

For such cases, we use `unique_lock`.

In simple words:

```text
lock_guard  -> Simple automatic locking
unique_lock -> Automatic locking with extra control
```

---

## Realistic Example

Suppose an order-processing function performs three tasks:

```text
1. Process payment
2. Check and update stock
3. Package the order
```

Only the stock update needs protection.

Payment processing and packaging do not use the shared stock.

If we use `lock_guard` for the complete function:

```cpp
void processOrder(){
    lock_guard<mutex> lock(stockMutex);

    processPayment();
    updateStock();
    packageOrder();
}
```

The mutex remains locked during payment and packaging.

Other threads must wait even though the shared stock is not being used.

This reduces concurrency.

The better flow is:

```text
Process payment without mutex
Lock mutex
Check and update stock
Unlock mutex
Package order without mutex
```

`unique_lock` allows us to do this.

---

## Where Should We Use `unique_lock`?

Use `unique_lock` when:

* The mutex should not be locked immediately
* The mutex must be unlocked before the scope ends
* The mutex needs to be locked again
* Only a specific part of a large function needs protection
* Lock ownership needs to be moved
* A `condition_variable` is being used

For a small and simple critical section, prefer `lock_guard`.

---

## Basic Syntax

Required header:

```cpp
#include <mutex>
```

Create a mutex:

```cpp
mutex mtx;
```

Create a `unique_lock`:

```cpp
unique_lock<mutex> lock(mtx);
```

The mutex is locked immediately.

```cpp
void updateData(){
    unique_lock<mutex> lock(mtx);

    // Mutex is locked here
}
```

When the function ends, `unique_lock` automatically unlocks the mutex.

---

## Automatic Unlocking

```cpp
void updateData(){
    unique_lock<mutex> lock(mtx);

    // Protected code
}
```

Execution:

```text
unique_lock is created
        ↓
Mutex is locked
        ↓
Protected code executes
        ↓
Function ends
        ↓
unique_lock is destroyed
        ↓
Mutex is automatically unlocked
```

This provides the same basic safety as `lock_guard`.

---

## Manual Unlock

Unlike `lock_guard`, `unique_lock` can unlock the mutex before the scope ends.

```cpp
unique_lock<mutex> lock(mtx);

// Protected code

lock.unlock();

// Unprotected code
```

After `lock.unlock()`, another waiting thread can acquire the mutex.

Example:

```cpp
void processOrder(){
    unique_lock<mutex> lock(stockMutex);

    availableStock = availableStock - 10;

    lock.unlock();

    cout << "Packaging order" << endl;
}
```

The packaging work runs without holding the stock mutex.

---

## Locking Again

After unlocking, the same `unique_lock` can lock the mutex again.

```cpp
unique_lock<mutex> lock(mtx);

lock.unlock();

// Other work

lock.lock();

// Protected work again
```

Example flow:

```text
Lock mutex
Update shared data
Unlock mutex
Perform independent work
Lock mutex again
Update shared data again
Unlock automatically when scope ends
```

---

## Delayed Locking with `defer_lock`

By default, `unique_lock` locks the mutex immediately.

```cpp
unique_lock<mutex> lock(mtx);
```

Sometimes we want to create the lock object now but lock the mutex later.

For this, use `defer_lock`.

```cpp
unique_lock<mutex> lock(mtx, defer_lock);
```

At this point, the mutex is not locked.

Later:

```cpp
lock.lock();
```

Example:

```cpp
void processOrder(){
    unique_lock<mutex> lock(stockMutex, defer_lock);

    cout << "Processing payment" << endl;

    lock.lock();

    availableStock = availableStock - 10;

    lock.unlock();

    cout << "Packaging order" << endl;
}
```

Execution:

```text
unique_lock object created
Mutex is not locked

Payment processing starts

lock.lock() is called
Mutex is locked

Stock is updated

lock.unlock() is called
Mutex is unlocked

Packaging starts
```

---

## `owns_lock()`

We can check whether the `unique_lock` currently owns the mutex.

```cpp
if(lock.owns_lock()){
    cout << "Mutex is locked by this unique_lock" << endl;
}
```

Example:

```cpp
unique_lock<mutex> lock(mtx, defer_lock);

cout << lock.owns_lock() << endl;

lock.lock();

cout << lock.owns_lock() << endl;
```

Possible output:

```text
0
1
```

`0` means it does not own the mutex.

`1` means it owns the mutex.

---

## `lock_guard` vs `unique_lock`

```text
lock_guard:
Locks immediately
Unlocks automatically
Cannot manually unlock
Cannot lock again
Simple and lightweight
Best for small critical sections

unique_lock:
Can lock immediately or later
Can unlock manually
Can lock again
Can check lock ownership
Can transfer ownership
Required with condition_variable
More flexible than lock_guard
```

---

## Which One Should We Prefer?

Use `lock_guard` when the complete scope needs protection.

```cpp
void update(){
    lock_guard<mutex> lock(mtx);

    sharedData++;
}
```

Use `unique_lock` when only part of the function needs protection.

```cpp
void update(){
    performIndependentWork();

    unique_lock<mutex> lock(mtx);

    sharedData++;

    lock.unlock();

    performMoreIndependentWork();
}
```

Simple rule:

```text
Need only automatic lock and unlock?
Use lock_guard.

Need more control over locking and unlocking?
Use unique_lock.
```

---

## Why Is `unique_lock` Required with `condition_variable`?

A `condition_variable` may temporarily unlock the mutex while a thread is waiting.

Later, when the condition becomes true, it locks the mutex again before continuing.

For this behaviour, the lock object must support:

```text
Unlock
Wait
Lock again
```

`lock_guard` cannot manually unlock and lock again.

`unique_lock` can do this.

That is why `condition_variable` normally works with `unique_lock`.

Example syntax:

```cpp
unique_lock<mutex> lock(mtx);

cv.wait(lock);
```

The detailed working of `condition_variable` will be covered later.

---

## Important Points

Do not call `unlock()` if the `unique_lock` does not own the mutex.

```cpp
unique_lock<mutex> lock(mtx, defer_lock);

lock.unlock(); // Incorrect because mutex is not locked
```

Do not call `lock()` again when the same `unique_lock` already owns the mutex.

```cpp
unique_lock<mutex> lock(mtx);

lock.lock(); // Incorrect because it is already locked
```

Prefer to keep the protected section as small as possible.

Do not hold a mutex during slow operations such as:

```text
Network calls
File operations
Long calculations
Sleep
Payment processing
Logging to a slow external system
```

unless those operations genuinely require shared-data protection.

---

## Final Summary

```text
lock_guard is simple and safe.

unique_lock provides the same automatic unlocking,
but it also gives extra control.

Use unique_lock when we need to:
Lock later
Unlock early
Lock again
Check ownership
Use condition_variable
```

## Related File

```text
08_unique_lock.cpp
```
---

---

# 11. Object-Level Locking

Object-level locking means every object has its own separate mutex.

The mutex is declared as a normal, non-static data member of the class.

```cpp
class BankAccount{
private:
    int balance;
    mutex accountMutex;
};
```

When multiple objects are created, every object receives its own mutex.

```cpp
BankAccount account1(1000);
BankAccount account2(2000);
```

Conceptually:

```text
account1
├── balance
└── mutex1

account2
├── balance
└── mutex2
```

If one thread locks `account1`, another thread can still work on `account2`.

```text
Thread 1 → Locks account1
Thread 2 → Locks account2

Both threads can run concurrently.
```

However, two threads cannot modify the same object at the same time.

```text
Thread 1 → Locks account1
Thread 2 → Also wants account1

Thread 2 must wait.
```

## Basic Syntax

```cpp
class BankAccount{
private:
    int balance;
    mutex accountMutex;

public:
    void deposit(int amount){
        lock_guard<mutex> lock(accountMutex);

        balance = balance + amount;
    }
};
```

Since `accountMutex` is non-static, every `BankAccount` object has a different mutex.

## Where Is Object-Level Locking Useful?

Use object-level locking when every object manages independent data.

Examples:

```text
Each bank account has a separate balance
Each robot has a separate state
Each sensor has separate data
Each order has a separate status
Each device has separate configuration
```

## Main Benefit

Object-level locking provides better concurrency.

```text
Different objects → Can be processed together
Same object       → Only one thread can modify it at a time
```

## Important

The mutex should normally be private so that the class controls access to its own data.

Object-level locking protects only that object's data. It does not protect static variables or other shared resources used by all objects.

## Related File

```text
09_object_level_locking.cpp
```
---

# 12. Class-Level Locking

Class-level locking means all objects of a class share one common mutex.

The mutex is declared as a `static` data member.

```cpp
class BankAccount{
private:
    int balance;
    static mutex classMutex;
};
```

A static mutex belongs to the class, not to one specific object.

```cpp
BankAccount account1(1000);
BankAccount account2(2000);
```

Conceptually:

```text
account1 ──┐
           ├── One common class mutex
account2 ──┘
```

If one thread locks the class mutex while working on `account1`, another thread working on `account2` must also wait.

```text
Thread 1 → Locks common mutex for account1
Thread 2 → Wants common mutex for account2

Thread 2 must wait.
```

## Basic Syntax

```cpp
class BankAccount{
private:
    int balance;
    static mutex classMutex;

public:
    void deposit(int amount){
        lock_guard<mutex> lock(classMutex);

        balance = balance + amount;
    }
};

mutex BankAccount::classMutex;
```

The static mutex must be defined once outside the class.

## Where Is Class-Level Locking Useful?

Use class-level locking when all objects access one common shared resource.

Examples:

```text
Common log file
Shared database connection
Static counter
Shared hardware device
Global configuration
Common transaction history
```

## Main Behaviour

```text
Different objects → Still use the same mutex
Same object       → Uses the same mutex
```

So only one protected operation can run at a time across all objects.

## Object-Level vs Class-Level

```text
Object-level locking:
Each object has its own mutex.
Different objects can work concurrently.

Class-level locking:
All objects share one mutex.
Different objects may block each other.
```

## Important

Class-level locking may reduce concurrency because unrelated objects also wait for the same mutex.

Use it only when the protected resource is genuinely shared by all objects.

## Related File

```text
10_class_level_locking.cpp
```
---

# 13. Multiple Mutexes

Multiple mutexes are used when a program has multiple independent shared resources.

Each mutex protects one specific shared resource.

Example:

```cpp
int balance = 1000;
string transactionLog;

mutex balanceMutex;
mutex logMutex;
```

Here:

```text
balanceMutex → Protects balance
logMutex     → Protects transactionLog
```

## Why Use Separate Mutexes?

Suppose one thread updates the balance and another thread writes a log.

With separate mutexes:

```text
Thread 1 locks balanceMutex
Thread 2 locks logMutex

Both threads can run concurrently.
```

Independent resources do not block each other unnecessarily.

## When One Operation Needs Multiple Mutexes

Sometimes one operation needs multiple shared resources.

For example, a deposit operation must:

```text
1. Update the balance
2. Update the transaction log
```

Therefore, the function needs both mutexes.

```cpp
balanceMutex.lock();
logMutex.lock();

balance = balance + amount;
transactionLog = "Deposit completed";

logMutex.unlock();
balanceMutex.unlock();
```

## Locking Sequence

When multiple mutexes are required, all threads should lock them in the same order.

Example order:

```text
First  → balanceMutex
Second → logMutex
```

```cpp
balanceMutex.lock();
logMutex.lock();
```

If another thread uses the opposite order, deadlock may occur.

```text
Thread 1:
Locks balanceMutex
Waits for logMutex

Thread 2:
Locks logMutex
Waits for balanceMutex
```

## Unlocking Sequence

A common practice is to unlock mutexes in the reverse order of locking.

If mutexes were locked like this:

```text
First  → balanceMutex
Second → logMutex
```

Unlock them like this:

```text
First  → logMutex
Second → balanceMutex
```

Code:

```cpp
balanceMutex.lock();
logMutex.lock();

// Critical section

logMutex.unlock();
balanceMutex.unlock();
```

This follows the **last locked, first unlocked** approach.

```text
Lock order:
balanceMutex → logMutex

Unlock order:
logMutex → balanceMutex
```

Reverse unlocking keeps resource ownership clear, especially when locks depend on each other.

For simple independent mutexes, reverse unlocking is not always technically required, but it is a good and consistent practice.

## Simple Rule

```text
Lock mutexes in a fixed order.

Unlock mutexes in the reverse order.
```

## Benefits

* Independent resources can run concurrently
* Unnecessary blocking is reduced
* Each mutex protects a specific resource
* Fixed lock order reduces deadlock risk
* Reverse unlock order keeps locking logic clear

## Important

Manual `lock()` and `unlock()` are risky if an early return or exception occurs.

Later, safer methods such as these should be preferred:

```text
std::lock()
scoped_lock
unique_lock with defer_lock
```

## Related File

```text
11_multiple_mutexes.cpp
```
---

# 14. `chrono` and `sleep_for()`

`std::chrono` provides time-related types in C++.

`this_thread::sleep_for()` pauses only the thread that calls it for a specified duration.

Required headers:

```cpp
#include <chrono>
#include <thread>
```

Example syntax:

```cpp
this_thread::sleep_for(chrono::milliseconds(500));
```

This pauses the current thread for approximately `500` milliseconds.

## What Happens During Sleep?

```text
Thread is running
      ↓
sleep_for() is called
      ↓
Thread enters sleeping state
      ↓
Specified time completes
      ↓
Thread becomes ready
      ↓
Scheduler runs it again
```

While one thread is sleeping, other threads can continue running.

```text
Thread 1 → Sleeping
Thread 2 → Running
Main     → Running or waiting
```

## Why Do We Use Sleep in Examples?

In learning examples, sleep is used to simulate operations that take time.

Examples:

```text
Payment processing
Network response
Sensor reading
File operation
Database request
Order processing
```

It also gives other threads a chance to run, making thread execution order easier to observe.

For example:

```text
Thread 1 performs one step
Thread 1 sleeps
Thread 2 gets CPU time
Thread 2 performs its work
Thread 1 wakes up and continues
```

## What Happens Without Sleep?

Without sleep, one thread may complete its work very quickly before another thread gets CPU time.

Example:

```text
Thread 1 starts
Thread 1 completes
Thread 2 starts
```

With sleep, execution may look like:

```text
Thread 1 starts
Thread 1 sleeps
Thread 2 starts
Thread 2 performs work
Thread 1 wakes up
```

Sleep helps us clearly observe concurrent thread behaviour during testing and learning.

## Where Is `sleep_for()` Used?

Common uses include:

```text
Simulating slow operations
Adding delay between retries
Running periodic tasks
Waiting before the next sensor read
Testing timing behaviour
Reducing repeated CPU polling
```

Example periodic flow:

```text
Read sensor
Sleep for 100 milliseconds
Read sensor again
```

## Common Time Durations

```cpp
chrono::nanoseconds
chrono::microseconds
chrono::milliseconds
chrono::seconds
chrono::minutes
chrono::hours
```

Examples:

```cpp
this_thread::sleep_for(chrono::milliseconds(100));
this_thread::sleep_for(chrono::seconds(2));
```

## Important

`sleep_for()` pauses the thread for at least approximately the requested time.

The thread may resume slightly later because the operating-system scheduler decides when it gets CPU time again.

```text
sleep_for(100 milliseconds)

does not guarantee:
Resume at exactly 100 milliseconds

It means:
Do not run this thread for approximately 100 milliseconds
```

## Final Summary

```text
sleep_for() pauses only the calling thread.

Other threads can continue running.

It is useful for delays, periodic work,
testing and simulating time-consuming operations.

Without sleep, one thread may finish before
another thread gets a chance to run.
```
---

# 15. Deadlock

A deadlock occurs when two or more threads wait for each other forever, and none of them can continue.

Deadlock commonly happens when multiple mutexes are locked in different orders.

## Simple Example

Suppose we have two mutexes:

```cpp
mutex balanceMutex;
mutex logMutex;
```

Two threads use them differently:

```text
Thread 1:
Locks balanceMutex
Then tries to lock logMutex

Thread 2:
Locks logMutex
Then tries to lock balanceMutex
```

Possible execution:

```text
Thread 1 locks balanceMutex
Thread 2 locks logMutex

Thread 1 waits for logMutex
Thread 2 waits for balanceMutex
```

Now:

```text
Thread 1 cannot continue until Thread 2 releases logMutex.

Thread 2 cannot continue until Thread 1 releases balanceMutex.
```

Neither thread can continue or release its first mutex.

This situation is called a deadlock.

## Deadlock Flow

```text
Thread 1 owns balanceMutex
Thread 1 waits for logMutex

Thread 2 owns logMutex
Thread 2 waits for balanceMutex

Both threads wait forever.
```

## Why Does `main()` Also Appear Stuck?

If `main()` calls:

```cpp
thread1.join();
thread2.join();
```

the main thread waits for both worker threads to complete.

But the worker threads are deadlocked and never complete.

Therefore, the main thread also waits forever at `join()`.

## Common Deadlock Conditions

Deadlock can occur when:

* Multiple threads use multiple mutexes
* A thread holds one mutex while waiting for another
* Mutexes are locked in different orders
* The mutex cannot be taken away automatically
* Threads keep waiting without releasing their current mutex

## How to Prevent Deadlock

Common solutions are:

```text
Use the same lock order in every thread
Use std::lock() to lock multiple mutexes
Use scoped_lock
Keep critical sections small
Avoid holding one mutex while waiting for another resource
```

Example of consistent lock order:

```text
Thread 1: balanceMutex → logMutex
Thread 2: balanceMutex → logMutex
```

Both threads request the mutexes in the same order, reducing deadlock risk.

## Important

Deadlock does not normally crash the program.

The program remains running, but the affected threads stop making progress.

```text
Program is running
Threads are waiting
No further work is completed
```

## Related Files

```text
12_deadlock_occurrence.cpp
```

This file intentionally creates a deadlock by locking the same two mutexes in opposite order.

```text
Thread 1: balanceMutex → logMutex
Thread 2: logMutex → balanceMutex
```

The program becomes stuck because both threads wait for each other.

```text
13_deadlock_resolved.cpp
```

This file resolves the deadlock by using the same locking order in both threads.

```text
Thread 1: balanceMutex → logMutex
Thread 2: bala## Related File

```text
12_deadlock.cppnceMutex → logMutex
```

One thread may wait, but eventually both threads complete successfully.

```
---

# 16. Preventing Deadlock Using `std::lock()`

## Why Do We Need `std::lock()`?

When one operation needs multiple mutexes, we may lock them manually.

```cpp
balanceMutex.lock();
logMutex.lock();
```

Manual locking is risky because different threads may lock the same mutexes in different orders.

```text
Thread 1:
balanceMutex → logMutex

Thread 2:
logMutex → balanceMutex
```

This can create a deadlock.

`std::lock()` is used to acquire multiple mutexes while avoiding deadlock caused by different locking orders.

Required header:

```cpp
#include <mutex>
```

## Basic Syntax

```cpp
lock(mutex1, mutex2);
```

After this statement completes successfully, the current thread owns both mutexes.

```text
Current thread owns:
mutex1
mutex2
```

## Manual Locking vs `std::lock()`

### Manual Locking

```cpp
balanceMutex.lock();
logMutex.lock();
```

The developer must maintain the same lock order everywhere.

If another thread uses the opposite order, deadlock may occur.

### Using `std::lock()`

```cpp
lock(balanceMutex, logMutex);
```

`std::lock()` tries to acquire all provided mutexes without creating a deadlock between those lock attempts.

Conceptually:

```text
Try to acquire all mutexes
          ↓
Could not acquire all
          ↓
Release any mutex already acquired
          ↓
Try again
```

The exact internal implementation depends on the C++ standard library.

## Why Use `unique_lock` with `defer_lock`?

We also need automatic unlocking.

Normally, `unique_lock` immediately locks its mutex:

```cpp
unique_lock<mutex> lock1(balanceMutex);
```

But we want `std::lock()` to acquire both mutexes together.

Therefore, we create the `unique_lock` objects using `defer_lock`.

```cpp
unique_lock<mutex> lock1(balanceMutex, defer_lock);
unique_lock<mutex> lock2(logMutex, defer_lock);
```

`defer_lock` means:

```text
Create the unique_lock object,
but do not lock the mutex yet.
```

Then both locks are passed to `std::lock()`:

```cpp
lock(lock1, lock2);
```

## Complete Pattern

```cpp
unique_lock<mutex> lock1(balanceMutex, defer_lock);
unique_lock<mutex> lock2(logMutex, defer_lock);

lock(lock1, lock2);

// Critical section
```

Execution flow:

```text
lock1 is created
balanceMutex is not locked

lock2 is created
logMutex is not locked

std::lock() is called
Both mutexes are acquired safely

Critical section executes

Scope ends
Both unique_lock objects are destroyed
Both mutexes are automatically unlocked
```

## Responsibilities

```text
mutex:
Protects the shared resource

unique_lock:
Manages ownership and automatic unlocking

defer_lock:
Prevents immediate locking

std::lock():
Acquires multiple locks without deadlock
```

## Realistic Example: Money Transfer

Suppose money must be transferred between two bank accounts.

The operation must:

```text
1. Deduct money from the source account
2. Add money to the destination account
```

Both accounts must be protected during the complete transfer.

```text
Transfer A to B:
Needs Account A mutex and Account B mutex

Transfer B to A:
Needs Account B mutex and Account A mutex
```

Manual locking may create opposite lock orders.

Using `std::lock()` allows both account locks to be acquired safely.

## Important

`std::lock()` does not automatically unlock mutexes when raw mutex objects are passed.

This requires manual unlocking:

```cpp
lock(mutex1, mutex2);

// Critical section

mutex2.unlock();
mutex1.unlock();
```

Using `unique_lock` with `defer_lock` is safer because both mutexes are unlocked automatically when the scope ends.

## When Should We Use `std::lock()`?

Use it when one operation needs multiple mutexes together.

Examples:

```text
Transfer money between two accounts
Move an item between two queues
Access sensor data and robot state together
Update balance and transaction history
Copy data between two shared objects
```

## Important Limitation

`std::lock()` prevents deadlock while acquiring the mutexes passed in the same call.

It does not automatically fix every possible deadlock in the complete application.

Deadlocks can still happen because of:

```text
Other mutexes
Nested locking
Incorrect application design
Waiting for external resources
Holding locks during callbacks
```

## Final Summary

```text
Manual multiple locking:
Developer controls the lock order.
Wrong order may cause deadlock.

std::lock():
Acquires multiple locks without deadlock.

unique_lock with defer_lock:
Creates lock managers without immediately locking.

When the scope ends:
unique_lock automatically releases the mutexes.
```

Remember this pattern:

```cpp
unique_lock<mutex> lock1(mutex1, defer_lock);
unique_lock<mutex> lock2(mutex2, defer_lock);

lock(lock1, lock2);
```

## Related File

```text
14_std_lock.cpp
```
---

# 17. `scoped_lock`

## Why Do We Need `scoped_lock`?

We previously used `std::lock()` with `unique_lock` to safely lock multiple mutexes.

```cpp
unique_lock<mutex> lock1(mutex1, defer_lock);
unique_lock<mutex> lock2(mutex2, defer_lock);

lock(lock1, lock2);
```

This works correctly, but it requires multiple lines and several objects.

In C++17, `scoped_lock` provides a simpler way to lock one or multiple mutexes safely.

```cpp
scoped_lock lock(mutex1, mutex2);
```

In simple words:

```text
std::lock() + unique_lock → Detailed and flexible approach

scoped_lock               → Simple approach for locking
                            multiple mutexes
```

## Required Header

```cpp
#include <mutex>
```

`scoped_lock` is available from C++17.

Compile using:

```bash
g++ -std=c++17 15_scoped_lock.cpp -pthread
```

## Basic Syntax

For one mutex:

```cpp
scoped_lock lock(mutex1);
```

For multiple mutexes:

```cpp
scoped_lock lock(mutex1, mutex2);
```

For three mutexes:

```cpp
scoped_lock lock(mutex1, mutex2, mutex3);
```

When the `scoped_lock` object is created, it locks the provided mutexes.

When the scope ends, it automatically unlocks them.

## How Does It Work?

```cpp
{
    scoped_lock lock(mutex1, mutex2);

    // Both mutexes are locked here
}

// Both mutexes are automatically unlocked here
```

Execution flow:

```text
scoped_lock object is created
        ↓
All provided mutexes are locked
        ↓
Critical section executes
        ↓
Scope ends
        ↓
scoped_lock object is destroyed
        ↓
All mutexes are automatically unlocked
```

## Why Is It Safer Than Manual Locking?

Manual locking may look like this:

```cpp
mutex1.lock();
mutex2.lock();

// Critical section

mutex2.unlock();
mutex1.unlock();
```

Problems with manual locking:

```text
Developer may use the wrong lock order
Developer may forget to unlock
Early return may skip unlock()
Exception may skip unlock()
Deadlock may occur
```

With `scoped_lock`:

```cpp
scoped_lock lock(mutex1, mutex2);
```

The locking and unlocking are handled automatically.

## Deadlock-Safe Multiple Locking

Suppose two threads transfer money in opposite directions.

```text
Thread 1:
Account A → Account B

Thread 2:
Account B → Account A
```

Both operations require two account mutexes.

Manual locking may create opposite lock orders:

```text
Thread 1:
Locks Account A
Then locks Account B

Thread 2:
Locks Account B
Then locks Account A
```

This may cause deadlock.

Instead, both threads can use:

```cpp
scoped_lock lock(sourceMutex, destinationMutex);
```

`scoped_lock` uses deadlock-avoidance behaviour for multiple mutexes.

After the statement completes, the current thread owns all provided mutexes.

## Automatic Unlocking

`scoped_lock` follows RAII.

RAII means the lifetime of the lock object controls the lifetime of mutex ownership.

```text
Object created   → Mutexes locked
Object destroyed → Mutexes unlocked
```

Example:

```cpp
void update(){
    scoped_lock lock(mutex1, mutex2);

    if(error){
        return;
    }
}
```

Even if the function returns early, both mutexes are automatically unlocked.

This makes `scoped_lock` safer than manual `lock()` and `unlock()`.

## `lock_guard` vs `scoped_lock`

For one mutex, both can be used.

```cpp
lock_guard<mutex> lock(mtx);
```

```cpp
scoped_lock lock(mtx);
```

Main difference:

```text
lock_guard:
Normally used for one mutex
Available before C++17
Simple automatic locking

scoped_lock:
Can lock one or multiple mutexes
Available from C++17
Useful for deadlock-safe multiple locking
```

For a single mutex, `lock_guard` is still simple and clear.

For multiple mutexes in C++17, `scoped_lock` is usually preferred.

## `unique_lock` vs `scoped_lock`

```text
unique_lock:
Can unlock manually
Can lock again
Supports defer_lock
Supports condition_variable
Movable
More flexible

scoped_lock:
Locks immediately
Cannot manually unlock
Cannot lock again
Simple automatic lifetime
Best when all mutexes are needed for the complete scope
```

Use `unique_lock` when you need control over when the mutex is locked or unlocked.

Use `scoped_lock` when you want all mutexes locked for the complete scope.

## `std::lock()` vs `scoped_lock`

Using `std::lock()`:

```cpp
unique_lock<mutex> lock1(mutex1, defer_lock);
unique_lock<mutex> lock2(mutex2, defer_lock);

lock(lock1, lock2);
```

Using `scoped_lock`:

```cpp
scoped_lock lock(mutex1, mutex2);
```

Both approaches can safely acquire multiple mutexes.

`scoped_lock` is shorter and easier when no manual lock control is required.

## When Should We Use `scoped_lock`?

Use `scoped_lock` when:

```text
One operation needs multiple mutexes
All mutexes are required for the complete scope
Automatic unlocking is required
Manual lock and unlock are unnecessary
C++17 or newer is available
```

Realistic examples:

```text
Money transfer between two accounts
Move data between two queues
Transfer a parcel between two containers
Swap data between two shared objects
Update sensor data and robot state together
Copy data between two thread-safe objects
```

## Important: Same Mutex Must Not Be Passed Twice

Do not pass the same mutex twice.

Incorrect:

```cpp
scoped_lock lock(mtx, mtx);
```

A normal mutex cannot be locked twice by the same thread.

This may cause deadlock or undefined behaviour depending on the mutex type and implementation.

## Important: Object Lifetime

The mutexes must remain alive while `scoped_lock` is using them.

```text
Mutex objects must exist
        ↓
scoped_lock is created
        ↓
Critical section executes
        ↓
scoped_lock is destroyed
        ↓
Mutexes may later be destroyed
```

## Scope Control

A smaller scope can be created when the mutexes should be released early.

```cpp
void process(){
    performIndependentWork();

    {
        scoped_lock lock(mutex1, mutex2);

        // Protected work
    }

    performMoreIndependentWork();
}
```

The mutexes are unlocked when the inner block ends.

This is useful because unrelated work does not keep the mutexes locked.

## Final Summary

```text
scoped_lock is available from C++17.

It can lock one or multiple mutexes.

For multiple mutexes, it avoids manual lock-order problems.

It automatically unlocks all mutexes when the scope ends.

Use scoped_lock when all mutexes are required
for the complete critical section.

Use unique_lock when manual lock or unlock control is needed.
```

## Related File

```text
15_scoped_lock.cpp
```
