/*
 * ====================================================================
 * PROGRAM 226: std_thread.cpp
 * TOPIC    : Multithreading - std::thread basics
 * ====================================================================
 * THEORY:
 * std::thread represents a single thread of execution.
 * Call .join() to wait for thread completion (blocking).
 * Call .detach() to let the thread run independently.
 * Thread IDs: .get_id() or std::this_thread::get_id()
 * hardware_concurrency() returns number of hardware thread contexts.
 * Arguments are passed by value by default; use std::ref() for refs.
 *
 * COMPLEXITY:
 *   Thread creation/destruction: O(1) amortized
 *   join() blocks until thread finishes
 *
 * INTERVIEW QUESTIONS:
 * Q: What happens if you join a thread twice?
 * A: std::terminate is called.
 *
 * Q: What happens if you don't join or detach?
 * A: std::terminate is called in the destructor if joinable.
 *
 * COMMON MISTAKES:
 * - Forgetting to join or detach a joinable thread
 * - Joining a detached thread
 * - Passing references without std::ref (copies instead)
 * - Racy access to shared data without synchronization
 * ====================================================================
 */

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

void worker(int id, int sleep_ms) {
    cout << "Thread " << id << " starting (ID: "
         << this_thread::get_id() << ")\n";
    this_thread::sleep_for(chrono::milliseconds(sleep_ms));
    cout << "Thread " << id << " done\n";
}

void shared_worker(int id, int& counter) {
    for (int i = 0; i < 10; ++i)
        ++counter; // UNSAFE without mutex!
}

int main() {
    cout << "=== std::thread BASICS ===\n\n";
    cout << "Main thread ID: " << this_thread::get_id() << "\n";
    cout << "Hardware concurrency: " << thread::hardware_concurrency() << "\n\n";

    // Basic join
    thread t1(worker, 1, 500);
    thread t2(worker, 2, 200);
    t1.join();
    t2.join();

    // Detach
    thread t3(worker, 3, 100);
    t3.detach();

    // Vector of threads
    cout << "\n--- Vector of threads ---\n";
    vector<thread> threads;
    for (int i = 0; i < 4; ++i)
        threads.emplace_back(worker, i + 4, 100 * (i + 1));
    for (auto& t : threads)
        t.join();

    // Reference argument (unsafe without mutex)
    cout << "\n--- Reference argument (no mutex) ---\n";
    int counter = 0;
    thread t4(shared_worker, 10, ref(counter));
    thread t5(shared_worker, 11, ref(counter));
    t4.join();
    t5.join();
    cout << "Counter (expected 20): " << counter << "\n";

    this_thread::sleep_for(chrono::milliseconds(300));
    cout << "\nMain done\n";

    /*
     * SAMPLE I/O:
     * Main thread ID: 0x1... (varies)
     * Hardware concurrency: 8
     *
     * Thread 1 starting (ID: 0x...)
     * Thread 2 starting (ID: 0x...)
     * Thread 2 done
     * Thread 1 done
     * Thread 3 starting (ID: ...)
     * (detached, may finish before or after main)
     *
     * --- Vector of threads ---
     * ...
     * Counter (expected 20): 18 (racy!)
     * Main done
     */

    return 0;
}
