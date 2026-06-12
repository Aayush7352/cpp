/*
 * ====================================================================
 * PROGRAM 229: Shared_Mutex.cpp
 * TOPIC    : Multithreading - std::shared_mutex (C++17)
 * ====================================================================
 * THEORY:
 * std::shared_mutex (C++17) / std::shared_timed_mutex (C++14)
 * supports two access levels:
 *   - Exclusive (write): lock_guard, unique_lock
 *   - Shared (read):     shared_lock (C++14)
 *
 * Multiple readers can hold the shared lock simultaneously.
 * A writer gets exclusive access (no readers or other writers).
 *
 * USAGE:
 *   For frequent reads, infrequent writes (reader-writer lock pattern).
 *
 * COMPLEXITY:
 *   Slightly more overhead than std::mutex due to reader counting.
 *
 * INTERVIEW QUESTIONS:
 * Q: When to use shared_mutex vs regular mutex?
 * A: Shared_mutex when reads >> writes. If writes are frequent,
 *    regular mutex may perform better.
 *
 * Q: Does shared_mutex have priority?
 * A: Not guaranteed. Some implementations starve writers if readers
 *    keep arriving.
 *
 * COMMON MISTAKES:
 * - Using shared_lock for writes (undefined behavior)
 * - Using lock_guard for reads (blocks all readers unnecessarily)
 * - Assuming shared_mutex gives better performance always
 * - Not upgrading from shared to exclusive (need to unlock shared first)
 * ====================================================================
 */

#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <chrono>

using namespace std;

class ThreadSafeCounter {
    mutable shared_mutex mtx;
    int value = 0;

public:
    void increment() {
        unique_lock lock(mtx); // exclusive write lock
        ++value;
    }

    int get() const {
        shared_lock lock(mtx); // shared read lock
        return value;
    }
};

// Concurrent readers example
shared_mutex sh_mtx;
int shared_resource = 0;

void reader(int id) {
    shared_lock lock(sh_mtx);
    cout << "Reader " << id << " sees: " << shared_resource << "\n";
    this_thread::sleep_for(chrono::milliseconds(50));
    // Multiple readers can be here simultaneously
}

void writer(int id, int val) {
    unique_lock lock(sh_mtx);
    cout << "Writer " << id << " setting to " << val << "\n";
    shared_resource = val;
    this_thread::sleep_for(chrono::milliseconds(100));
}

int main() {
    cout << "=== SHARED MUTEX (Reader-Writer Lock) ===\n\n";

    cout << "--- Concurrent readers + writer ---\n";
    vector<thread> threads;

    // Multiple readers can run concurrently
    for (int i = 0; i < 5; ++i)
        threads.emplace_back(reader, i + 1);
    threads.emplace_back(writer, 1, 42);
    for (int i = 0; i < 3; ++i)
        threads.emplace_back(reader, i + 6);

    for (auto& t : threads) t.join();

    cout << "\n--- ThreadSafeCounter ---\n";
    ThreadSafeCounter counter;
    vector<thread> writers;
    for (int i = 0; i < 10; ++i)
        writers.emplace_back([&counter]() { counter.increment(); });
    for (auto& t : writers) t.join();

    // Multiple readers read final value concurrently
    vector<thread> readers;
    for (int i = 0; i < 5; ++i)
        readers.emplace_back([&counter, i]() {
            cout << "Counter reader " << i << ": " << counter.get() << "\n";
        });
    for (auto& t : readers) t.join();

    /*
     * SAMPLE I/O:
     * --- Concurrent readers + writer ---
     * Reader 1 sees: 0
     * Reader 2 sees: 0
     * Reader 3 sees: 0
     * (readers run concurrently, then writer blocks all)
     * Writer 1 setting to 42
     * Reader 6 sees: 42
     * ...
     *
     * --- ThreadSafeCounter ---
     * Counter reader 0: 10
     * Counter reader 1: 10
     * ...
     */

    return 0;
}
