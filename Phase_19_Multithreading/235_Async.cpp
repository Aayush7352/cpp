/*
 * ====================================================================
 * PROGRAM 235: Async.cpp
 * TOPIC    : Multithreading - std::async
 * ====================================================================
 * THEORY:
 * std::async runs a function potentially in a separate thread.
 * Returns a std::future to retrieve the result.
 *
 * LAUNCH POLICIES:
 *   launch::async    - run in a new thread immediately
 *   launch::deferred - run on first get() or wait(), lazily
 *   launch::async | launch::deferred (default) -
 *     implementation chooses (typically async for non-deferred)
 *
 * ADVANTAGES OVER std::thread:
 *   - Automatic exception propagation
 *   - Returns result via future
 *   - May use thread pool internally (implementation-dependent)
 *   - Cleaner RAII (no explicit join needed for one-off tasks)
 *
 * COMPLEXITY:
 *   async: O(1) creation overhead
 *   deferred: O(1) creation, O(execution) on get()
 *
 * INTERVIEW QUESTIONS:
 * Q: When to use async vs thread?
 * A: async for one-off tasks where we need results.
 *    thread + promise for finer control.
 *
 * Q: Does async guarantee thread creation?
 * A: Only with launch::async. Default may use deferred.
 *
 * Q: What happens if you destroy the future from async?
 * A: If async task is running, waits for it to complete (blocking).
 *
 * COMMON MISTAKES:
 * - Using default policy and assuming async (may be deferred)
 * - Not storing the future (temporary destroyed, blocks)
 * - Assuming get() won't block when using deferred
 * - Launching too many async tasks (resource exhaustion)
 * ====================================================================
 */

#include <iostream>
#include <future>
#include <chrono>
#include <vector>
#include <numeric>

using namespace std;

int expensive_compute(int x) {
    this_thread::sleep_for(chrono::milliseconds(300));
    cout << "  Computing " << x << "^2 in thread "
         << this_thread::get_id() << "\n";
    return x * x;
}

int main() {
    cout << "=== std::async ===\n\n";

    // launch::async - immediate thread execution
    cout << "--- launch::async ---\n";
    future<int> f1 = async(launch::async, expensive_compute, 5);
    future<int> f2 = async(launch::async, expensive_compute, 10);

    cout << "Main doing other work...\n";
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "f1: " << f1.get() << "\n";
    cout << "f2: " << f2.get() << "\n\n";

    // launch::deferred - lazy execution
    cout << "--- launch::deferred ---\n";
    future<int> f3 = async(launch::deferred, [] {
        cout << "  Deferred task in thread " << this_thread::get_id() << "\n";
        this_thread::sleep_for(chrono::milliseconds(100));
        return 42;
    });
    cout << "Deferred not started yet...\n";
    this_thread::sleep_for(chrono::milliseconds(50));
    cout << "Calling get() now:\n";
    cout << "f3: " << f3.get() << "\n\n";

    // Default policy (implementation chooses)
    cout << "--- Default policy ---\n";
    future<int> f4 = async([] {
        cout << "  Default policy task in " << this_thread::get_id() << "\n";
        return 100;
    });
    cout << "f4: " << f4.get() << "\n\n";

    // Running many async tasks
    cout << "--- Many async tasks ---\n";
    vector<future<int>> tasks;
    for (int i = 0; i < 6; ++i)
        tasks.push_back(async(launch::async, expensive_compute, i + 1));

    int sum = 0;
    for (auto& f : tasks) sum += f.get();
    cout << "Sum of squares (1..6): " << sum << "\n";

    /*
     * SAMPLE I/O:
     * --- launch::async ---
     *   Computing 5^2 in thread 0x...
     *   Computing 10^2 in thread 0x...
     * Main doing other work...
     * f1: 25
     * f2: 100
     *
     * --- launch::deferred ---
     * Deferred not started yet...
     * Calling get() now:
     *   Deferred task in thread 0x... (main thread)
     * f3: 42
     *
     * --- Default policy ---
     *   Default policy task in 0x...
     * f4: 100
     *
     * --- Many async tasks ---
     *   ...
     * Sum of squares (1..6): 91
     */

    return 0;
}
