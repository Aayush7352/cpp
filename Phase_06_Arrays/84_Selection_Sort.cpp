/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Selection Sort                          ║
* ║  Program 84 — Selection sort, comparison with bubble sort  ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Divides array into sorted (left) and unsorted (right) halves.
*   Each iteration finds the minimum element in the unsorted part
*   and swaps it to the end of the sorted part.
* - Not stable (typical implementation swaps across equal elements).
* - Does O(n²) comparisons but only O(n) swaps — good when writes
*   are expensive.
*
* SAMPLE I/O:
*   Before: [29 10 14 37 13]
*   After:  [10 13 14 29 37]
*
* COMPLEXITY:
*   Time: O(n²) always — no early exit.
*   Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: How does selection sort compare to bubble sort?
*    A: Selection sort does O(n) swaps vs O(n²) for bubble. Both do
*       O(n²) comparisons. Selection is faster when swap cost is high.
* 2. Q: Can selection sort be made stable?
*    A: Yes — instead of swapping, insert the min element by shifting
*       (like insertion sort with a rotated section). This increases
*       writes to O(n²) though.
* 3. Q: What is the best-case time?
*    A: O(n²) — it always scans the full unsorted part.
*
* COMMON MISTAKES:
* - Starting inner loop from i (should be i+1).
* - Not tracking min_idx properly (min_idx should be i initially).
* - Swapping arr[i] with arr[min_idx] without checking i != min_idx
*   (optional optimisation).
*/

#include <iostream>
#include <array>

int main() {
    std::array arr{29, 10, 14, 37, 13};
    auto n = arr.size();

    std::cout << "Before: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── Selection sort ───────────────────────────────────────
    for (std::size_t i = 0; i < n - 1; ++i) {
        std::size_t min_idx = i;
        for (std::size_t j = i + 1; j < n; ++j)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        if (min_idx != i)
            std::swap(arr[i], arr[min_idx]);
    }

    std::cout << "After:  ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Swaps: O(n) ≈ " << (n - 1) << " (at most n-1)\n";

    return 0;
}
