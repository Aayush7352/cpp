/*
 * ====================================================================
 * PROGRAM 234: Promises.cpp
 * TOPIC    : Multithreading - std::promise
 * ====================================================================
 * THEORY:
 * std::promise<T> provides a channel to send a value (or exception)
 * to a std::future<T>. The promise's set_value() or set_exception()
 * fulfills the associated future.
 *
 * USAGE:
 *   1. Create a promise and get its future.
 *   2. Pass the promise to a thread (by std::move).
 *   3. Thread calls set_value() with the result.
 *   4. The original thread calls future.get() to receive it.
 *
 * EXCEPTIONS: set_exception() stores an exception_ptr in the future.
 *
 * COMPLEXITY:
 *   Promise/Future: O(1) shared state allocation
 *   set_value/get: O(1) with synchronization
 *
 * INTERVIEW QUESTIONS:
 * Q: Can a promise be used with multiple futures?
 * A: No, one promise creates one future. Use shared_future for multi.
 *
 * Q: What happens if promise is destroyed without setting value?
 * A: A broken_promise exception is stored in the future.
 *
 * Q: Difference between promise and packaged_task?
 * A: promise is manual; packaged_task wraps a callable automatically.
 *
 * COMMON MISTAKES:
 * - Forgetting to move the promise to the thread
 * - Setting value multiple times on the same promise (throws)
 * - Not handling broken_promise
 * - Using a future after its associated promise is destroyed
 * ====================================================================
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <exception>
#include <stdexcept>

using namespace std;

// Basic promise usage
void compute_sum(promise<int> prom, int a, int b) {
    try {
        this_thread::sleep_for(chrono::milliseconds(200));
        if (a < 0 || b < 0)
            throw invalid_argument("negative not allowed");
        prom.set_value(a + b);
    } catch (...) {
        prom.set_exception(current_exception());
    }
}

// Promise with void
void compute_void(promise<void> prom) {
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "Void task done (no value)\n";
    prom.set_value();
}

int main() {
    cout << "=== PROMISES ===\n\n";

    // Basic: send result via promise
    cout << "--- Basic promise ---\n";
    promise<int> prom;
    future<int> fut = prom.get_future();
    thread t(compute_sum, move(prom), 10, 20);
    cout << "Waiting for sum...\n";
    cout << "Result: " << fut.get() << "\n";
    t.join();

    // Exception via promise
    cout << "\n--- Exception via promise ---\n";
    promise<int> prom2;
    future<int> fut2 = prom2.get_future();
    thread t2(compute_sum, move(prom2), -5, 10);
    try {
        cout << fut2.get() << "\n";
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << "\n";
    }
    t2.join();

    // Void promise
    cout << "\n--- Void promise ---\n";
    promise<void> prom3;
    future<void> fut3 = prom3.get_future();
    thread t3(compute_void, move(prom3));
    fut3.get(); // wait for void result
    t3.join();

    // Broken promise demonstration
    cout << "\n--- Broken promise ---\n";
    future<int> broken_fut;
    {
        promise<int> temp_prom;
        broken_fut = temp_prom.get_future();
        // temp_prom destroyed without set_value
    } // broken_promise stored here
    try {
        broken_fut.get();
    } catch (const future_error& e) {
        cout << "future_error: " << e.what() << "\n";
    }

    /*
     * SAMPLE I/O:
     * --- Basic promise ---
     * Waiting for sum...
     * Result: 30
     *
     * --- Exception via promise ---
     * Caught: negative not allowed
     *
     * --- Void promise ---
     * Void task done (no value)
     *
     * --- Broken promise ---
     * future_error: broken promise
     */

    return 0;
}
