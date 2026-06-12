/*
 * Phase 16 – Data Structures
 * 194. Deque (Double-Ended Queue)
 *
 * Theory:
 * - Deque (double-ended queue) allows insertion and deletion at both ends.
 * - Operations: push_front, push_back, pop_front, pop_back, front, back.
 * - Implementations:
 *   1. Array (circular, with front/rear at both ends) – O(1) all operations.
 *   2. Linked list (doubly linked) – O(1) all operations, more memory.
 * - Applications: sliding window maximum, palindrome checking, undo-redo,
 *   maintaining a deque in BFS 0-1.
 *
 * Common Mistakes:
 * - Array-based: confusion when front wraps past index 0.
 * - Linked-list: forgetting to update both prev/next.
 * - Off-by-one in circular buffer management.
 *
 * Interview Questions:
 * 1. Implement a deque with all operations O(1).
 * 2. Sliding window maximum using deque.
 * 3. Design a queue that supports max() in O(1).
 * 4. Check if a string is a palindrome using deque.
 */

#include <iostream>
#include <vector>
#include <stdexcept>

// ---------- Array (circular) Deque ----------
template<typename T, size_t Capacity = 10>
class ArrayDeque {
    T data[Capacity];
    size_t front_idx;
    size_t rear_idx;
    size_t count;

public:
    ArrayDeque() : front_idx(0), rear_idx(0), count(0) {}

    bool empty() const { return count == 0; }
    bool full() const { return count == Capacity; }
    size_t size() const { return count; }
    size_t capacity() const { return Capacity; }

    void push_front(const T& val) {
        if (full()) throw std::overflow_error("Deque full");
        front_idx = (front_idx - 1 + Capacity) % Capacity;
        data[front_idx] = val;
        ++count;
    }

    void push_back(const T& val) {
        if (full()) throw std::overflow_error("Deque full");
        data[rear_idx] = val;
        rear_idx = (rear_idx + 1) % Capacity;
        ++count;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("Deque empty");
        front_idx = (front_idx + 1) % Capacity;
        --count;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Deque empty");
        rear_idx = (rear_idx - 1 + Capacity) % Capacity;
        --count;
    }

    T& front() {
        if (empty()) throw std::underflow_error("Deque empty");
        return data[front_idx];
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Deque empty");
        return data[front_idx];
    }

    T& back() {
        if (empty()) throw std::underflow_error("Deque empty");
        return data[(rear_idx - 1 + Capacity) % Capacity];
    }

    const T& back() const {
        if (empty()) throw std::underflow_error("Deque empty");
        return data[(rear_idx - 1 + Capacity) % Capacity];
    }
};

// ---------- Linked List Deque (Doubly Linked) ----------
template<typename T>
struct DQNode {
    T data;
    DQNode* prev;
    DQNode* next;
    explicit DQNode(const T& val) : data(val), prev(nullptr), next(nullptr) {}
};

template<typename T>
class LinkedDeque {
    DQNode<T>* head;
    DQNode<T>* tail;
    size_t count;

public:
    LinkedDeque() : head(nullptr), tail(nullptr), count(0) {}
    ~LinkedDeque() { clear(); }

    bool empty() const { return count == 0; }
    size_t size() const { return count; }

    void push_front(const T& val) {
        DQNode<T>* new_node = new DQNode<T>(val);
        if (!head) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        ++count;
    }

    void push_back(const T& val) {
        DQNode<T>* new_node = new DQNode<T>(val);
        if (!tail) {
            head = tail = new_node;
        } else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        ++count;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("Deque empty");
        DQNode<T>* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        --count;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Deque empty");
        DQNode<T>* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        --count;
    }

    T& front() {
        if (empty()) throw std::underflow_error("Deque empty");
        return head->data;
    }

    const T& front() const {
        if (empty()) throw std::underflow_error("Deque empty");
        return head->data;
    }

    T& back() {
        if (empty()) throw std::underflow_error("Deque empty");
        return tail->data;
    }

    const T& back() const {
        if (empty()) throw std::underflow_error("Deque empty");
        return tail->data;
    }

    void clear() {
        while (head) {
            DQNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }
};

// ---------- Application: sliding window maximum ----------
std::vector<int> sliding_window_max(const std::vector<int>& arr, int k) {
    std::vector<int> result;
    // Use C++ std::deque for simplicity
    std::deque<int> dq;
    for (size_t i = 0; i < arr.size(); ++i) {
        // Remove out-of-window indices
        while (!dq.empty() && dq.front() <= static_cast<int>(i) - k)
            dq.pop_front();
        // Remove smaller elements from back
        while (!dq.empty() && arr[dq.back()] <= arr[i])
            dq.pop_back();
        dq.push_back(static_cast<int>(i));
        if (i >= static_cast<size_t>(k - 1))
            result.push_back(arr[dq.front()]);
    }
    return result;
}

int main() {
    using namespace std;

    cout << "=== Deque Demo ===\n\n";

    // ---- Array Deque ----
    cout << "--- ArrayDeque ---\n";
    ArrayDeque<int, 10> ad;
    ad.push_back(10); ad.push_back(20); ad.push_back(30);
    cout << "After push_back 10,20,30: front=" << ad.front() << " back=" << ad.back() << '\n';
    ad.push_front(5); ad.push_front(1);
    cout << "After push_front 5,1: front=" << ad.front() << " back=" << ad.back() << '\n';
    ad.pop_front();
    cout << "After pop_front: front=" << ad.front() << '\n';
    ad.pop_back();
    cout << "After pop_back: back=" << ad.back() << '\n';

    // ---- Linked Deque ----
    cout << "\n--- LinkedDeque ---\n";
    LinkedDeque<int> ld;
    ld.push_back(100); ld.push_back(200);
    ld.push_front(50); ld.push_front(25);
    cout << "front=" << ld.front() << " back=" << ld.back() << " size=" << ld.size() << '\n';
    ld.pop_front(); ld.pop_back();
    cout << "After pop front+back: front=" << ld.front() << " back=" << ld.back() << '\n';

    // ---- Application: Sliding Window Maximum ----
    cout << "\n--- Sliding Window Maximum (k=3) ---\n";
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    cout << "Array: ";
    for (int x : arr) cout << x << ' ';
    cout << "\nResult: ";
    for (int x : sliding_window_max(arr, 3)) cout << x << ' ';
    cout << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "push_front/push_back: O(1)\n";
    cout << "pop_front/pop_back: O(1)\n";
    cout << "Front/Back: O(1)\n";
    cout << "Sliding window max: O(n)\n";

    return 0;
}
