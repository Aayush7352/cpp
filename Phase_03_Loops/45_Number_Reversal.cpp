/*
 * ====================================================================
 * Program 45: Number Reversal
 * Theory:
 *   Reversing the digits of a number with mathematics:
 *   - Extract last digit: digit = n % 10
 *   - Append to reversed: rev = rev * 10 + digit
 *   - Remove last digit: n /= 10
 *   - Repeat until n == 0
 *
 *   Combined with palindrome check by comparing original vs reversed.
 *
 * Sample Input/Output:
 *   Enter a number: 12345
 *   Original: 12345
 *   Reversed: 54321
 *   Not a palindrome.
 *
 *   Enter a number: 12321
 *   Original: 12321
 *   Reversed: 12321
 *   Is a palindrome!
 *
 *   Enter a number: 1000
 *   Original: 1000
 *   Reversed: 1 (leading zeros dropped)
 *
 * Time Complexity:  O(d) where d = number of digits
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: What about leading zeros in reversed number?
 *   A: Mathematical reversal naturally drops leading zeros.
 *      1000 reversed = 1, not 0001. This is mathematically correct.
 *
 *   Q: How to reverse a number as a string?
 *   A: Convert to string, use std::reverse, then convert back.
 *      This preserves trailing zeros if needed.
 *
 * Common Mistakes:
 *   - Overflow when reversing large numbers (use long long)
 *   - Not saving original value before modification
 *   - Forgetting that negative numbers need sign handling
 * ====================================================================
 */

#include <iostream>
#include <string>
#include <algorithm>

// --- Reverse number mathematically ---
long long reverseNumber(int n)
{
    long long reversed{};
    int num = std::abs(n);

    while (num > 0)
    {
        reversed = reversed * 10 + (num % 10);
        num /= 10;
    }

    return (n < 0) ? -reversed : reversed;
}

// --- Reverse number using strings (preserves trailing zeros context) ---
std::string reverseString(int n)
{
    std::string s = std::to_string(n);
    std::reverse(s.begin(), s.end());
    return s;
}

int main()
{
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    long long rev = reverseNumber(num);
    std::cout << "Original: " << num << '\n';
    std::cout << "Reversed: " << rev << '\n';

    if (rev == static_cast<long long>(num))
        std::cout << "Is a palindrome!\n";
    else
        std::cout << "Not a palindrome.\n";

    // --- String-based reversal ---
    std::string strRev = reverseString(num);
    std::cout << "Reversed (string): " << strRev << '\n';

    return 0;
}
