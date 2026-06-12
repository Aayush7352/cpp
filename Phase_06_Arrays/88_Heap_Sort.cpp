/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Heap Sort                               ║
* ║  Program 88 — std::make_heap, std::sort_heap, manual       ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Heap sort: build a max-heap, then repeatedly extract the maximum
*   by swapping root with last element and heapifying down.
* - In-place O(1) space (apart from recursion/iteration variables).
* - Not stable.
* - C++ standard library provides heap operations (<algorithm>):
*   • std::make_heap — build heap from range (O(n))
*   • std::push_heap — insert element (O(log n))
*   • std::pop_heap — move max to end (O(log n))
*   • std::sort_heap — repeatedly pop (O(n log n))
*
* SAMPLE I/O:
*   Before: [12 11 13 5 6 7]
*   After:  [5 6 7 11 12 13]
*
* COMPLEXITY:
*   Time: O(n log n) — both build and sort.
*   Space: O(1) iterative.
*
* INTERVIEW QUESTIONS:
* 1. Q: Why does make_heap run in O(n) instead of O(n log n)?
*    A: It uses Floyd's algorithm: heapify from last parent to root.
*       Total work is Σ 2^(h-i) * i ≈ O(n).
* 2. Q: Compare heap sort with quick sort.
*    A: Heap sort guarantees O(n log n) and is in-place. Quick sort
*       is usually faster in practice due to better cache locality.
* 3. Q: Is heap sort stable?
*    A: No — heap operations can reorder equal elements arbitrarily.
*
* COMMON MISTAKES:
* - Forgetting to call make_heap before sort_heap.
* - Using max-heap for ascending sort (should pop max to end).
* - Off-by-one in manual heapify: indices must be < n.
*/

#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    std::vector<int> arr{12, 11, 13, 5, 6, 7};

    std::cout << "Before: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── Manual heap sort ─────────────────────────────────────
    auto heapify = [&](auto&& self, std::size_t n, std::size_t i) -> void {
        std::size_t largest = i;
        std::size_t l = 2 * i + 1;
        std::size_t r = 2 * i + 2;
        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;
        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            self(self, n, largest);
        }
    };

    // Build heap (rearrange array)
    for (std::size_t i = arr.size() / 2; i > 0; --i)
        heapify(heapify, arr.size(), i - 1);

    // Extract elements one by one
    for (std::size_t i = arr.size(); i > 0; --i) {
        std::swap(arr[0], arr[i - 1]);
        heapify(heapify, i - 1, 0);
    }

    std::cout << "Manual: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── Using standard library ───────────────────────────────
    std::vector<int> arr2{12, 11, 13, 5, 6, 7};

    std::make_heap(arr2.begin(), arr2.end());   // max-heap
    std::sort_heap(arr2.begin(), arr2.end());   // ascending

    std::cout << "std::heap: ";
    for (auto x : arr2) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
