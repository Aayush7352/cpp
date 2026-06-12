/*
 * ====================================================================
 * Program 49: Power Function (Exponentiation by Squaring)
 * Theory:
 *   Compute base^exponent efficiently using exponentiation by squaring.
 *
 *   Algorithm:
 *     If exponent is even:  result = (base^2)^(exp/2)
 *     If exponent is odd:   result = base × base^(exp-1)
 *
 *   This reduces O(n) multiplications to O(log n).
 *
 *   Standard: std::pow from <cmath> works with doubles (floating point).
 *   Our custom version uses integers with overflow checking.
 *
 * Sample Input/Output:
 *   Enter base and exponent: 2 10
 *   2^10 = 1024
 *   2^10 (std::pow) = 1024
 *
 *   Enter base and exponent: 3 5
 *   3^5 = 243
 *
 *   Enter base and exponent: 2 -2
 *   Negative exponent not supported for integer power.
 *
 * Time Complexity:  O(log exp) for exponentiation by squaring
 * Space Complexity: O(1) iterative, O(log exp) recursive
 *
 * Common Interview Questions:
 *   Q: Why is exponentiation by squaring faster?
 *   A: Instead of exp multiplications, we square the base and halve
 *      the exponent, achieving O(log exp) time.
 *
 *   Q: Handle negative exponents?
 *   A: Integer pow with negative exponent is fractional. Use floating
 *      point (std::pow) or reject.
 *
 * Common Mistakes:
 *   - Not handling exponent = 0 (result should be 1)
 *   - Integer overflow without warning
 *   - Using the loop the naive way (O(n)) in interviews
 * ====================================================================
 */

#include <iostream>
#include <cmath>
#include <limits>
#include <cstdlib>

// --- Iterative exponentiation by squaring ---
long long powerIterative(long long base, int exp)
{
    if (exp < 0) return -1;  // not supported

    long long result{1};
    long long b = base;

    while (exp > 0)
    {
        if (exp & 1)  // if odd
        {
            if (result > std::numeric_limits<long long>::max() / b)
            {
                std::cerr << "Overflow detected!\n";
                return 0;
            }
            result *= b;
        }
        exp >>= 1;  // exp /= 2

        if (exp > 0)
        {
            if (b > std::numeric_limits<long long>::max() / b)
            {
                std::cerr << "Overflow detected!\n";
                return 0;
            }
            b *= b;
        }
    }

    return result;
}

// --- Recursive exponentiation by squaring ---
long long powerRecursive(long long base, int exp)
{
    if (exp < 0) return -1;
    if (exp == 0) return 1;
    if (exp == 1) return base;

    long long half = powerRecursive(base, exp / 2);
    if (half > std::numeric_limits<long long>::max() / half)
    {
        std::cerr << "Overflow detected!\n";
        return 0;
    }

    if (exp % 2 == 0)
        return half * half;
    else
    {
        if (half * half > std::numeric_limits<long long>::max() / base)
        {
            std::cerr << "Overflow detected!\n";
            return 0;
        }
        return half * half * base;
    }
}

int main()
{
    long long base{};
    int exp{};
    std::cout << "Enter base and exponent: ";
    std::cin >> base >> exp;

    if (exp < 0)
    {
        std::cout << "Negative exponent not supported for integer power.\n";
        return 1;
    }

    std::cout << base << "^" << exp << " = " << powerIterative(base, exp) << '\n';
    std::cout << base << "^" << exp << " (recursive) = " << powerRecursive(base, exp) << '\n';
    std::cout << base << "^" << exp << " (std::pow) = "
              << static_cast<long long>(std::pow(static_cast<double>(base), exp)) << '\n';

    return 0;
}
