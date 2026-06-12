/*
 * ====================================================================
 * Program 22: if-else Statement
 * Theory:
 *   The if-else construct provides two-way decision making.
 *   If the condition is true, the if-block executes; otherwise,
 *   the else-block executes. Exactly one branch always runs.
 *
 *   This is useful for binary decisions: pass/fail, even/odd,
 *   positive/negative, adult/minor, etc.
 *
 * Sample Input/Output:
 *   Enter a number: 7
 *   7 is odd.
 *
 *   Enter a number: 12
 *   12 is even.
 *
 *   Enter marks: 85
 *   Status: PASS
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Can an if-else chain replace a switch?
 *   A: Yes, but switch is often more readable for discrete values.
 *      if-else is better for ranges and complex conditions.
 *
 *   Q: What is the advantage of early return vs if-else?
 *   A: Early return reduces nesting and simplifies control flow,
 *      especially in input validation (guard clauses).
 *
 * Common Mistakes:
 *   - Writing else if as "elseif" instead of "else if"
 *   - Placing a semicolon after condition: if(x > 5);  // empty body
 *   - Forgetting that else attaches to the nearest if
 * ====================================================================
 */

#include <iostream>

int main()
{
    // --- Even / Odd check ---
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    if (num % 2 == 0)
    {
        std::cout << num << " is even.\n";
    }
    else
    {
        std::cout << num << " is odd.\n";
    }

    // --- Pass / Fail based on marks ---
    int marks{};
    std::cout << "Enter marks: ";
    std::cin >> marks;

    if (marks >= 40)
        std::cout << "Status: PASS\n";
    else
        std::cout << "Status: FAIL\n";

    return 0;
}
