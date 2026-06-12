/*
 * ====================================================================
 * PROGRAM 228: Recursive_Mutex.cpp
 * TOPIC    : Multithreading - std::recursive_mutex
 * ====================================================================
 * THEORY:
 * std::recursive_mutex allows the same thread to lock the mutex
 * multiple times without deadlocking. It keeps a count of locks;
 * must unlock the same number of times.
 *
 * USE CASES:
 *   - Recursive functions that need to acquire the same mutex
 *   - When refactoring legacy code where lock is already held
 *   - When multiple member functions call each other
 *
 * WARNING: Usually indicates poor design. Prefer redesigning to
 * avoid nested locks. Recursive mutexes are slower than regular ones.
 *
 * COMPLEXITY:
 *   Slightly slower than std::mutex (tracking ownership + count)
 *
 * INTERVIEW QUESTIONS:
 * Q: When should you NOT use recursive_mutex?
 * A: When you can restructure code to lock once. Recursive mutexes
 *    hide concurrency design issues.
 *
 * Q: Can recursive_mutex cause deadlocks?
 * A: Yes, if two threads try to lock each other's recursive mutexes
 *    in different order.
 *
 * COMMON MISTAKES:
 * - Using recursive_mutex when a regular mutex + restructuring suffices
 * - Forgetting to unlock the same number of times
 * - Assuming it prevents all deadlocks (deadlocks still possible)
 * - Using it for different resources (use separate mutexes)
 * ====================================================================
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

recursive_mutex rec_mtx;

// Recursive function that needs mutex
void recursive_task(int depth, int max_depth) {
    if (depth > max_depth) return;

    lock_guard<recursive_mutex> lock(rec_mtx);
    cout << "Depth " << depth << " locked (thread "
         << this_thread::get_id() << ")\n";

    // Recursive call - same thread re-locks the recursive_mutex
    recursive_task(depth + 1, max_depth);

    cout << "Depth " << depth << " unlocking\n";
} // lock_guard unlocks here

// Simulating nested member function calls
class BankAccount {
    recursive_mutex mtx;
    int balance = 0;
public:
    void add(int amount) {
        lock_guard<recursive_mutex> lock(mtx);
        balance += amount;
    }

    void add_twice(int a, int b) {
        lock_guard<recursive_mutex> lock(mtx);
        add(a);  // re-enters same mutex - OK for recursive_mutex
        add(b);  // would deadlock with std::mutex
    }

    int get_balance() {
        lock_guard<recursive_mutex> lock(mtx);
        return balance;
    }
};

int main() {
    cout << "=== RECURSIVE MUTEX ===\n\n";

    cout << "--- Recursive function ---\n";
    thread t1(recursive_task, 1, 5);
    thread t2(recursive_task, 1, 3);
    t1.join();
    t2.join();

    cout << "\n--- Nested member calls ---\n";
    BankAccount acc;
    thread t3([&]() { acc.add_twice(100, 200); });
    thread t4([&]() { acc.add_twice(50, 25); });
    t3.join();
    t4.join();
    cout << "Final balance: " << acc.get_balance() << "\n";

    /*
     * SAMPLE I/O:
     * --- Recursive function ---
     * Depth 1 locked (thread 0x...)
     * Depth 2 locked (thread 0x...)
     * Depth 3 locked (thread 0x...)
     * Depth 3 unlocking
     * Depth 2 unlocking
     * Depth 1 unlocking
     * Depth 1 locked (thread 0x...)
     * Depth 2 locked (thread 0x...)
     * Depth 3 locked (thread 0x...)
     * Depth 3 unlocking
     * Depth 2 unlocking
     * Depth 1 unlocking
     *
     * --- Nested member calls ---
     * Final balance: 375
     */

    return 0;
}
