/*
 * Phase 16 – Data Structures
 * 202. Heap (Binary Heap)
 *
 * Theory:
 * - Complete binary tree where every node satisfies heap property.
 * - Max-heap: parent ≥ children. Min-heap: parent ≤ children.
 * - Stored as array: children of i are at 2i+1, 2i+2.
 * - Operations:
 *   push (insert at end, bubble up / sift up): O(log n)
 *   pop (swap root with last, sift down): O(log n)
 *   heapify (build heap from array): O(n)
 *   heap sort: O(n log n)
 * - d-ary heap: each node has d children; push O(log_d n), pop O(d log_d n).
 *
 * Common Mistakes:
 * - Off-by-one in child/parent index.
 * - Heap property violation after push/pop.
 * - Forgetting to update size.
 * - In-place heap sort: confusing max-heap vs min-heap order.
 *
 * Interview Questions:
 * 1. Implement heap sort.
 * 2. Find k largest/smallest elements using heap.
 * 3. Median of running stream using two heaps.
 * 4. Merge k sorted arrays using heap.
 * 5. Check if array is a valid heap.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>

template<typename T, typename Compare = std::less<T>>
class BinaryHeap {
    std::vector<T> data;
    Compare comp;

    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t left(size_t i) const { return 2 * i + 1; }
    size_t right(size_t i) const { return 2 * i + 2; }

    void sift_up(size_t i) {
        while (i > 0 && comp(data[i], data[parent(i)])) {
            std::swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }

    void sift_down(size_t i) {
        size_t n = data.size();
        while (true) {
            size_t smallest = i;
            size_t l = left(i);
            size_t r = right(i);
            if (l < n && comp(data[l], data[smallest])) smallest = l;
            if (r < n && comp(data[r], data[smallest])) smallest = r;
            if (smallest == i) break;
            std::swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    BinaryHeap() = default;

    explicit BinaryHeap(const std::vector<T>& arr) : data(arr) {
        heapify();
    }

    void push(const T& val) {
        data.push_back(val);
        sift_up(data.size() - 1);
    }

    void pop() {
        if (data.empty()) throw std::underflow_error("Heap empty");
        std::swap(data[0], data.back());
        data.pop_back();
        if (!data.empty()) sift_down(0);
    }

    const T& top() const {
        if (data.empty()) throw std::underflow_error("Heap empty");
        return data[0];
    }

    void heapify() {
        if (data.empty()) return;
        for (size_t i = data.size() / 2; i > 0; --i)
            sift_down(i - 1);
    }

    [[nodiscard]] size_t size() const { return data.size(); }
    [[nodiscard]] bool empty() const { return data.empty(); }

    // Returns sorted vector (for max-heap: ascending; for min-heap: descending)
    std::vector<T> heap_sort() {
        std::vector<T> sorted;
        sorted.reserve(data.size());
        while (!data.empty()) {
            sorted.push_back(top());
            pop();
        }
        return sorted;
    }

    const std::vector<T>& get_data() const { return data; }
};

// ---------- d-ary Heap ----------
template<typename T, size_t D = 2, typename Compare = std::less<T>>
class DAryHeap {
    static_assert(D >= 2, "D must be >= 2");
    std::vector<T> data;
    Compare comp;

    size_t parent(size_t i) const { return (i - 1) / D; }
    size_t child(size_t i, size_t k) const { return D * i + k + 1; }

    void sift_up(size_t i) {
        while (i > 0 && comp(data[i], data[parent(i)])) {
            std::swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }

    void sift_down(size_t i) {
        size_t n = data.size();
        while (true) {
            size_t smallest = i;
            for (size_t k = 0; k < D; ++k) {
                size_t c = child(i, k);
                if (c < n && comp(data[c], data[smallest]))
                    smallest = c;
            }
            if (smallest == i) break;
            std::swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    void push(const T& val) {
        data.push_back(val);
        sift_up(data.size() - 1);
    }

    void pop() {
        if (data.empty()) throw std::underflow_error("Heap empty");
        std::swap(data[0], data.back());
        data.pop_back();
        if (!data.empty()) sift_down(0);
    }

    const T& top() const {
        if (data.empty()) throw std::underflow_error("Heap empty");
        return data[0];
    }

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
};

int main() {
    using namespace std;

    cout << "=== Binary Heap Demo ===\n\n";

    // ---- Min-Heap ----
    cout << "--- Min-Heap (default) ---\n";
    BinaryHeap<int> min_heap;
    for (int v : {5, 3, 8, 1, 9, 2}) min_heap.push(v);

    cout << "Push: 5,3,8,1,9,2\n";
    cout << "Top: " << min_heap.top() << '\n';
    cout << "Size: " << min_heap.size() << '\n';

    cout << "Extract all: ";
    while (!min_heap.empty()) {
        cout << min_heap.top() << ' ';
        min_heap.pop();
    }
    cout << "(ascending)\n";

    // ---- Max-Heap ----
    cout << "\n--- Max-Heap (greater<int>) ---\n";
    BinaryHeap<int, std::greater<int>> max_heap;
    for (int v : {5, 3, 8, 1, 9, 2}) max_heap.push(v);
    cout << "Extract all: ";
    while (!max_heap.empty()) {
        cout << max_heap.top() << ' ';
        max_heap.pop();
    }
    cout << "(descending)\n";

    // ---- Heapify ----
    cout << "\n--- Heapify an array ---\n";
    vector<int> arr = {10, 3, 5, 7, 1, 8, 2};
    cout << "Original: ";
    for (int x : arr) cout << x << ' ';
    cout << '\n';

    BinaryHeap<int> heap_from_arr(arr);
    cout << "Heap: ";
    for (int x : heap_from_arr.get_data()) cout << x << ' ';
    cout << " (min-heap order)\n";

    // ---- Heap Sort ----
    cout << "\n--- Heap Sort ---\n";
    BinaryHeap<int> hs;
    for (int v : {4, 2, 9, 1, 7, 5, 3, 8, 6}) hs.push(v);
    auto sorted = hs.heap_sort();
    cout << "Sorted: ";
    for (int x : sorted) cout << x << ' ';
    cout << '\n';

    // ---- d-ary Heap ----
    cout << "\n--- 3-ary Heap ---\n";
    DAryHeap<int, 3> ternary;
    for (int v : {5, 3, 8, 1, 9, 2, 7, 4, 6}) ternary.push(v);
    cout << "Extract all: ";
    while (!ternary.empty()) {
        cout << ternary.top() << ' ';
        ternary.pop();
    }
    cout << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Push: O(log n)\n";
    cout << "Pop: O(log n)\n";
    cout << "Top: O(1)\n";
    cout << "Heapify: O(n)\n";
    cout << "Heap sort: O(n log n)\n";
    cout << "d-ary heap push: O(log_d n), pop: O(d log_d n)\n";

    return 0;
}
