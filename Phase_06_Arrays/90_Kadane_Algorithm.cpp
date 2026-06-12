/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Kadane's Algorithm (Max Subarray Sum)   ║
* ║  Program 90 — O(n) maximum subarray sum                    ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Kadane's algorithm finds the maximum sum of a contiguous subarray
*   in O(n) time.
* - Maintain two variables:
*     • max_ending_here: best sum ending at current position
*     • max_so_far: overall best sum
* - If max_ending_here drops below 0, reset it to 0 (start new subarray).
* - Works for arrays with both positive and negative numbers.
* - For all-negative arrays, Kadane as written returns 0; the
*   modification below tracks the maximum element if all are negative.
*
* SAMPLE I/O:
*   Array: [-2 1 -3 4 -1 2 1 -5 4]
*   Max subarray sum = 6  (from indices 3 to 6: [4 -1 2 1])
*
* COMPLEXITY:
*   Time: O(n) — single pass.
*   Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: Kadane vs divide-and-conquer approach?
*    A: Kadane is O(n), O(1) space. Divide-and-conquer is O(n log n).
* 2. Q: How to handle all-negative arrays?
*    A: Kadane returns 0. To handle correctly, initialise max_so_far
*       to arr[0] and don't reset to 0 — or use the variant below.
* 3. Q: Can you also return the subarray indices?
*    A: Yes — track start/end when max_ending_here is reset/updated.
*
* COMMON MISTAKES:
* - Not resetting max_ending_here to 0 when it goes negative.
* - Forgetting that an empty subarray (sum = 0) is allowed/allowed
*   depending on problem constraints.
* - Using int and getting overflow for large arrays.
*/

#include <iostream>
#include <array>
#include <algorithm>

int main() {
    constexpr std::array arr{-2, 1, -3, 4, -1, 2, 1, -5, 4};
    constexpr auto n = arr.size();
    static_assert(n > 0);

    // ── Standard Kadane (allows empty subarray → sum ≥ 0) ────
    int max_ending_here = 0;
    int max_so_far = 0;
    for (auto v : arr) {
        max_ending_here = std::max(0, max_ending_here + v);
        max_so_far = std::max(max_so_far, max_ending_here);
    }
    std::cout << "Max subarray sum (empty allowed) = " << max_so_far << '\n';

    // ── Kadane variant (handles all-negative, no empty) ──────
    int max_ending = arr[0];
    int max_total = arr[0];
    for (std::size_t i = 1; i < n; ++i) {
        max_ending = std::max(arr[i], max_ending + arr[i]);
        max_total = std::max(max_total, max_ending);
    }
    std::cout << "Max subarray sum (non-empty) = " << max_total << '\n';

    // ── With subarray indices ────────────────────────────────
    max_ending = 0;
    max_so_far = 0;
    int start = 0, end = 0, temp_start = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (max_ending + arr[i] < 0) {
            max_ending = 0;
            temp_start = static_cast<int>(i) + 1;
        } else {
            max_ending += arr[i];
        }
        if (max_ending > max_so_far) {
            max_so_far = max_ending;
            start = temp_start;
            end = static_cast<int>(i);
        }
    }
    std::cout << "Subarray indices: [" << start << ", " << end << "]\n";

    return 0;
}
