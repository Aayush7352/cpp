/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — Reverse a String                       ║
* ║  Program 92 — std::reverse, two-pointer, rbegin/rend       ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - std::reverse (C++11, <algorithm>) reverses any bidirectional range.
* - Two-pointer: swap characters from both ends converging to middle.
* - Reverse iterators (rbegin/rend) allow constructing a reversed
*   copy without modifying the original.
*
* SAMPLE I/O:
*   Original: "Hello"
*   Manual:   "olleH"
*   std::reverse: "olleH"
*   Copy (rbegin): "olleH"
*
* COMPLEXITY:
*   Time: O(n).
*   Space: O(1) in-place; O(n) for copy via reverse iterators.
*
* INTERVIEW QUESTIONS:
* 1. Q: Reverse a string in place vs create a copy?
*    A: std::reverse modifies original; use string(s.rbegin(), s.rend())
*       to create a reversed copy.
* 2. Q: What about UTF-8 strings?
*    A: Reversing UTF-8 byte-wise produces invalid sequences. Use a
*       library like ICU to reverse grapheme clusters.
*
* COMMON MISTAKES:
* - Off-by-one in two-pointer: condition should be left < right.
* - Forgetting #include <algorithm> for std::reverse.
* - Expecting reverse iterators to modify the original.
*/

#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string s{"Hello"};

    std::cout << "Original: \"" << s << "\"\n";

    // ── 1. Two-pointer manual ────────────────────────────────
    std::size_t left = 0, right = s.size() - 1;
    while (left < right) {
        std::swap(s[left], s[right]);
        ++left; --right;
    }
    std::cout << "Manual:   \"" << s << "\"\n";

    // ── 2. std::reverse ──────────────────────────────────────
    std::reverse(s.begin(), s.end());
    std::cout << "std::reverse: \"" << s << "\"\n";

    // ── 3. Reverse copy using reverse iterators ──────────────
    std::string rev(s.rbegin(), s.rend());
    std::cout << "Copy (rbegin): \"" << rev << "\"\n";

    return 0;
}
