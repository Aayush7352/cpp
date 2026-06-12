/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Insertion Sort                          ║
* ║  Program 85 — Online algorithm property, stable sort       ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Builds the sorted sequence one element at a time by repeatedly
*   inserting the current element into its correct position among
*   previously sorted elements.
* - Online: processes elements as they arrive, without needing the
*   entire array up front.
* - Stable: does not swap past equal elements.
*
* SAMPLE I/O:
*   Before: [12 11 13 5 6]
*   After:  [5 6 11 12 13]
*
* COMPLEXITY:
*   Time: O(n²) worst, O(n) best (already sorted).
*   Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: When is insertion sort preferred?
*    A: Small arrays (n < 50), nearly-sorted data, or as the base
*       case in hybrid sorts like introsort / timsort.
* 2. Q: Why is it called "online"?
*    A: It can sort elements as they are received one by one; the
*       sorted list is always available after each insertion.
* 3. Q: Compare with selection sort.
*    A: Insertion sort does fewer comparisons on average with
*       partially sorted data; selection sort always does n²/2
*       comparisons. Insertion sort is stable; selection usually is not.
*
* COMMON MISTAKES:
* - Forgetting to shift elements (not just swap — insertion sort
*   typically shifts right and places key).
* - Off-by-one in inner loop when shifting.
* - Using arr[j] > key instead of arr[j] > key for stability
*   (use > not >= to preserve relative order of equal elements).
*/

#include <iostream>
#include <array>

int main() {
    std::array arr{12, 11, 13, 5, 6};
    auto n = arr.size();

    std::cout << "Before: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── Insertion sort ───────────────────────────────────────
    for (std::size_t i = 1; i < n; ++i) {
        int key = arr[i];
        std::size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }

    std::cout << "After:  ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Stable: yes, Online: yes\n";

    return 0;
}
