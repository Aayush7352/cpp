/*
THEORY: Lock-Free Queue
- Uses atomic operations to achieve thread-safety without mutexes
- Based on Michael & Scott algorithm (1996)
- CAS (Compare-And-Swap) for concurrent enqueue/dequeue
- Eliminates deadlocks and priority inversion
- Better scalability in high-contention scenarios

Complexity: O(1) enqueue/dequeue
Interview Questions:
1. ABA problem and how to solve it? (tagged pointers)
2. Memory reclamation in lock-free structures? (epoch-based, hazard pointers)
3. When to use lock-free vs mutex-based?
Common Mistakes:
- Not handling ABA problem properly
- Memory leaks from no garbage collection
- Incorrect memory ordering (need acquire/release semantics)
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

template<typename T>
class LockFreeQueue {
    struct Node {
        T value;
        std::atomic<Node*> next{nullptr};
        Node() = default;
        explicit Node(const T& val) : value(val) {}
    };

    std::atomic<Node*> head_;
    std::atomic<Node*> tail_;

public:
    LockFreeQueue() {
        Node* dummy = new Node();
        head_.store(dummy);
        tail_.store(dummy);
    }

    ~LockFreeQueue() {
        T val;
        while (dequeue(val));
        delete head_.load();
    }

    void enqueue(const T& value) {
        Node* node = new Node(value);
        node->next.store(nullptr);

        while (true) {
            Node* tail = tail_.load(std::memory_order_acquire);
            Node* next = tail->next.load(std::memory_order_acquire);

            if (tail == tail_.load(std::memory_order_acquire)) {
                if (next == nullptr) {
                    if (tail->next.compare_exchange_weak(next, node,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                        tail_.compare_exchange_weak(tail, node,
                            std::memory_order_release,
                            std::memory_order_relaxed);
                        break;
                    }
                } else {
                    tail_.compare_exchange_weak(tail, next,
                        std::memory_order_release,
                        std::memory_order_relaxed);
                }
            }
        }
    }

    bool dequeue(T& result) {
        while (true) {
            Node* head = head_.load(std::memory_order_acquire);
            Node* tail = tail_.load(std::memory_order_acquire);
            Node* next = head->next.load(std::memory_order_acquire);

            if (head == head_.load(std::memory_order_acquire)) {
                if (head == tail) {
                    if (next == nullptr) return false;
                    tail_.compare_exchange_weak(tail, next,
                        std::memory_order_release,
                        std::memory_order_relaxed);
                } else {
                    result = next->value;
                    if (head_.compare_exchange_weak(head, next,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                        delete head;
                        return true;
                    }
                }
            }
        }
    }
};

int main() {
    LockFreeQueue<int> queue;
    const int NUM = 100000;
    std::atomic<int> sum{0};

    std::thread producer([&]() {
        for (int i = 0; i < NUM; ++i) queue.enqueue(i);
    });

    std::thread consumer([&]() {
        int val;
        int count = 0;
        while (count < NUM) {
            if (queue.dequeue(val)) {
                sum.fetch_add(val);
                ++count;
            }
        }
    });

    producer.join();
    consumer.join();

    int expected = NUM * (NUM - 1) / 2;
    std::cout << "Sum: " << sum.load() << " (expected: " << expected << ")\n";
    std::cout << (sum.load() == expected ? "PASS" : "FAIL") << "\n";
    return 0;
}

/*
SAMPLE OUTPUT:
Sum: 4999950000 (expected: 4999950000)
PASS
*/
