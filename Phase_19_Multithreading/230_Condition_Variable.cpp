/*
 * ====================================================================
 * PROGRAM 230: Condition_Variable.cpp
 * TOPIC    : Multithreading - std::condition_variable
 * ====================================================================
 * THEORY:
 * std::condition_variable allows threads to wait for a condition.
 *   wait(lock, predicate) - atomically unlocks and blocks until
 *     notified and predicate returns true.
 *   notify_one()  - wakes one waiting thread
 *   notify_all()  - wakes all waiting threads
 *
 * SPURIOUS WAKEPUPS:
 *   Threads may wake without notification. Always use a predicate!
 *
 * COMPLEXITY:
 *   wait/notify: O(1) average
 *
 * INTERVIEW QUESTIONS:
 * Q: Why must the predicate be used with wait?
 * A: To guard against spurious wakeups and missed signals.
 *
 * Q: What happens if notify is called before wait?
 * A: The waiting thread blocks forever (missed wakeup). Use predicate.
 *
 * Q: Can condition_variable be used without mutex?
 * A: No, it always requires a mutex for correct synchronization.
 *
 * COMMON MISTAKES:
 * - Not using a predicate in wait (spurious wakeup)
 * - Notifying before thread is waiting (missed wakeup)
 * - Using notify_all when notify_one suffices (performance)
 * - Forgetting to lock the mutex before calling wait
 * ====================================================================
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;

void worker(int id) {
    unique_lock lock(mtx);
    cv.wait(lock, []{ return ready; }); // predicate prevents spurious wakeup
    cout << "Worker " << id << " starting\n";
} // lock released here

void setter() {
    this_thread::sleep_for(chrono::milliseconds(200));
    {
        lock_guard lock(mtx);
        ready = true;
    }
    cv.notify_all(); // wake all workers
}

// Manual wait (without predicate) - showing spurious wakeup handling
void manual_worker(int id) {
    unique_lock lock(mtx);
    while (!ready) { // manual predicate check
        cv.wait(lock);
        // May wake spuriously; re-check condition
    }
    cout << "Manual worker " << id << " done\n";
}

int main() {
    cout << "=== CONDITION VARIABLE ===\n\n";

    // Predicate-based wait
    cout << "--- Predicate-based wait ---\n";
    thread t1(worker, 1);
    thread t2(worker, 2);
    thread t3(worker, 3);
    thread s(setter);
    t1.join(); t2.join(); t3.join(); s.join();

    ready = false;
    cout << "\n--- Manual wait (while loop) ---\n";
    thread t4(manual_worker, 4);
    thread t5(manual_worker, 5);
    this_thread::sleep_for(chrono::milliseconds(100));
    {
        lock_guard lock(mtx);
        ready = true;
    }
    cv.notify_all();
    t4.join(); t5.join();

    /*
     * SAMPLE I/O:
     * --- Predicate-based wait ---
     * (200ms pause)
     * Worker 1 starting
     * Worker 2 starting
     * Worker 3 starting
     *
     * --- Manual wait (while loop) ---
     * Manual worker 4 done
     * Manual worker 5 done
     */

    return 0;
}
