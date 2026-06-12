/*
 * ====================================================================
 * PROGRAM 232: Thread_Pool.cpp
 * TOPIC    : Multithreading - Custom Thread Pool
 * ====================================================================
 * THEORY:
 * Thread pool maintains a set of worker threads that execute tasks
 * from a shared queue. Avoids the overhead of creating/destroying
 * threads repeatedly.
 *
 * COMPONENTS:
 *   - Worker threads: loop waiting for tasks
 *   - Task queue: std::queue<std::packaged_task<void()>>
 *   - Synchronization: mutex + condition_variable
 *   - Graceful shutdown: stop flag, notify_all, join all threads
 *
 * std::packaged_task wraps any callable and stores its return value
 * in a std::future that can be retrieved later.
 *
 * COMPLEXITY:
 *   Submit: O(1) amortized
 *   Thread count: typically hardware_concurrency
 *
 * INTERVIEW QUESTIONS:
 * Q: How to handle exceptions from tasks?
 * A: std::packaged_task stores exceptions; future::get rethrows.
 *
 * Q: When would you increase threads beyond hardware_concurrency?
 * A: For I/O-bound tasks where threads block frequently.
 *
 * Q: How to implement work-stealing?
 * A: Each thread has its own queue; idle threads steal from others.
 *
 * COMMON MISTAKES:
 * - Not stopping threads gracefully (joining a stuck thread)
 * - Using std::function instead of std::packaged_task (no future)
 * - Forgetting to handle exceptions in worker loop
 * - Deadlock if a submitted task tries to join the pool
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <chrono>

using namespace std;

class ThreadPool {
    vector<thread> workers;
    queue<packaged_task<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    atomic<bool> stop;

public:
    ThreadPool(size_t num = thread::hardware_concurrency()) : stop(false) {
        for (size_t i = 0; i < num; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    packaged_task<void()> task;
                    {
                        unique_lock lock(mtx);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task(); // execute
                }
            });
        }
    }

    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> future<invoke_result_t<F, Args...>> {
        using ReturnType = invoke_result_t<F, Args...>;
        packaged_task<ReturnType()> task(bind(forward<F>(f), forward<Args>(args)...));
        future<ReturnType> result = task.get_future();
        {
            lock_guard lock(mtx);
            if (stop) throw runtime_error("submit on stopped pool");
            tasks.emplace(move(task));
        }
        cv.notify_one();
        return result;
    }

    size_t pending() {
        lock_guard lock(mtx);
        return tasks.size();
    }

    void shutdown() {
        stop = true;
        cv.notify_all();
        for (auto& w : workers) w.join();
    }

    ~ThreadPool() {
        if (!stop) shutdown();
    }
};

// ---------- MAIN ----------
int main() {
    cout << "=== THREAD POOL ===\n\n";

    ThreadPool pool(4);
    cout << "Pool with " << 4 << " threads\n\n";

    // Submit tasks
    vector<future<int>> futures;
    for (int i = 0; i < 12; ++i) {
        futures.push_back(pool.submit([i] {
            this_thread::sleep_for(chrono::milliseconds(100 * (i % 3 + 1)));
            return i * i;
        }));
    }

    cout << "Tasks pending: " << pool.pending() << "\n";

    // Collect results
    int total = 0;
    for (auto& f : futures) {
        total += f.get();
    }

    cout << "Sum of squares (0..11): " << total << "\n";
    pool.shutdown();
    cout << "Pool shutdown complete\n";

    /*
     * SAMPLE I/O:
     * Pool with 4 threads
     *
     * Tasks pending: 12
     * Sum of squares (0..11): 506
     * Pool shutdown complete
     *
     * (0²+1²+2²+...+11² = 506)
     */

    return 0;
}
