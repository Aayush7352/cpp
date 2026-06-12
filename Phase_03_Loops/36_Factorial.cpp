/*
 * ====================================================================
 * Program 36: Factorial
 * Theory:
 *   n! = 1 × 2 × 3 × ... × n   (for n ≥ 0)
 *   By definition: 0! = 1
 *
 *   Approaches:
 *   - Iterative: loop from 1 to n, multiply
 *   - Recursive: n! = n × (n-1)!, base case = 1
 *
 *   Overflow: Factorial grows extremely fast (13! overflows 32-bit int,
 *   21! overflows 64-bit). Use unsigned long long and check bounds.
 *
 * Sample Input/Output:
 *   Enter n (0-20): 5
 *   Iterative: 5! = 120
 *   Recursive: 5! = 120
 *
 *   Enter n (0-20): 20
 *   20! = 2432902008176640000
 *
 * Time Complexity:
 *   Iterative: O(n)
 *   Recursive: O(n)
 * Space Complexity:
 *   Iterative: O(1)
 *   Recursive: O(n) (call stack)
 *
 * Common Interview Questions:
 *   Q: Why does factorial overflow so quickly?
 *   A: It grows super-exponentially. 20! ≈ 2.4e18, near limit of 64-bit.
 *
 *   Q: Iterative vs recursive for factorial?
 *   A: Iterative is preferred — no stack overhead, O(1) space.
 *
 * Common Mistakes:
 *   - Not handling n=0 (result should be 1)
 *   - Not checking for overflow
 *   - Using int and getting negative values from overflow
 * ====================================================================
 */

#include <iostream>
#include <limits>

// --- Iterative factorial with overflow check ---
unsigned long long factorialIterative(int n)
{
    if (n < 0) return 0;  // undefined for negative
    unsigned long long result{1};
    for (int i{2}; i <= n; ++i)
    {
        if (result > std::numeric_limits<unsigned long long>::max() / static_cast<unsigned long long>(i))
        {
            std::cerr << "Overflow at i=" << i << '\n';
            return 0;
        }
        result *= static_cast<unsigned long long>(i);
    }
    return result;
}

// --- Recursive factorial ---
unsigned long long factorialRecursive(int n)
{
    if (n < 0) return 0;
    if (n <= 1) return 1;
    return static_cast<unsigned long long>(n) * factorialRecursive(n - 1);
}

int main()
{
    int n{};
    std::cout << "Enter n (0-20): ";
    std::cin >> n;

    if (n < 0 || n > 20)
    {
        std::cout << "n must be between 0 and 20 (fits in 64-bit).\n";
        return 1;
    }

    std::cout << "Iterative: " << n << "! = " << factorialIterative(n) << '\n';
    std::cout << "Recursive: " << n << "! = " << factorialRecursive(n) << '\n';

    return 0;
}
