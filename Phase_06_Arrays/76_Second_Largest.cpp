/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Second Largest Element                  ║
* ║  Program 76 — Two-pass and single-pass O(n) solutions      ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Second largest: the largest element that is strictly smaller
*   than the maximum.
* - Two-pass: find max (O(n)), then find largest < max (O(n)) → O(2n).
* - Single-pass: track both max and second_max while iterating once.
*
* SAMPLE I/O:
*   Array: [10 5 8 12 7 12 6]
*   Two-pass:  second largest = 10
*   One-pass:  second largest = 10
*
* COMPLEXITY:
*   Both O(n). Single pass reduces constant factor.
*   Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: What if there is no second largest element?
*    A: If all elements are equal or n < 2, there is no second largest.
*       Return a sentinel (e.g. INT_MIN) or throw an exception.
* 2. Q: Can you find the k-th largest this way?
*    A: For k=2 yes. For general k use a min-heap of size k (O(n log k))
*       or quickselect (O(n) average).
*
* COMMON MISTAKES:
* - Not handling duplicates correctly (second != max).
* - Initialising second_max to INT_MIN — fails if array has negative
*   numbers but actual second largest is smaller than INT_MIN? Actually
*   INT_MIN is fine in C++ if you use a flag. Safer: use std::optional.
* - Off-by-one in two-pass when skipping the max.
*/

#include <iostream>
#include <array>
#include <optional>
#include <climits>

int main() {
    constexpr std::array arr{10, 5, 8, 12, 7, 12, 6};
    constexpr auto n = arr.size();

    // ── Two-pass method ──────────────────────────────────────
    int max = arr[0];
    for (std::size_t i = 1; i < n; ++i)
        if (arr[i] > max) max = arr[i];

    std::optional<int> second;
    for (auto v : arr)
        if (v < max && (!second.has_value() || v > second.value()))
            second = v;

    std::cout << "Two-pass: second largest = ";
    second.has_value() ? std::cout << second.value() : std::cout << "none";
    std::cout << '\n';

    // ── Single-pass method ───────────────────────────────────
    int max1 = arr[0];
    std::optional<int> max2;
    for (std::size_t i = 1; i < n; ++i) {
        if (arr[i] > max1) {
            max2 = max1;
            max1 = arr[i];
        } else if (arr[i] != max1 && (!max2.has_value() || arr[i] > max2.value())) {
            max2 = arr[i];
        }
    }

    std::cout << "One-pass:  second largest = ";
    max2.has_value() ? std::cout << max2.value() : std::cout << "none";
    std::cout << '\n';

    return 0;
}
