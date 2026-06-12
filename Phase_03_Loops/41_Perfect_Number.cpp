/*
 * ====================================================================
 * Program 41: Perfect Number
 * Theory:
 *   A perfect number is a positive integer that equals the sum of its
 *   proper divisors (all positive divisors except the number itself).
 *
 *   Examples:
 *     6 = 1 + 2 + 3
 *     28 = 1 + 2 + 4 + 7 + 14
 *     496, 8128
 *
 *   Optimization: Divisors come in pairs (i, n/i). If i divides n,
 *   both i and n/i are divisors. Check only up to sqrt(n).
 *   Be careful not to double-count when i == n/i (perfect square).
 *
 * Sample Input/Output:
 *   Enter a number: 28
 *   28 is a perfect number.
 *   Divisors: 1 + 2 + 4 + 7 + 14 = 28
 *
 *   Enter a number: 12
 *   12 is NOT a perfect number.
 *   Sum of divisors: 1 + 2 + 3 + 4 + 6 = 16
 *
 *   Perfect numbers up to 10000:
 *   6 28 496 8128
 *
 * Time Complexity:  O(√n) for one number
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: How many perfect numbers are known?
 *   A: As of 2024, 51 perfect numbers are known (all even). It is
 *      unknown if any odd perfect numbers exist.
 *
 *   Q: Euclid-Euler theorem?
 *   A: Every even perfect number has the form 2^(p-1)(2^p - 1)
 *      where 2^p - 1 is a Mersenne prime.
 *
 * Common Mistakes:
 *   - Not excluding the number itself from divisor sum
 *   - Including 1 as a divisor but forgetting that 1 is not perfect
 *   - Double-counting square roots (e.g., for 16: 4 counted twice)
 * ====================================================================
 */

#include <iostream>
#include <cmath>

// --- Check perfect number with sqrt optimization ---
bool isPerfect(int n)
{
    if (n < 2) return false;

    int sum{1};  // 1 is always a proper divisor for n > 1
    int limit = static_cast<int>(std::sqrt(n));

    for (int i{2}; i <= limit; ++i)
    {
        if (n % i == 0)
        {
            sum += i;
            int pair = n / i;
            if (pair != i)  // avoid double-counting squares
            {
                sum += pair;
            }
        }
    }

    return sum == n;
}

int main()
{
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    if (isPerfect(num))
        std::cout << num << " is a perfect number.\n";
    else
        std::cout << num << " is NOT a perfect number.\n";

    // --- Generate perfect numbers up to 10000 ---
    int limit{10000};
    std::cout << "\nPerfect numbers up to " << limit << ":\n";
    for (int i{2}; i <= limit; ++i)
    {
        if (isPerfect(i)) std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
