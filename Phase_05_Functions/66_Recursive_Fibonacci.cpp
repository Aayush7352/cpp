/*
 * 66_Recursive_Fibonacci.cpp
 * --------------------------
 * Recursive Fibonacci with memoization (top-down DP).
 *
 * Theory:
 * - Fibonacci: F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2).
 * - Naive recursion is O(2^n). Memoization caches results,
 *   reducing to O(n) time and O(n) space.
 * - Uses static std::vector (or std::unordered_map) as cache.
 *
 * Sample I/O:
 * Enter n: 10
 * F(10) = 55
 *
 * Complexity: O(n) time, O(n) space (with memo)
 * Interview Questions:
 * Q: What is the space complexity of the iterative method?
 * A: O(1) space (just two variables); faster than memoized recursion.
 * Q: Can memoized Fibonacci overflow the stack?
 * A: Yes, recursion depth = n; for large n (e.g. >10^5), use iteration.
 *
 * Common Mistakes:
 * - Forgetting base cases (causes infinite recursion).
 * - Not using long long (overflow at ~F(93) even with 64-bit).
 * - Dynamic cache reallocation on every call (static vector avoids this).
 */

#include <iostream>
#include <vector>

long long fibMemo(int n) {
    static std::vector<long long> memo{0, 1};
    if (n < static_cast<int>(memo.size()))
        return memo[n];
    memo.push_back(fibMemo(n - 1) + fibMemo(n - 2));
    return memo.back();
}

int main() {
    int n;
    std::cout << "Enter n: ";
    std::cin >> n;

    std::cout << "F(" << n << ") = " << fibMemo(n) << '\n';

    return 0;
}
