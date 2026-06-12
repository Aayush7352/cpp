/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Frequency Count                         ║
* ║  Program 79 — unordered_map, array for limited range       ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Frequency: count occurrences of each distinct element.
* - unordered_map (C++11, <unordered_map>): hash-based, O(1) average
*   per insertion/lookup, O(n) total for n elements.
* - For small-integer ranges (e.g. 0–255), a plain array of size
*   MAX_VAL+1 is faster and cache-friendly.
*
* SAMPLE I/O:
*   Array: [3 1 4 1 5 9 2 6 5 3 5]
*   Frequency (unordered_map):
*     3 → 2, 1 → 2, 4 → 1, 5 → 3, 9 → 1, 2 → 1, 6 → 1
*   Frequency (array method, range 0–9):
*     Same counts.
*
* COMPLEXITY:
*   Time: O(n) — single pass.
*   Space: O(k) where k = number of distinct elements.
*
* INTERVIEW QUESTIONS:
* 1. Q: When is the array method better than unordered_map?
*    A: When the range of possible values is small and known a priori;
*       e.g. ASCII characters (0-127), grades (0-100).
* 2. Q: What if values are negative?
*    A: Shift index: val - min_val, or use unordered_map.
* 3. Q: Why not use std::map?
*    A: map is O(log n) per operation — slower for counting.
*
* COMMON MISTAKES:
* - For unordered_map: modifying bucket while iterating.
* - Array index out of bounds when value exceeds pre-allocated range.
* - Forgetting to initialise count array to zero.
*/

#include <iostream>
#include <unordered_map>
#include <array>
#include <algorithm>

int main() {
    constexpr std::array arr{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    // ── Method 1: unordered_map — general purpose ─────────────
    std::unordered_map<int, int> freq;
    for (int v : arr) ++freq[v];          // operator[] default-inserts 0

    std::cout << "Frequency (unordered_map):\n";
    for (auto [val, cnt] : freq)
        std::cout << "  " << val << " → " << cnt << '\n';

    // ── Method 2: array for limited range (0-9 here) ─────────
    constexpr int RANGE = 10;
    int counts[RANGE]{};

    for (int v : arr)
        if (v >= 0 && v < RANGE) ++counts[v];

    std::cout << "Frequency (array 0–9):\n";
    for (int i = 0; i < RANGE; ++i)
        if (counts[i] > 0)
            std::cout << "  " << i << " → " << counts[i] << '\n';

    return 0;
}
