/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Minimum Element                         ║
* ║  Program 75 — Manual min, std::min_element                 ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Identical logic to max but with reversed comparison.
* - std::min_element returns iterator to first smallest element.
* - std::minmax_element returns both min and max in a single O(n) pass
*   (≈ 3n/2 comparisons vs 2n for separate scans).
*
* SAMPLE I/O:
*   Array: [12 3 7 1 9 1 5]
*   Min element = 1  (at index 3)
*   Using min_element = 1
*   Using minmax_element: min = 1, max = 12
*
* COMPLEXITY:
*   Time: O(n)
*   Space: O(1)
*
* INTERVIEW QUESTIONS:
* 1. Q: How many comparisons does minmax_element need?
*    A: About 3n/2. It processes elements in pairs: one comparison to
*       determine local min/max, then one each to update global.
* 2. Q: What happens if the array has only one element?
*    A: That sole element is both min and max.
*
* COMMON MISTAKES:
* - Setting initial min to a very large value instead of arr[0].
* - Forgetting to check n > 0 before dereferencing begin()/end().
* - Confusing min_element (returns iterator) with min (returns value).
*/

#include <iostream>
#include <algorithm>
#include <array>

int main() {
    constexpr std::array arr{12, 3, 7, 1, 9, 1, 5};
    constexpr auto n = arr.size();
    static_assert(n > 0);

    // ── 1. Manual scan ───────────────────────────────────────
    int min_val = arr[0];
    std::size_t min_idx = 0;
    for (std::size_t i = 1; i < n; ++i) {
        if (arr[i] < min_val) {
            min_val = arr[i];
            min_idx = i;
        }
    }
    std::cout << "Min element = " << min_val
              << "  (at index " << min_idx << ")\n";

    // ── 2. std::min_element ──────────────────────────────────
    auto it = std::min_element(arr.begin(), arr.end());
    std::cout << "Using min_element = " << *it << '\n';

    // ── 3. std::minmax_element (C++11) ──────────────────────
    auto [min_it, max_it] = std::minmax_element(arr.begin(), arr.end());
    std::cout << "minmax_element: min = " << *min_it
              << ", max = " << *max_it << '\n';

    return 0;
}
