# Thread Internal Working

## 1. Program, Process and Main Thread

When we execute a C++ program, the operating system creates a **process**.

A process is a running instance of a program. It contains memory, code, files, and other resources required by the program.

The operating system also creates the first thread, called the **main thread**.

```text
C++ executable
      ↓
Process is created
      ↓
Main thread is created
      ↓
main() starts executing
```

The `main()` function executes on the main thread.

```cpp
int main()
{
    // This code runs on the main thread
    return 0;
}
```

In simple words:

```text
Process = Running program with resources
Thread  = Execution flow inside the process
```

---

## 2. What Happens When We Create a Thread?

In C++, we create a thread using `std::thread`.

```cpp
std::thread worker(task);
```

`std::thread` is a C++ wrapper around the operating system's threading functionality.

On Linux, the conceptual flow is:

```text
std::thread
    ↓
C++ standard library
    ↓
POSIX thread library
    ↓
pthread_create()
    ↓
Linux kernel creates a thread
```

The operating system creates a new execution context and schedules the thread for execution.

```text
Thread created
     ↓
Placed in ready state
     ↓
CPU becomes available
     ↓
Thread starts executing task()
```

---

## 3. System Calls

A normal application runs in **user space**. It cannot directly control hardware or kernel resources.

For operating-system services, the application makes a request to the kernel. This request is called a **system call**.

```text
User-space program
       ↓
System call
       ↓
Operating-system kernel
```

Thread-related Linux mechanisms may involve operations such as:

```text
clone()
futex()
nanosleep()
sched_yield()
```

Examples:

* Creating a thread
* Putting a thread to sleep
* Waiting for another thread
* Synchronizing threads
* Reading from a file or socket

The exact system calls used depend on the operating system and standard-library implementation.

---

## 4. Thread Memory

Threads of the same process share most process resources.

### Shared Between Threads

```text
Program code
Global variables
Static variables
Heap memory
Open files
Sockets
Shared libraries
```

### Separate for Every Thread

```text
Stack
CPU registers
Instruction pointer
Stack pointer
Thread ID
Thread-local storage
```

A simplified process memory view:

```text
+----------------------------+
| Worker Thread 2 Stack      |
+----------------------------+
| Worker Thread 1 Stack      |
+----------------------------+
| Main Thread Stack          |
+----------------------------+
| Shared Libraries           |
+----------------------------+
| Shared Heap                |
+----------------------------+
| Global and Static Data     |
+----------------------------+
| Program Code               |
+----------------------------+
```

---

## 5. Thread Stack

Every thread has its own stack.

The stack generally stores:

```text
Local variables
Function parameters
Return addresses
Function-call information
Temporary values
```

Example:

```cpp
void task()
{
    int value = 10;
}
```

If two threads execute `task()`, each thread gets its own separate `value`.

```text
Thread 1 stack → value = 10
Thread 2 stack → value = 10
```

These two local variables are different.

---

## 6. Shared Heap and Global Variables

Heap memory belongs to the process, so all threads can access it when they have the required address or object reference.

```cpp
int* number = new int(10);
```

The allocated integer is stored on the heap. It can be accessed by multiple threads.

Global and static variables are also shared.

```cpp
int sharedValue = 0;

void task()
{
    sharedValue = 100;
}
```

If the worker thread changes `sharedValue`, the main thread can see that change.

Sharing memory makes communication between threads easy, but it can also create race conditions.

---

## 7. CPU Registers and Instruction Pointer

Each thread has its own CPU execution state.

This includes:

```text
CPU registers
Instruction pointer
Stack pointer
Current execution state
```

The instruction pointer stores the location of the next instruction that the thread will execute.

For example:

```text
Main thread   → Executing line 30
Worker thread → Executing line 8
Logger thread → Executing line 55
```

Although all threads belong to the same process, they can execute different parts of the program.

---

## 8. Thread Scheduling

The operating-system scheduler decides:

```text
Which thread should run
When the thread should run
How long it should run
Which CPU core should run it
When it should be paused
```

A thread may move through states such as:

```text
Created
   ↓
Ready
   ↓
Running
   ↓
Waiting / Sleeping / Blocked
   ↓
Ready
   ↓
Running
   ↓
Completed
```

Because scheduling is controlled by the operating system, the execution order of threads is not always guaranteed.

---

## 9. Context Switching

Suppose one CPU core has multiple runnable threads.

The CPU may run one thread for some time and then switch to another thread.

```text
Thread A runs
      ↓
Thread A state is saved
      ↓
Thread B state is restored
      ↓
Thread B runs
```

