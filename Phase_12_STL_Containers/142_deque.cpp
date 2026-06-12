/*
==================================================================
142. std::deque
==================================================================
Theory:
- Double-ended queue: O(1) push/pop at both ends
- Block structure: segmented array of fixed-size blocks
- Random access O(1) (but slightly slower than vector)
- No reallocation semantics (no reserve/capacity)
- Iterator invalidation: insertion at ends doesn't invalidate
  references (but may invalidate iterators)
*/

#include <iostream>
#include <deque>
#include <algorithm>

int main() {
    std::cout << "=== std::deque\n\n";

    // --- Construction ---
    std::deque<int> d;
    std::deque<int> d2(5, 100);
    std::deque<int> d3{1, 2, 3, 4, 5};

    std::cout << "d2: "; for (int x : d2) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Double-ended operations ---
    std::cout << "Double-ended ops:\n";
    d.push_back(10);
    d.push_front(20);
    d.push_back(30);
    d.push_front(40);
    std::cout << "  After pushes: ";
    for (int x : d) std::cout << x << " ";

    std::cout << "\n  Front: " << d.front() << ", Back: " << d.back();

    d.pop_front();
    d.pop_back();
    std::cout << "\n  After pops:  ";
    for (int x : d) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Random access ---
    std::cout << "Random access:\n";
    for (size_t i = 0; i < d.size(); ++i)
        std::cout << "  d[" << i << "] = " << d[i] << "\n";
    std::cout << "\n";

    // --- Insert/Erase in middle ---
    std::cout << "Insert/Erase:\n";
    d.insert(d.begin() + 1, 99);
    std::cout << "  After insert at pos 1: ";
    for (int x : d) std::cout << x << " ";

    d.erase(d.begin() + 2);
    std::cout << "\n  After erase at pos 2: ";
    for (int x : d) std::cout << x << " ";
    std::cout << "\n\n";

    // --- deque as sliding window ---
    std::cout << "Sliding window (max sum of 3 consecutive):\n";
    std::deque<int> window{1, 2, 3, 4, 5, 6};
    // Monotonic deque for sliding window max
    // (conceptual; actual implementation uses indices)

    // --- Block structure visualization ---
    std::cout << "\nBlock structure concept:\n";
    std::cout << "  deque uses fixed-size blocks (e.g., 512 bytes each)\n";
    std::cout << "  Map (pointer array) points to blocks\n";
    std::cout << "  No large contiguous allocation needed\n";

    // --- Algorithms on deque ---
    std::deque<int> unsorted{9, 1, 7, 3, 5};
    std::sort(unsorted.begin(), unsorted.end());
    std::cout << "\nSorted: ";
    for (int x : unsorted) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
d2: 100 100 100 100 100

Double-ended ops:
  After pushes: 40 20 10 30
  Front: 40, Back: 30
  After pops:  20 10

Random access:
  d[0] = 20
  d[1] = 10

Insert/Erase:
  After insert at pos 1: 20 99 10
  After erase at pos 2: 20 99

Sorted: 1 3 5 7 9
==================================================================
Complexity:
- push_front/push_back: O(1)
- pop_front/pop_back: O(1)
- Random access: O(1)
- Insert/erase in middle: O(n)
==================================================================
Interview Questions:
1. When to use deque vs vector?
   -> deque: frequent push/pop at both ends. vector: mostly
      push_back only, needs contiguous memory.

2. Does deque have capacity/reserve?
   -> No. deque doesn't expose capacity. It grows block-by-block.

3. Does insertion at ends invalidate references?
   -> No (references remain valid). Iterators may be invalidated
      (implementation-defined).
==================================================================
Common Mistakes:
- Expecting contiguous memory layout (not guaranteed)
- Using reserve() on deque (doesn't exist)
- Assuming O(1) is same speed as vector (deque has extra indirection)
- Frequent insert/erase in middle (deque is not good for this)
==================================================================
*/
