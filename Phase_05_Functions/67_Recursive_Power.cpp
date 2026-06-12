/*
 * 67_Recursive_Power.cpp
 * ----------------------
 * Fast exponentiation (binary exponentiation) using recursion.
 *
 * Theory:
 * - pow(x,n) = x * pow(x,n-1)  (O(n), naive)
 * - Fast exponentiation:
 *   - if n == 0: return 1
 *   - if n is even: pow(x*x, n/2)
 *   - if n is odd: x * pow(x*x, (n-1)/2)
 * - O(log n) time, recursion depth = O(log n).
 *
 * Sample I/O:
 * Enter base and exponent: 2 10
 * 2^10 = 1024
 *
 * Complexity: O(log n) time, O(log n) stack space
 * Interview Questions:
 * Q: What about negative exponents?
 * A: Return 1.0 / pow(x, -n) with double return type.
 * Q: Is there a std::pow and when should you use it?
 * A: std::pow works for floating point; integer fast pow is useful
 *    when you need exact integer results or custom mod.
 *
 * Common Mistakes:
 * - Forgetting base case n == 0.
 * - Integer overflow for large exponents (use long long).
 * - Not handling negative exponent properly.
 */

#include <iostream>

long long fastPow(long long x, long long n) {
    if (n == 0) return 1;
    long long half = fastPow(x * x, n / 2);
    return (n % 2 == 0) ? half : x * half;
}

int main() {
    long long x, n;
    std::cout << "Enter base and exponent: ";
    std::cin >> x >> n;

    std::cout << x << "^" << n << " = " << fastPow(x, n) << '\n';

    return 0;
}
