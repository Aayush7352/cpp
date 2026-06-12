/*
 * ====================================================================
 * Program 39: Armstrong Number (Narcissistic Number)
 * Theory:
 *   An Armstrong number (also called narcissistic or pluperfect) is a
 *   number that equals the sum of its own digits each raised to the
 *   power of the number of digits.
 *
 *   Example: 153 = 1³ + 5³ + 3³ = 1 + 125 + 27 = 153
 *   1634 = 1⁴ + 6⁴ + 3⁴ + 4⁴ = 1 + 1296 + 81 + 256 = 1634
 *
 *   Three-digit Armstrong numbers: 153, 370, 371, 407
 *
 * Sample Input/Output:
 *   Enter a number: 153
 *   153 is an Armstrong number.
 *   1^3 + 5^3 + 3^3 = 1 + 125 + 27 = 153
 *
 *   Enter a number: 123
 *   123 is NOT an Armstrong number.
 *
 *   Armstrong numbers between 1 and 10000:
 *   1 2 3 4 5 6 7 8 9 153 370 371 407 1634 8208 9474
 *
 * Time Complexity:
 *   Check one number: O(d) where d = number of digits
 *   Generate up to N: O(N × log₁₀ N)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: How many Armstrong numbers are there?
 *   A: Only 88 in base 10, with the largest being 39 digits long.
 *
 *   Q: What is the algorithm?
 *   A: Count digits, then sum each digit raised to that power, compare.
 *
 * Common Mistakes:
 *   - Using pow() from <cmath> (floating-point rounding errors)
 *   - Not saving the original number before modifying it
 *   - Forgetting that single-digit numbers (0-9) are Armstrong numbers
 * ====================================================================
 */

#include <iostream>
#include <cmath>

// --- Check if a number is Armstrong ---
bool isArmstrong(int n)
{
    if (n < 0) return false;

    int original = n;
    int numDigits = static_cast<int>(std::log10(static_cast<double>(n))) + 1;
    if (n == 0) numDigits = 1;

    unsigned long long sum{};

    while (n > 0)
    {
        int digit = n % 10;

        // Manual power to avoid floating-point issues
        unsigned long long power{1};
        for (int i{}; i < numDigits; ++i)
        {
            power *= static_cast<unsigned long long>(digit);
        }

        sum += power;
        n /= 10;
    }

    return sum == static_cast<unsigned long long>(original);
}

int main()
{
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    if (isArmstrong(num))
    {
        std::cout << num << " is an Armstrong number.\n";
    }
    else
    {
        std::cout << num << " is NOT an Armstrong number.\n";
    }

    // --- Generate Armstrong numbers up to 10000 ---
    int limit{10000};
    std::cout << "\nArmstrong numbers between 1 and " << limit << ":\n";
    for (int i{1}; i <= limit; ++i)
    {
        if (isArmstrong(i))
        {
            std::cout << i << ' ';
        }
    }
    std::cout << '\n';

    return 0;
}