This operation is called a **context switch**.

During a context switch, the operating system saves and restores information such as:

```text
CPU registers
Instruction pointer
Stack pointer
Scheduling state
```

Context switching has a performance cost. Therefore, creating too many threads may reduce performance.

---

## 10. Concurrency and Parallelism

### Concurrency

Concurrency means multiple tasks make progress during the same period.

On a single-core processor, the operating system switches between threads quickly.

```text
Thread A runs
Thread B runs
Thread A runs again
Thread C runs
```

Only one thread may execute at one exact moment, but all threads continue making progress.

### Parallelism

Parallelism means multiple threads execute at the exact same time.

This requires multiple CPU cores.

```text
CPU Core 1 → Thread A
CPU Core 2 → Thread B
CPU Core 3 → Thread C
```

Important:

```text
Multithreading provides concurrency.

Parallel execution depends on the available CPU cores
and the operating-system scheduler.
```

---

## 11. How `sleep_for()` Works

```cpp
std::this_thread::sleep_for(std::chrono::seconds(5));
```

When this statement executes, the thread asks the operating system not to schedule it for approximately five seconds.

```text
Thread calls sleep_for()
        ↓
Thread enters sleeping state
        ↓
CPU runs another thread
        ↓
Sleep duration completes
        ↓
Thread becomes ready
        ↓
Scheduler runs it again
```

The sleeping thread does not continuously use the CPU while waiting.

---

## 12. How `join()` Works

```cpp
worker.join();
```

When the main thread calls `join()`, it waits until the worker thread completes.

```text
Main reaches join()
       ↓
Main thread waits
       ↓
Worker continues executing
       ↓
Worker completes
       ↓
Main thread wakes up
       ↓
Main executes the next statement
```

Example:

```cpp
worker.join();

std::cout << "Suraj is developer\n";
```

The print statement runs only after the worker thread completes.

---

## 13. How `detach()` Works

```cpp
worker.detach();
```

`detach()` separates the running thread from its `std::thread` object.

The main thread does not wait for the detached thread.

```text
Worker starts
Main calls detach()
Main continues immediately
Worker runs independently
```

Example:

```cpp
worker.detach();

std::cout << "Suraj is developer\n";

return 0;
```

The print statement executes immediately.

When `main()` returns, the complete process ends. All threads belonging to that process are terminated.

Therefore, the detached thread may not complete its task.

```text
Main returns
     ↓
Process ends
     ↓
Detached thread may remain incomplete
```

---

## 14. Why Race Conditions Occur

Consider this shared variable:

```cpp
int counter = 0;
```

Two threads execute:

```cpp
counter++;
```

The operation is internally similar to:

```text
Read counter
Add 1
Write counter
```

Possible execution:

```text
Initial counter = 0

Thread 1 reads 0
Thread 2 reads 0

Thread 1 writes 1
Thread 2 writes 1
```

Expected value:

```text
2
```

Actual value:

```text
1
```

This is called a **race condition**.

It happens because multiple threads access and modify shared data without synchronization.

---

## 15. How Mutex Waiting Works

A mutex allows only one thread at a time to enter a protected section.

```cpp
mutex.lock();

counter++;

mutex.unlock();
```

Conceptual flow:

```text
Thread A locks mutex
Thread B tries to lock mutex
Thread B waits
Thread A unlocks mutex
Thread B gets the mutex
```

On Linux, mutex implementations may use atomic CPU operations and a kernel mechanism called `futex`.

The exact implementation depends on the standard library and operating system.

---

## 16. Why Threads Are Called Lightweight

Threads are called lightweight because threads of the same process share most resources.

A new process normally requires a separate address space and separate resources.

A new thread mainly requires:

```text
Separate stack
Register state
Instruction pointer
Thread ID
Scheduling information
```

However, threads are not free.

Every thread requires memory and operating-system management. Too many threads can introduce:

```text
Context-switching overhead
Extra stack memory
Synchronization cost
Scheduling overhead
Complex debugging
```

---

## Final Summary

```text
A process is a running program.

A thread is an execution flow inside that process.

All threads of a process share:
Code, global variables, static variables, heap,
files, sockets and shared libraries.

Every thread has its own:
Stack, registers, instruction pointer,
thread ID and execution state.

The operating-system scheduler decides:
When a thread runs and on which CPU core.

Single-core CPU:
Threads run concurrently using context switching.

Multi-core CPU:
Threads may execute in parallel.

System calls:
Allow user-space programs to request services
from the operating-system kernel.
```
