/*
 * ====================================================================
 * Program 44: LCM (Least Common Multiple)
 * Theory:
 *   LCM of two numbers is the smallest positive integer divisible by
 *   both numbers.
 *
 *   Formula: lcm(a, b) = |a × b| / gcd(a, b)
 *
 *   Overflow-safe LCM: Compute a / gcd(a, b) first before multiplying
 *   by b. This prevents overflow when a × b exceeds the integer range.
 *   E.g., a = 1e9, b = 1e9: a × b = 1e18 (may overflow 32-bit).
 *   But a / gcd × b keeps the intermediate result smaller.
 *
 * Sample Input/Output:
 *   Enter two numbers: 12 18
 *   LCM of 12 and 18 is 36
 *
 *   Enter two numbers: 1000000000 2000000000
 *   LCM (overflow-safe): 2000000000
 *
 * Time Complexity:  O(log min(a, b)) due to gcd
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Can LCM be smaller than max(a,b)?
 *   A: No, LCM ≥ max(a,b). The only way lcm = max is if max is
 *      divisible by min.
 *
 *   Q: How to compute LCM of >2 numbers?
 *   A: lcm(a, b, c) = lcm(lcm(a, b), c)
 *
 * Common Mistakes:
 *   - Computing a * b first (risk of overflow)
 *   - Not handling zero (LCM(0, x) is undefined, but some define as 0)
 *   - Forgetting to use unsigned types or long long for large values
 * ====================================================================
 */

#include <iostream>
#include <numeric>  // std::gcd
#include <cstdlib>  // std::abs

// --- LCM using formula: lcm = |a| / gcd(a,b) × |b| ---
long long lcm(int a, int b)
{
    if (a == 0 || b == 0) return 0;  // LCM undefined for zero

    long long absA = std::abs(static_cast<long long>(a));
    long long absB = std::abs(static_cast<long long>(b));

    // Overflow-safe: divide before multiply
    return (absA / static_cast<long long>(std::gcd(a, b))) * absB;
}

int main()
{
    int a{}, b{};
    std::cout << "Enter two numbers: ";
    std::cin >> a >> b;

    long long result = lcm(a, b);
    std::cout << "LCM of " << a << " and " << b << " is " << result << '\n';

    // --- Demonstration of overflow-safe LCM ---
    int big1{1'000'000'000};
    int big2{2'000'000'000};

    long long safeLcm = lcm(big1, big2);
    std::cout << "LCM (overflow-safe) of " << big1 << " and " << big2
              << " = " << safeLcm << '\n';

    // --- Wrong way (would overflow 32-bit int) ---
    // long long badLcm = (static_cast<long long>(big1) * big2) / std::gcd(big1, big2);
    // std::cout << "LCM (unsafe but same with 64-bit): " << badLcm << '\n';

    return 0;
}
