/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Remove Duplicates                       ║
* ║  Program 80 — Sorted (in-place), std::unique, unsorted/set ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Sorted array: duplicates are adjacent. Use two-pointer (i for
*   insertion, j for scanning) to compact in-place. O(n).
* - std::unique (C++11, <algorithm>) does exactly this for any
*   forward range, returning the new logical end.
* - Unsorted array: use std::unordered_set to track seen elements.
*   O(n) average, but not in-place (needs extra storage).
*
* SAMPLE I/O:
*   Sorted:   [1 1 2 2 3 4 4 5]
*   Unique:   [1 2 3 4 5]
*   Unsorted: [4 2 4 1 3 2 1]
*   Unique:   [4 2 1 3]  (order of first occurrence)
*
* COMPLEXITY:
*   Sorted in-place: O(n) time, O(1) space.
*   Unsorted with set: O(n) time, O(n) space.
*
* INTERVIEW QUESTIONS:
* 1. Q: Does std::unique remove all but the first of each group?
*    A: Yes — it keeps the first element and overwrites subsequent
*       duplicates.
* 2. Q: After std::unique, what happens to the tail?
*    A: Elements after the returned iterator are in a valid but
*       unspecified state. Call erase() to shrink the container.
* 3. Q: Can you remove duplicates from an unsorted array in-place?
*    A: Not in O(n) without extra space. One O(n²) approach: for each
*       element, check if it appeared before.
*
* COMMON MISTAKES:
* - Forgetting to sort before calling std::unique.
* - Not erasing the tail after std::unique.
* - Using two-pointer but not tracking the write index correctly.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <iterator>

int main() {
    // ── Sorted array: in-place ───────────────────────────────
    std::vector<int> sorted{1, 1, 2, 2, 3, 4, 4, 5};
    std::cout << "Sorted original: ";
    for (auto x : sorted) std::cout << x << ' ';
    std::cout << '\n';

    // Manual two-pointer
    std::size_t write_idx = 1;
    for (std::size_t read_idx = 1; read_idx < sorted.size(); ++read_idx)
        if (sorted[read_idx] != sorted[write_idx - 1])
            sorted[write_idx++] = sorted[read_idx];
    sorted.resize(write_idx);

    std::cout << "Unique (manual): ";
    for (auto x : sorted) std::cout << x << ' ';
    std::cout << '\n';

    // ── Using std::unique ────────────────────────────────────
    std::vector<int> vec{1, 1, 2, 2, 3, 4, 4, 5};
    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());

    std::cout << "Unique (std::unique): ";
    for (auto x : vec) std::cout << x << ' ';
    std::cout << '\n';

    // ── Unsorted array: using std::unordered_set ──────────────
    std::vector<int> unsorted{4, 2, 4, 1, 3, 2, 1};
    std::unordered_set<int> seen;
    std::vector<int> result;

    for (int v : unsorted)
        if (seen.insert(v).second)      // true if actually inserted
            result.push_back(v);

    std::cout << "Unsorted unique: ";
    for (auto x : result) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
