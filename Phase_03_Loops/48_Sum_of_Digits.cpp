/*
 * ====================================================================
 * Program 48: Sum of Digits
 * Theory:
 *   Sum all digits of a given number.
 *
 *   Approaches:
 *   - Iterative: Extract digits with % 10, add to sum, n /= 10
 *   - Recursive: sum(n) = n%10 + sum(n/10), base case n == 0
 *   - String-based: Convert to string, iterate characters
 *
 * Sample Input/Output:
 *   Enter a number: 12345
 *   Sum of digits (iterative): 15
 *   Sum of digits (recursive): 15
 *   Sum of digits (string): 15
 *
 * Time Complexity:  O(d) where d = number of digits
 * Space Complexity:
 *   Iterative: O(1)
 *   Recursive: O(d) (call stack)
 *   String:    O(d) (string storage)
 *
 * Common Interview Questions:
 *   Q: Can you find the digital root (repeated sum until single digit)?
 *   A: Yes: sum digits, if result >= 10, repeat. Or use: dr(n) = 1 + (n-1)%9.
 *
 *   Q: How to handle negative numbers?
 *   A: Take absolute value, or treat negative sign as separate.
 *
 * Common Mistakes:
 *   - Not handling n = 0 (sum should be 0, not getting stuck)
 *   - Modifying the original number without saving it
 *   - Off-by-one in loop termination
 * ====================================================================
 */

#include <iostream>
#include <string>
#include <cstdlib>

// --- Iterative sum of digits ---
int sumDigitsIterative(int n)
{
    int sum{};
    n = std::abs(n);
    while (n > 0)
    {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

// --- Recursive sum of digits ---
int sumDigitsRecursive(int n)
{
    if (n == 0) return 0;
    return std::abs(n % 10) + sumDigitsRecursive(n / 10);
}

// --- String-based sum of digits ---
int sumDigitsString(int n)
{
    std::string s = std::to_string(std::abs(n));
    int sum{};
    for (char c : s)
    {
        sum += c - '0';
    }
    return sum;
}

int main()
{
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    std::cout << "Sum of digits (iterative): " << sumDigitsIterative(num) << '\n';
    std::cout << "Sum of digits (recursive): " << sumDigitsRecursive(num) << '\n';
    std::cout << "Sum of digits (string):    " << sumDigitsString(num) << '\n';

    // --- Digital root demonstration ---
    int root = num;
    while (root >= 10)
    {
        root = sumDigitsIterative(root);
    }
    std::cout << "Digital root: " << root << '\n';

    return 0;
}
