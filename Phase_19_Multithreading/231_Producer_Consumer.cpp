/*
 * ====================================================================
 * PROGRAM 231: Producer_Consumer.cpp
 * TOPIC    : Multithreading - Producer-Consumer (Bounded Buffer)
 * ====================================================================
 * THEORY:
 * Classic synchronization problem using mutex + condition_variables.
 * Bounded buffer with fixed capacity:
 *   - Producer blocks when buffer is full
 *   - Consumer blocks when buffer is empty
 *   - Multiple producers and consumers can share the buffer
 *
 * SYNCHRONIZATION:
 *   mutex mtx - protects buffer access
 *   cv not_full  - signaled when slot becomes available
 *   cv not_empty - signaled when item becomes available
 *
 * COMPLEXITY:
 *   push/pop: O(1) amortized
 *   Blocking: efficient (no busy-waiting)
 *
 * INTERVIEW QUESTIONS:
 * Q: What happens if there are multiple consumers?
 * A: notify_all wakes all; they contend for the mutex; one gets item.
 *
 * Q: How to handle priority items?
 * A: Use priority_queue instead of queue, or multiple queues.
 *
 * Q: Can we use a single condition variable?
 * A: Yes, but less efficient (wakes both producers and consumers).
 *
 * COMMON MISTAKES:
 * - Notifying outside the lock (performance issue, not correctness)
 * - Using notify_all when notify_one is sufficient
 * - Not handling exceptions in critical sections
 * - Deadlock if lock order is inconsistent
 * ====================================================================
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <random>

using namespace std;

template<typename T>
class BoundedBuffer {
    queue<T> buffer;
    size_t capacity;
    mutex mtx;
    condition_variable not_full;
    condition_variable not_empty;

public:
    BoundedBuffer(size_t cap) : capacity(cap) {}

    void push(const T& item) {
        unique_lock lock(mtx);
        not_full.wait(lock, [this] { return buffer.size() < capacity; });
        buffer.push(item);
        lock.unlock();
        not_empty.notify_one();
    }

    T pop() {
        unique_lock lock(mtx);
        not_empty.wait(lock, [this] { return !buffer.empty(); });
        T item = buffer.front();
        buffer.pop();
        lock.unlock();
        not_full.notify_one();
        return item;
    }

    size_t size() {
        lock_guard lock(mtx);
        return buffer.size();
    }
};

// ---------- MAIN ----------
int main() {
    cout << "=== PRODUCER-CONSUMER (Bounded Buffer) ===\n\n";

    BoundedBuffer<int> buffer(5);
    atomic<bool> done(false);
    const int items_per_producer = 10;
    const int num_producers = 3;
    const int num_consumers = 2;

    // Producers
    auto producer = [&](int id) {
        for (int i = 0; i < items_per_producer; ++i) {
            this_thread::sleep_for(chrono::milliseconds(rand() % 100));
            int val = id * 100 + i;
            buffer.push(val);
            cout << "Producer " << id << " pushed " << val << " (size="
                 << buffer.size() << ")\n";
        }
    };

    // Consumers
    auto consumer = [&](int id) {
        int consumed = 0;
        while (consumed < (items_per_producer * num_producers / num_consumers)) {
            this_thread::sleep_for(chrono::milliseconds(rand() % 150));
            int val = buffer.pop();
            cout << "Consumer " << id << " popped " << val << " (size="
                 << buffer.size() << ")\n";
            ++consumed;
        }
    };

    vector<thread> producers, consumers;
    for (int i = 0; i < num_producers; ++i)
        producers.emplace_back(producer, i + 1);
    for (int i = 0; i < num_consumers; ++i)
        consumers.emplace_back(consumer, i + 1);

    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();

    cout << "\nAll done. Buffer size: " << buffer.size() << "\n";

    /*
     * SAMPLE I/O (abbreviated):
     * Producer 1 pushed 100 (size=1)
     * Producer 2 pushed 200 (size=2)
     * Consumer 1 popped 100 (size=1)
     * Producer 3 pushed 300 (size=2)
     * Consumer 2 popped 200 (size=1)
     * ...
     * All done. Buffer size: 0
     */

    return 0;
}
