/*
 * ====================================================================
 * Program 37: Fibonacci Sequence
 * Theory:
 *   F(0) = 0, F(1) = 1
 *   F(n) = F(n-1) + F(n-2) for n ≥ 2
 *
 *   Approaches:
 *   - Iterative: O(n) time, O(1) space (best for single query)
 *   - Recursive (naive): O(2ⁿ) time — exponential, very slow
 *   - DP with caching (memoization): O(n) time, O(n) space
 *   - Binet's formula: O(1) using floating point (approximate, rounding
 *     errors for large n)
 *
 * Sample Input/Output:
 *   Enter n (0-92): 10
 *   Iterative: F(10) = 55
 *   Recursive: F(10) = 55
 *   DP (memo): F(10) = 55
 *   Binet:     F(10) = 55
 *   Sequence: 0 1 1 2 3 5 8 13 21 34 55
 *
 * Time Complexity:
 *   Iterative: O(n), Recursive: O(2ⁿ), DP: O(n), Binet: O(1)
 * Space Complexity:
 *   Iterative: O(1), Recursive: O(n) stack, DP: O(n), Binet: O(1)
 *
 * Common Interview Questions:
 *   Q: What's the problem with naive recursive Fibonacci?
 *   A: Exponential time due to repeated calculations: F(5) calls F(4)
 *      and F(3); F(4) calls F(3) and F(2) — F(3) is computed twice.
 *
 *   Q: Which Fibonacci approach is best?
 *   A: Iterative for single query; DP with memo for multiple queries.
 *
 * Common Mistakes:
 *   - Not using unsigned types (Fibonacci is always non-negative)
 *   - Integer overflow after F(93) for 64-bit
 *   - Off-by-one indexing: F(0)=0 vs F(1)=1
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>

// --- Iterative ---
unsigned long long fibIterative(int n)
{
    if (n <= 1) return static_cast<unsigned long long>(n);
    unsigned long long a{0}, b{1}, c{};
    for (int i{2}; i <= n; ++i)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// --- Naive Recursive (slow) ---
unsigned long long fibRecursive(int n)
{
    if (n <= 1) return static_cast<unsigned long long>(n);
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// --- DP with memoization ---
std::unordered_map<int, unsigned long long> memo;
unsigned long long fibDP(int n)
{
    if (n <= 1) return static_cast<unsigned long long>(n);
    if (memo.contains(n)) return memo[n];
    memo[n] = fibDP(n - 1) + fibDP(n - 2);
    return memo[n];
}

// --- Binet's formula (approximate) ---
unsigned long long fibBinet(int n)
{
    double phi = (1.0 + std::sqrt(5.0)) / 2.0;
    double psi = (1.0 - std::sqrt(5.0)) / 2.0;
    return static_cast<unsigned long long>(std::round((std::pow(phi, n) - std::pow(psi, n)) / std::sqrt(5.0)));
}

int main()
{
    int n{};
    std::cout << "Enter n (0-92): ";
    std::cin >> n;

    if (n < 0 || n > 92)
    {
        std::cout << "n must be 0-92 (fits in 64-bit unsigned).\n";
        return 1;
    }

    std::cout << "Iterative: F(" << n << ") = " << fibIterative(n) << '\n';
    if (n <= 40)
        std::cout << "Recursive: F(" << n << ") = " << fibRecursive(n) << '\n';
    else
        std::cout << "Recursive: (too slow for n > 40)\n";
    std::cout << "DP (memo): F(" << n << ") = " << fibDP(n) << '\n';
    std::cout << "Binet:     F(" << n << ") = " << fibBinet(n) << '\n';

    // Print sequence
    if (n <= 20)
    {
        std::cout << "Sequence: ";
        for (int i{}; i <= n; ++i)
            std::cout << fibIterative(i) << ' ';
        std::cout << '\n';
    }

    return 0;
}
