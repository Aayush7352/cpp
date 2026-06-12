/*
THEORY: Ring Buffer (Circular Buffer)
- Fixed-size buffer that wraps around when full
- FIFO data structure with O(1) push/pop
- Used in audio processing, network drivers, producer-consumer
- Thread-safe single-producer single-consumer (SPSC) variant
- Avoids dynamic memory allocation during operation

Complexity: O(1) push/pop
Interview Questions:
1. How to handle buffer full? (overwrite/block/throw)
2. SPSC vs MPMC ring buffer differences?
3. How to use ring buffer for audio processing?
Common Mistakes:
- Off-by-one errors in head/tail management
- Not handling wrap-around correctly
- Race conditions in head/tail updates
*/

#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <cassert>

template<typename T, size_t Capacity>
class RingBuffer {
    alignas(64) std::array<T, Capacity> buffer_{};
    alignas(64) std::atomic<size_t> head_{0};  // Read position
    alignas(64) std::atomic<size_t> tail_{0};  // Write position

    static constexpr size_t Mask = Capacity - 1;
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be power of 2");

public:
    bool push(const T& value) {
        size_t tail = tail_.load(std::memory_order_relaxed);
        size_t head = head_.load(std::memory_order_acquire);
        if ((tail - head) >= Capacity) return false; // Full
        buffer_[tail & Mask] = value;
        tail_.store(tail + 1, std::memory_order_release);
        return true;
    }

    bool pop(T& value) {
        size_t head = head_.load(std::memory_order_relaxed);
        size_t tail = tail_.load(std::memory_order_acquire);
        if (head == tail) return false; // Empty
        value = buffer_[head & Mask];
        head_.store(head + 1, std::memory_order_release);
        return true;
    }

    size_t size() const {
        return tail_.load() - head_.load();
    }

    bool empty() const { return size() == 0; }
    bool full() const { return size() >= Capacity; }
};

int main() {
    RingBuffer<int, 1024> buffer;
    std::atomic<bool> done{false};

    std::thread producer([&]() {
        for (int i = 0; i < 10000; ++i) {
            while (!buffer.push(i)) {
                std::this_thread::yield();
            }
        }
        done.store(true);
    });

    std::thread consumer([&]() {
        int prev = -1;
        int val;
        while (!done.load() || !buffer.empty()) {
            if (buffer.pop(val)) {
                assert(val == prev + 1);
                prev = val;
            } else {
                std::this_thread::yield();
            }
        }
        std::cout << "Consumed all values, last: " << prev << "\n";
    });

    producer.join();
    consumer.join();
    std::cout << "Ring buffer test: PASS\n";
    return 0;
}

/*
SAMPLE OUTPUT:
Consumed all values, last: 9999
Ring buffer test: PASS
*/
