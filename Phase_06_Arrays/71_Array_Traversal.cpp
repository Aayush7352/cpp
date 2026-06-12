/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Array Traversal                         ║
* ║  Program 71 — Four ways to traverse an array               ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Array traversal visits every element exactly once. O(n).
* - C++ offers: index loop, range-for (C++11), raw pointer iterators,
*   and std::for_each (C++11, <algorithm>).
* - Range-for internally uses begin()/end() — works with raw arrays
*   and all standard containers.
* - std::for_each accepts a callable (lambda, function pointer, functor).
*
* COMPLEXITIES:
*   Time: O(n) — each approach must visit all n elements.
*   Space: O(1) — no extra space beyond the loop variable.
*
* INTERVIEW QUESTIONS:
* 1. Q: Which traversal is fastest?
*    A: Usually identical after optimisation. Range-for is safest and
*       clearest; it cannot go out of bounds or suffer off-by-one errors.
* 2. Q: Can range-for modify elements?
*    A: Yes — use `auto&` or `auto&&`, not `auto`.
* 3. Q: When would you prefer index loop over range-for?
*    A: When you need the index itself, or need to stride by >1.
*
* COMMON MISTAKES:
* - Off-by-one in index loop: `i <= n` instead of `i < n`.
* - Copying large objects: `auto x : arr` instead of `const auto& x`.
* - Passing array to function: raw array decays to pointer; template
*   or std::array / std::vector preserves size information.
*/

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstddef>

int main() {
    // ── Sample data ──────────────────────────────────────────
    int arr[]{10, 20, 30, 40, 50};
    constexpr std::size_t n{sizeof(arr) / sizeof(arr[0])};   // compile-time

    std::cout << "Array: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── 1. Classic for-loop with index ───────────────────────
    std::cout << "1) Index loop  : ";
    for (std::size_t i{0}; i < n; ++i)
        std::cout << arr[i] << ' ';
    std::cout << '\n';

    // ── 2. Range-based for (C++11) ───────────────────────────
    std::cout << "2) Range-for   : ";
    for (int val : arr)
        std::cout << val << ' ';
    std::cout << '\n';

    // ── 3. Iterator-style (pointer arithmetic) ───────────────
    std::cout << "3) Iterators   : ";
    for (auto it = std::begin(arr); it != std::end(arr); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // ── 4. std::for_each with lambda (C++11) ─────────────────
    std::cout << "4) std::for_each: ";
    std::for_each(std::begin(arr), std::end(arr),
                  [](int v) { std::cout << v << ' '; });
    std::cout << '\n';

    return 0;
}
