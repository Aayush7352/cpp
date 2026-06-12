/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Linear Search with Sentinel             ║
* ║  Program 81 — Basic linear search, sentinel optimisation   ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Linear scan until element found or end reached.
* - Sentinel optimisation: place target at the end of array so the
*   loop always finds it. This eliminates the bounds check on each
*   iteration — reduces comparisons from 2n to ~n.
*   After the loop, check if the found position == n (sentinel only).
*
* SAMPLE I/O:
*   Array: [45 23 67 12 89 34 56]
*   Search for 67 → found at index 2
*   Search for 99 → not found
*
* COMPLEXITY:
*   Time: O(n) worst-case, O(n/2) average.
*   Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: When is sentinel search useful?
*    A: When search is a hot path and n is large; it saves ~n
*       comparisons in the worst case.
* 2. Q: Does it work on read-only arrays?
*    A: No — sentinel modifies the array (or requires a writable copy).
* 3. Q: Can we use it with std::find?
*    A: std::find does not use the sentinel technique. If you need
*       sentinel speed, implement it manually.
*
* COMMON MISTAKES:
* - Placing sentinel beyond actual array bounds (must have buffer).
* - Forgetting to restore the original element after search.
* - Off-by-one: sentinel position should be n, not n-1.
*/

#include <iostream>
#include <vector>
#include <algorithm>

// Sentinel-based search — ASSUMES vec has space for one extra element
int sentinel_search(std::vector<int>& vec, int target) {
    std::size_t n = vec.size();
    // Ensure we have room for sentinel (caller must guarantee)
    if (vec.capacity() == vec.size())
        vec.reserve(n + 1);      // may reallocate — but we already have space

    int orig_back = vec.back();
    if (vec.back() != target)
        vec.push_back(target);    // sentinel

    std::size_t i = 0;
    while (vec[i] != target) ++i;

    // Restore
    vec.pop_back();
    if (i != n)
        return static_cast<int>(i);
    return -1;
}

int main() {
    std::vector<int> arr{45, 23, 67, 12, 89, 34, 56};
    arr.reserve(arr.size() + 1);  // guarantee room for sentinel

    std::cout << "Array: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── Standard linear search ───────────────────────────────
    int key = 67;
    auto it = std::find(arr.begin(), arr.end(), key);
    if (it != arr.end())
        std::cout << key << " found at index " << (it - arr.begin()) << '\n';
    else
        std::cout << key << " not found\n";

    // ── Sentinel search ─────────────────────────────────────
    int idx = sentinel_search(arr, 99);
    if (idx != -1)
        std::cout << "99 found at index " << idx << '\n';
    else
        std::cout << "99 not found\n";

    idx = sentinel_search(arr, 89);
    if (idx != -1)
        std::cout << "89 found at index " << idx << '\n';

    return 0;
}
