/*
 * ====================================================================
 * PROGRAM 227: Mutex.cpp
 * TOPIC    : Multithreading - std::mutex and lock guards
 * ====================================================================
 * THEORY:
 * std::mutex provides mutual exclusion.
 *   lock()   - blocks until mutex acquired
 *   unlock() - releases mutex
 *   try_lock()- non-blocking attempt, returns bool
 *
 * std::lock_guard: RAII wrapper, locks on construction, unlocks on
 * destruction. Non-copyable, non-movable.
 *
 * std::unique_lock: more flexible, can defer_lock, adopt_lock,
 * try_to_lock, supports .unlock() and .lock() manually, movable.
 *
 * DEADLOCK PREVENTION:
 *   - Always lock mutexes in the same order
 *   - Use std::lock() to lock multiple mutexes atomically
 *   - Use std::scoped_lock (C++17) for multiple mutexes
 *
 * COMPLEXITY:
 *   Lock/unlock: O(1) (fast, user-space if no contention)
 *
 * INTERVIEW QUESTIONS:
 * Q: What is the difference between lock_guard and unique_lock?
 * A: lock_guard is simpler (lock on construct, unlock on destroy).
 *    unique_lock supports defer_lock, try_lock, manual lock/unlock.
 *
 * Q: How does std::lock avoid deadlocks?
 * A: Uses a deadlock-avoidance algorithm internally.
 *
 * COMMON MISTAKES:
 * - Forgetting to unlock (use lock_guard/unique_lock to be safe)
 * - Locking in different order causing deadlock
 * - Using the same mutex for unrelated resources (contention)
 * - Copying lock_guard or unique_lock
 * ====================================================================
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;

mutex mtx;
int shared_data = 0;

void safe_increment(int id, int times) {
    for (int i = 0; i < times; ++i) {
        lock_guard<mutex> lock(mtx);
        ++shared_data;
    }
    cout << "Thread " << id << " done\n";
}

// Unique lock with defer_lock
void safe_increment_unique(int id, int times) {
    unique_lock<mutex> ulock(mtx, defer_lock);
    for (int i = 0; i < times; ++i) {
        ulock.lock();
        ++shared_data;
        ulock.unlock();
    }
    cout << "Thread " << id << " (unique) done\n";
}

// Deadlock example (prevention using std::lock)
mutex mtx1, mtx2;

void deadlock_free(int id) {
    // Locks both atomically to prevent deadlock
    scoped_lock lock(mtx1, mtx2); // C++17
    cout << "Thread " << id << " holds both mutexes\n";
    this_thread::sleep_for(chrono::milliseconds(100));
}

int main() {
    cout << "=== MUTEX ===\n\n";

    // Multiple threads incrementing shared data safely
    const int N = 5;
    vector<thread> threads;

    for (int i = 0; i < N; ++i)
        threads.emplace_back(safe_increment, i + 1, 100);
    for (auto& t : threads) t.join();
    cout << "Final value (lock_guard): " << shared_data << "\n\n";

    shared_data = 0;
    vector<thread> threads2;
    for (int i = 0; i < N; ++i)
        threads2.emplace_back(safe_increment_unique, i + 1, 100);
    for (auto& t : threads2) t.join();
    cout << "Final value (unique_lock): " << shared_data << "\n\n";

    // Deadlock-free double locking
    cout << "--- scoped_lock (deadlock free) ---\n";
    thread t1(deadlock_free, 1);
    thread t2(deadlock_free, 2);
    t1.join();
    t2.join();

    /*
     * SAMPLE I/O:
     * Thread 1 done
     * Thread 2 done
     * ...
     * Final value (lock_guard): 500
     *
     * Final value (unique_lock): 500
     *
     * --- scoped_lock (deadlock free) ---
     * Thread 1 holds both mutexes
     * Thread 2 holds both mutexes
     */

    return 0;
}
