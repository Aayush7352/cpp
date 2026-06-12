/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Reverse Array                           ║
* ║  Program 77 — In-place reversal, std::reverse, two-pointer ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - In-place reversal swaps symmetrical elements: arr[i] ↔ arr[n-1-i].
* - Two-pointer: left at 0, right at n-1; swap and converge.
* - std::reverse (C++11, <algorithm>) works on any bidirectional range.
*
* SAMPLE I/O:
*   Original: [1 2 3 4 5]
*   Manual:   [5 4 3 2 1]
*   Reverse:  [1 2 3 4 5]
*
* COMPLEXITY:
*   Time: O(n) — must touch every element once.
*   Space: O(1) — in-place.
*
* INTERVIEW QUESTIONS:
* 1. Q: Is std::reverse in-place?
*    A: Yes — it swaps elements, no extra storage beyond temporaries.
* 2. Q: Can you reverse only part of an array?
*    A: Yes — std::reverse(begin + i, begin + j) reverses [i, j).
* 3. Q: Reverse a linked list instead?
*    A: Iteratively rewire pointers; recursively reach tail and rewire
*       on unwind.
*
* COMMON MISTAKES:
* - Swapping twice (reversing then reversing again returns original).
* - Off-by-one loop: condition should be i < n/2, not i <= n/2.
* - Using std::reverse with wrong iterator pair (must be [first, last)).
*/

#include <iostream>
#include <algorithm>
#include <array>

int main() {
    std::array arr{1, 2, 3, 4, 5};
    const auto print = [&]{
        for (auto x : arr) std::cout << x << ' ';
        std::cout << '\n';
    };

    std::cout << "Original: "; print();

    // ── 1. Manual two-pointer ────────────────────────────────
    std::size_t left = 0, right = arr.size() - 1;
    while (left < right) {
        std::swap(arr[left], arr[right]);
        ++left; --right;
    }
    std::cout << "Manual:   "; print();

    // ── 2. Using std::reverse ────────────────────────────────
    std::reverse(arr.begin(), arr.end());
    std::cout << "Reverse:  "; print();

    return 0;
}
