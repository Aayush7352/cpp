/*
 * ====================================================================
 * Program 40: Palindrome Number
 * Theory:
 *   A palindrome number reads the same forwards and backwards.
 *   E.g., 121, 12321, 1221, 0.
 *
 *   Approach: Reverse the number mathematically and compare to original.
 *   - Extract digits using % 10
 *   - Build reversed: rev = rev * 10 + digit
 *   - Continue until n == 0
 *   - Compare reversed number with original
 *
 *   Edge case: Negative numbers are NOT palindromes (minus sign breaks it).
 *   Edge case: Numbers ending in 0 are not palindromes except 0 itself
 *              (e.g., 10 reversed = 01 = 1 ≠ 10).
 *
 * Sample Input/Output:
 *   Enter a number: 12321
 *   12321 is a palindrome.
 *
 *   Enter a number: 12345
 *   12345 is NOT a palindrome (reversed: 54321).
 *
 *   Enter a number: -121
 *   -121 is NOT a palindrome (negative numbers).
 *
 * Time Complexity:  O(d) where d = number of digits
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Can you check palindrome without converting to string?
 *   A: Yes, by reversing the number mathematically as shown.
 *
 *   Q: Can you check without reversing the full number?
 *   A: Yes, reverse only half and compare. When rev ≥ remaining,
 *      stop and compare (avoids overflow for large numbers).
 *
 * Common Mistakes:
 *   - Not handling negative numbers
 *   - Integer overflow during reversal (for large numbers)
 *   - Not saving the original value before modifying it
 * ====================================================================
 */

#include <iostream>

// --- Palindrome check by full reversal ---
bool isPalindrome(int n)
{
    // Negative numbers are not palindromes
    if (n < 0) return false;

    // Numbers ending with 0 (except 0 itself) are not palindromes
    if (n % 10 == 0 && n != 0) return false;

    int original = n;
    long long reversed{};  // use long long to avoid overflow

    while (n > 0)
    {
        int digit = n % 10;
        reversed = reversed * 10 + digit;
        n /= 10;
    }

    return reversed == static_cast<long long>(original);
}

// --- Palindrome check by half reversal (avoids overflow) ---
bool isPalindromeHalf(int n)
{
    if (n < 0) return false;
    if (n % 10 == 0 && n != 0) return false;

    int reversed{};
    while (n > reversed)
    {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }

    // For even length: n == reversed
    // For odd length:  n == reversed / 10 (middle digit discarded)
    return n == reversed || n == reversed / 10;
}

int main()
{
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    if (isPalindrome(num))
        std::cout << num << " is a palindrome.\n";
    else
        std::cout << num << " is NOT a palindrome (negative numbers).\n";

    // --- Show reversal ---
    int n = std::abs(num);
    long long rev{};
    int tmp = n;
    while (tmp > 0)
    {
        rev = rev * 10 + tmp % 10;
        tmp /= 10;
    }

    if (num >= 0 && !isPalindrome(num))
    {
        std::cout << "Reversed: " << rev << '\n';
    }

    return 0;
}
