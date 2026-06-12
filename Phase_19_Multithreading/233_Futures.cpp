/*
 * ====================================================================
 * PROGRAM 233: Futures.cpp
 * TOPIC    : Multithreading - std::future and std::shared_future
 * ====================================================================
 * THEORY:
 * std::future holds the result of an asynchronous operation.
 *   - get(): blocks until result is available (can only call once)
 *   - wait(): blocks until ready (doesn't return result)
 *   - wait_for(duration): returns future_status (ready/timeout/deferred)
 *   - valid(): checks if future has a shared state
 *
 * std::shared_future: can be copied; multiple threads can get().
 * Use .share() on a future to convert.
 *
 * POLLING: Use wait_for() with zero duration to check without blocking.
 *
 * COMPLEXITY:
 *   get/wait: O(1) if ready; blocks otherwise
 *
 * INTERVIEW QUESTIONS:
 * Q: Can future be copied?
 * A: No (move only). shared_future can be copied.
 *
 * Q: What happens if get() is called multiple times on a future?
 * A: Moves the state; second call throws std::future_error.
 *
 * Q: Difference between async and future?
 * A: future is the result channel; async creates the task.
 *
 * COMMON MISTAKES:
 * - Calling get() multiple times on the same future
 * - Not checking valid() before calling get()
 * - Forgetting to store the future (result lost if temporary)
 * - Blocking main thread unnecessarily
 * ====================================================================
 */

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

int slow_square(int x) {
    this_thread::sleep_for(chrono::milliseconds(200));
    return x * x;
}

int slow_cube(int x) {
    this_thread::sleep_for(chrono::milliseconds(300));
    return x * x * x;
}

int main() {
    cout << "=== FUTURES ===\n\n";

    // Basic future from async
    cout << "--- Basic future ---\n";
    future<int> f1 = async(launch::async, slow_square, 5);
    future<int> f2 = async(launch::async, slow_cube, 3);

    cout << "Waiting for results...\n";
    cout << "f1 (5^2): " << f1.get() << "\n";
    cout << "f2 (3^3): " << f2.get() << "\n\n";

    // Polling with wait_for
    cout << "--- Polling ---\n";
    future<int> f3 = async(launch::async, [](int ms) {
        this_thread::sleep_for(chrono::milliseconds(ms));
        return 42;
    }, 500);

    while (f3.wait_for(chrono::milliseconds(0)) != future_status::ready) {
        cout << "Still waiting...\n";
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "Poll result: " << f3.get() << "\n\n";

    // shared_future
    cout << "--- shared_future ---\n";
    future<int> f4 = async(launch::async, [] {
        this_thread::sleep_for(chrono::milliseconds(100));
        return 100;
    });
    shared_future<int> sf = f4.share(); // f4 is now invalid

    auto reader = [](int id, shared_future<int> sf) {
        cout << "Reader " << id << " got: " << sf.get() << "\n";
    };

    vector<thread> readers;
    for (int i = 0; i < 3; ++i)
        readers.emplace_back(reader, i + 1, sf);
    for (auto& t : readers) t.join();

    /*
     * SAMPLE I/O:
     * --- Basic future ---
     * Waiting for results...
     * f1 (5^2): 25
     * f2 (3^3): 27
     *
     * --- Polling ---
     * Still waiting...
     * Still waiting...
     * Still waiting...
     * Still waiting...
     * Poll result: 42
     *
     * --- shared_future ---
     * Reader 1 got: 100
     * Reader 2 got: 100
     * Reader 3 got: 100
     */

    return 0;
}
