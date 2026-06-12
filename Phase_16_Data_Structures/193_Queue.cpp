/*
 * Phase 16 – Data Structures
 * 193. Queue
 *
 * Theory:
 * - FIFO (First-In, First-Out) data structure.
 * - Enqueue (add to rear), Dequeue (remove from front), Front, Rear, isEmpty.
 * - Implementations:
 *   1. Array (circular) – O(1) enqueue/dequeue, fixed capacity.
 *   2. Linked list – dynamic, O(1) enqueue/dequeue.
 * - Circular array avoids waste by wrapping front/rear indices.
 * - Applications: BFS, scheduling, buffer management, printer queue.
 *
 * Common Mistakes:
 * - Not using modulo for circular wrap-around.
 * - Confusing front/rear: front = index of first element, rear = index of last.
 * - Enqueue/dequeue on empty queue (underflow).
 * - Full queue detection in circular array.
 *
 * Interview Questions:
 * 1. Implement a queue using two stacks.
 * 2. Implement a circular deque.
 * 3. Design a thread-safe queue.
 * 4. Implement a priority queue using heap.
 * 5. Generate numbers from a given set using queue.
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>

// ---------- Circular Array Queue ----------
template<typename T, size_t Capacity = 10>
class CircularQueue {
    T data[Capacity];
    size_t front_idx;
    size_t rear_idx;
    size_t count;

public:
    CircularQueue() : front_idx(0), rear_idx(0), count(0) {}

    void enqueue(const T& val) {
        if (full()) throw std::overflow_error("Queue full");
        data[rear_idx] = val;
        rear_idx = (rear_idx + 1) % Capacity;
        ++count;
    }

    void dequeue() {
        if (empty()) throw std::underflow_error("Queue empty");
        front_idx = (front_idx + 1) % Capacity;
        --count;
    }

    T& front() {
        if (empty()) throw std::underflow_error("Queue empty");
        return data[front_idx];
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Queue empty");
        return data[front_idx];
    }

    T& rear() {
        if (empty()) throw std::underflow_error("Queue empty");
        return data[(rear_idx - 1 + Capacity) % Capacity];
    }

    const T& rear() const {
        if (empty()) throw std::underflow_error("Queue empty");
        return data[(rear_idx - 1 + Capacity) % Capacity];
    }

    bool empty() const { return count == 0; }
    bool full() const { return count == Capacity; }
    size_t size() const { return count; }
    size_t capacity() const { return Capacity; }

    // Debug: print raw buffer state
    void debug() const {
        std::cout << "front=" << front_idx << " rear=" << rear_idx
                  << " count=" << count << " cap=" << Capacity << '\n';
    }
};

// ---------- Linked List Queue ----------
template<typename T>
struct QNode {
    T data;
    QNode* next;
    explicit QNode(const T& val) : data(val), next(nullptr) {}
};

template<typename T>
class LinkedQueue {
    QNode<T>* front_node;
    QNode<T>* rear_node;
    size_t count;

public:
    LinkedQueue() : front_node(nullptr), rear_node(nullptr), count(0) {}
    ~LinkedQueue() { clear(); }

    void enqueue(const T& val) {
        QNode<T>* new_node = new QNode<T>(val);
        if (!rear_node) {
            front_node = rear_node = new_node;
        } else {
            rear_node->next = new_node;
            rear_node = new_node;
        }
        ++count;
    }

    void dequeue() {
        if (empty()) throw std::underflow_error("Queue empty");
        QNode<T>* temp = front_node;
        front_node = front_node->next;
        if (!front_node) rear_node = nullptr;
        delete temp;
        --count;
    }

    T& front() {
        if (empty()) throw std::underflow_error("Queue empty");
        return front_node->data;
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Queue empty");
        return front_node->data;
    }

    T& rear() {
        if (empty()) throw std::underflow_error("Queue empty");
        return rear_node->data;
    }

    const T& rear() const {
        if (empty()) throw std::underflow_error("Queue empty");
        return rear_node->data;
    }

    bool empty() const { return front_node == nullptr; }
    size_t size() const { return count; }

    void clear() {
        while (front_node) {
            QNode<T>* temp = front_node;
            front_node = front_node->next;
            delete temp;
        }
        rear_node = nullptr;
        count = 0;
    }
};

// ---------- Application: generate numbers with digits {5,6} ----------
void generate_numbers(int n) {
    LinkedQueue<std::string> q;
    q.enqueue("5");
    q.enqueue("6");
    for (int i = 0; i < n; ++i) {
        std::string curr = q.front(); q.dequeue();
        std::cout << curr << ' ';
        q.enqueue(curr + "5");
        q.enqueue(curr + "6");
    }
    std::cout << '\n';
}

int main() {
    using namespace std;

    cout << "=== Queue Demo ===\n\n";

    // ---- Circular Array Queue ----
    cout << "--- CircularQueue (capacity=5) ---\n";
    CircularQueue<int, 5> cq;
    cq.enqueue(10); cq.enqueue(20); cq.enqueue(30);
    cout << "Front: " << cq.front() << ", Rear: " << cq.rear() << ", Size: " << cq.size() << '\n';
    cq.dequeue();
    cout << "After dequeue, front: " << cq.front() << '\n';
    cq.enqueue(40); cq.enqueue(50); cq.enqueue(60);
    cout << "Front: " << cq.front() << ", Rear: " << cq.rear() << ", Size: " << cq.size() << '\n';
    // Circular wrap: next enqueue would wrap
    cout << "Circular buffer state: ";
    cq.debug();

    // Drain queue
    cout << "Queue elements (FIFO): ";
    while (!cq.empty()) {
        cout << cq.front() << ' ';
        cq.dequeue();
    }
    cout << '\n';

    // ---- Linked Queue ----
    cout << "\n--- LinkedQueue ---\n";
    LinkedQueue<int> lq;
    for (int i = 1; i <= 5; ++i) lq.enqueue(i * 100);
    cout << "Front: " << lq.front() << ", Rear: " << lq.rear() << '\n';
    while (!lq.empty()) {
        cout << lq.front() << ' ';
        lq.dequeue();
    }
    cout << "(FIFO order)\n";

    // ---- Application ----
    cout << "\n--- Generate numbers with digits {5,6} ---\n";
    cout << "First 10: ";
    generate_numbers(10);

    cout << "\n--- Complexity ---\n";
    cout << "Enqueue: O(1)\n";
    cout << "Dequeue: O(1)\n";
    cout << "Front/Rear: O(1)\n";
    cout << "Space: O(n)\n";

    return 0;
}
