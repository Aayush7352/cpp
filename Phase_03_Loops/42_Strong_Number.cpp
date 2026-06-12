/*
 * ====================================================================
 * Program 42: Strong Number
 * Theory:
 *   A strong number (or Krishnamurthy number) is a number where the
 *   sum of the factorial of its digits equals the number itself.
 *
 *   Example: 145 = 1! + 4! + 5! = 1 + 24 + 120 = 145
 *
 *   Optimization: Precompute factorials for digits 0-9 since a digit
 *   can only be 0-9. This avoids repeated factorial calculations.
 *
 * Sample Input/Output:
 *   Enter a number: 145
 *   145 is a strong number.
 *   1! + 4! + 5! = 1 + 24 + 120 = 145
 *
 *   Enter a number: 40585
 *   40585 is a strong number.
 *
 *   Strong numbers between 1 and 100000:
 *   1 2 145 40585
 *
 * Time Complexity:  O(d) where d = number of digits
 * Space Complexity: O(1) (fixed factorial array)
 *
 * Common Interview Questions:
 *   Q: How many strong numbers exist?
 *   A: Only 4 known: 1, 2, 145, 40585.
 *
 *   Q: Why precompute factorials?
 *   A: Factorial of 9! = 362880, computing it repeatedly is wasteful.
 *      Digit factorials are bounded and known ahead of time.
 *
 * Common Mistakes:
 *   - Not precomputing factorials (inefficient)
 *   - Forgetting 0! = 1 (but 0 is not a strong number)
 *   - Integer overflow when computing factorials dynamically
 * ====================================================================
 */

#include <iostream>
#include <array>

// --- Precompute factorials for digits 0-9 ---
constexpr std::array<unsigned long long, 10> precomputeFactorials()
{
    std::array<unsigned long long, 10> fact{};
    fact[0] = 1;
    for (int i{1}; i < 10; ++i)
    {
        fact[i] = fact[i - 1] * static_cast<unsigned long long>(i);
    }
    return fact;
}

constinit static const std::array<unsigned long long, 10> factorials = precomputeFactorials();

// --- Check strong number ---
bool isStrong(int n)
{
    if (n < 0) return false;

    int original = n;
    unsigned long long sum{};

    while (n > 0)
    {
        int digit = n % 10;
        sum += factorials[static_cast<std::size_t>(digit)];
        n /= 10;
    }

    return sum == static_cast<unsigned long long>(original);
}

int main()
{
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    if (isStrong(num))
        std::cout << num << " is a strong number.\n";
    else
        std::cout << num << " is NOT a strong number.\n";

    // --- Generate strong numbers up to 100000 ---
    int limit{100000};
    std::cout << "\nStrong numbers between 1 and " << limit << ":\n";
    for (int i{1}; i <= limit; ++i)
    {
        if (isStrong(i)) std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
