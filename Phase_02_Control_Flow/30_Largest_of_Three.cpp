/*
 * ====================================================================
 * Program 30: Largest of Three Numbers
 * Theory:
 *   Finding the maximum of three numbers is a classic conditional
 *   exercise. Approaches:
 *     1. if-else chain: compare a vs b, then the larger vs c
 *     2. Nested if-else: if(a>b) { if(a>c) ... }
 *     3. Ternary operator chaining
 *     4. std::max (C++ algorithm library)
 *
 * Sample Input/Output:
 *   Enter three numbers: 10 25 7
 *   Largest (if-else): 25
 *   Largest (ternary): 25
 *   Largest (std::max): 25
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: How would you find the largest without using if?
 *   A: Use std::max or a ternary expression.
 *
 *   Q: Extend to find largest of N numbers?
 *   A: Iterate through the array, keep a running max variable.
 *
 * Common Mistakes:
 *   - Not handling equal numbers (still works, any equal value is max)
 *   - Overcomplicating with unnecessary nesting
 *   - Using > instead of >= (edge case: all equal)
 * ====================================================================
 */

#include <iostream>
#include <algorithm>  // std::max

int main()
{
    int a{}, b{}, c{};
    std::cout << "Enter three numbers: ";
    std::cin >> a >> b >> c;

    // --- Method 1: if-else chain ---
    int max1 = a;
    if (b > max1) max1 = b;
    if (c > max1) max1 = c;
    std::cout << "Largest (if-else): " << max1 << '\n';

    // --- Method 2: Nested if-else ---
    int max2{};
    if (a >= b && a >= c)
    {
        max2 = a;
    }
    else if (b >= a && b >= c)
    {
        max2 = b;
    }
    else
    {
        max2 = c;
    }
    std::cout << "Largest (nested): " << max2 << '\n';

    // --- Method 3: Ternary chaining ---
    int max3 = (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
    std::cout << "Largest (ternary): " << max3 << '\n';

    // --- Method 4: std::max (C++ standard library) ---
    int max4 = std::max({a, b, c});
    std::cout << "Largest (std::max): " << max4 << '\n';

    return 0;
}
