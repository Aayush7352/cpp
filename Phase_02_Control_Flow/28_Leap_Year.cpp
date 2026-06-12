/*
 * ====================================================================
 * Program 28: Leap Year Checker
 * Theory:
 *   A year is a leap year if:
 *     1. It is divisible by 400, OR
 *     2. It is divisible by 4 AND NOT divisible by 100
 *
 *   This is the Gregorian calendar rule. Without the "not 100" rule,
 *   we would have too many leap years (the tropical year ≈ 365.2425 days).
 *
 *   Logic decomposition:
 *     if (year % 400 == 0) → leap
 *     else if (year % 100 == 0) → not leap
 *     else if (year % 4 == 0) → leap
 *     else → not leap
 *
 * Sample Input/Output:
 *   Enter a year: 2000
 *   2000 is a leap year. (divisible by 400)
 *
 *   Enter a year: 1900
 *   1900 is NOT a leap year. (divisible by 100 but not 400)
 *
 *   Enter a year: 2024
 *   2024 is a leap year. (divisible by 4 but not 100)
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Why is 1900 not a leap year but 2000 is?
 *   A: The rule: divisible by 400 → leap; divisible by 100 but not
 *      400 → not leap; divisible by 4 → leap.
 *
 *   Q: What about negative years (BC)?
 *   A: The Gregorian calendar proleptic extension typically applies
 *      the same rule, but historical usage varies.
 *
 * Common Mistakes:
 *   - Simplifying to year % 4 == 0 (misses century exceptions)
 *   - Wrong order of checks (must check 400 first)
 *   - Using && / || incorrectly without proper parentheses
 * ====================================================================
 */

#include <iostream>

int main()
{
    int year{};
    std::cout << "Enter a year: ";
    std::cin >> year;

    // --- Using compound condition (single expression) ---
    bool isLeap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

    if (isLeap)
    {
        std::cout << year << " is a leap year.\n";
    }
    else
    {
        std::cout << year << " is NOT a leap year.\n";
    }

    // --- Step-by-step logic (more readable) ---
    std::cout << "\n--- Step-by-step ---\n";
    if (year % 400 == 0)
    {
        std::cout << year << " is a leap year (divisible by 400).\n";
    }
    else if (year % 100 == 0)
    {
        std::cout << year << " is NOT a leap year (divisible by 100 but not 400).\n";
    }
    else if (year % 4 == 0)
    {
        std::cout << year << " is a leap year (divisible by 4).\n";
    }
    else
    {
        std::cout << year << " is NOT a leap year (not divisible by 4).\n";
    }

    return 0;
}
