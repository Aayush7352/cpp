/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Maximum Element                         ║
* ║  Program 74 — Manual max, std::max_element, custom comp.   ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Find the largest element in an unsorted array: O(n) by scanning.
* - std::max_element returns an iterator to the first occurrence of
*   the largest element (stable w.r.t. ordering of equal maxima).
* - A custom comparator (e.g. lambda) changes the definition of "largest".
*
* SAMPLE I/O:
*   Array: [4 2 9 7 5 9 1]
*   Max element = 9  (at index 2)
*   Using max_element = 9
*   Max by absolute value = 9
*
* COMPLEXITY:
*   Time: O(n)
*   Space: O(1)
*
* INTERVIEW QUESTIONS:
* 1. Q: Can you find max in fewer than n-1 comparisons?
*    A: Theoretically n-1 is optimal for finding the max in an
*       unsorted array; every element must be seen.
* 2. Q: What if there are multiple equal maxima?
*    A: std::max_element returns the FIRST. std::minmax_element
*       returns the last maximum (by spec).
*
* COMMON MISTAKES:
* - Initialising max = 0 (fails for all-negative arrays).
* - Off-by-one in loop: starting with max = arr[0] is safe if n ≥ 1.
* - Dereferencing end() when array is empty.
*/

#include <iostream>
#include <algorithm>
#include <iterator>
#include <array>
#include <cstdlib>   // std::abs

int main() {
    constexpr std::array arr{4, 2, 9, 7, -5, 9, 1};
    constexpr auto n = arr.size();
    static_assert(n > 0);

    // ── 1. Manual scan ───────────────────────────────────────
    int max_val = arr[0];
    std::size_t max_idx = 0;
    for (std::size_t i = 1; i < n; ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
            max_idx = i;
        }
    }
    std::cout << "Max element = " << max_val
              << "  (at index " << max_idx << ")\n";

    // ── 2. std::max_element ──────────────────────────────────
    auto it = std::max_element(arr.begin(), arr.end());
    std::cout << "Using max_element = " << *it << '\n';

    // ── 3. Custom comparator: max absolute value ─────────────
    auto abs_max = std::max_element(arr.begin(), arr.end(),
                                    [](int a, int b) {
                                        return std::abs(a) < std::abs(b);
                                    });
    std::cout << "Max by absolute value = " << *abs_max << '\n';

    return 0;
}
