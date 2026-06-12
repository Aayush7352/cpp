/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Bubble Sort                             ║
* ║  Program 83 — Optimised bubble sort with early exit        ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Repeatedly steps through the array, compares adjacent elements,
*   and swaps them if out of order. Largest elements "bubble" to end.
* - Optimisation: if no swaps in a pass, the array is sorted — early exit.
* - Stable: equal elements retain relative order (only swaps when >, not ≥).
*
* SAMPLE I/O:
*   Before: [64 34 25 12 22 11 90]
*   After:  [11 12 22 25 34 64 90]
*
* COMPLEXITY:
*   Time: O(n²) worst/average, O(n) best (already sorted, early exit).
*   Space: O(1) in-place.
*
* INTERVIEW QUESTIONS:
* 1. Q: Is bubble sort stable?
*    A: Yes — if you only swap on `arr[j] > arr[j+1]` (not ≥).
* 2. Q: How does early exit affect worst-case?
*    A: No effect — worst case (reverse sorted) still does all passes.
* 3. Q: Compare bubble sort with insertion sort.
*    A: Both O(n²). Insertion sort does fewer swaps on average and
*       is adaptive; bubble sort is rarely used in practice.
*
* COMMON MISTAKES:
* - Inner loop condition should be `j < n - i - 1`, not `j < n - i`.
* - Not resetting swapped flag (or forgetting to break early).
* - Using `>=` for comparison, breaking stability.
*/

#include <iostream>
#include <array>

int main() {
    std::array arr{64, 34, 25, 12, 22, 11, 90};
    auto n = arr.size();

    std::cout << "Before: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── Optimised bubble sort ────────────────────────────────
    for (std::size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (std::size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;    // early exit
    }

    std::cout << "After:  ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Stable: yes (swap only on >)\n";

    return 0;
}
