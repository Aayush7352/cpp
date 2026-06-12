/*
 * ====================================================================
 * Program 43: GCD (Greatest Common Divisor)
 * Theory:
 *   GCD of two numbers is the largest positive integer dividing both.
 *
 *   Euclidean Algorithm:
 *     gcd(a, b) = gcd(b, a % b)
 *     Base case: gcd(a, 0) = a
 *
 *   Approaches:
 *   - Iterative (Euclidean)
 *   - Recursive (Euclidean)
 *   - std::gcd (C++17) from <numeric>
 *
 * Sample Input/Output:
 *   Enter two numbers: 56 98
 *   GCD of 56 and 98 is 14
 *   GCD (recursive): 14
 *   GCD (std::gcd): 14
 *
 * Time Complexity:  O(log min(a, b))
 * Space Complexity:
 *   Iterative: O(1)
 *   Recursive: O(log min(a, b)) (call stack)
 *
 * Common Interview Questions:
 *   Q: What is the time complexity of Euclidean algorithm?
 *   A: O(log min(a, b)) — the number of steps is proportional to
 *      the number of digits (Lamé's theorem).
 *
 *   Q: What if numbers are negative?
 *   A: GCD is defined as positive. std::gcd returns the absolute
 *      value. Our implementation should also handle negatives.
 *
 * Common Mistakes:
 *   - Not handling negative numbers (use abs)
 *   - Infinite recursion without base case
 *   - Using subtraction instead of modulo (slower)
 * ====================================================================
 */

#include <iostream>
#include <numeric>  // std::gcd (C++17)
#include <cstdlib>  // std::abs

// --- Iterative GCD ---
int gcdIterative(int a, int b)
{
    a = std::abs(a);
    b = std::abs(b);

    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// --- Recursive GCD ---
int gcdRecursive(int a, int b)
{
    a = std::abs(a);
    b = std::abs(b);

    if (b == 0) return a;
    return gcdRecursive(b, a % b);
}

int main()
{
    int a{}, b{};
    std::cout << "Enter two numbers: ";
    std::cin >> a >> b;

    int g1 = gcdIterative(a, b);
    int g2 = gcdRecursive(a, b);
    int g3 = std::gcd(a, b);

    std::cout << "GCD of " << a << " and " << b << " is " << g1 << '\n';
    std::cout << "GCD (recursive): " << g2 << '\n';
    std::cout << "GCD (std::gcd): " << g3 << '\n';

    return 0;
}
