/*
 * ====================================================================
 * Program 21: if Statement
 * Theory:
 *   The 'if' statement is the most basic control flow construct in C++.
 *   It evaluates a condition (typically a relational or logical expression)
 *   and executes the associated block only if the condition is truthy.
 *
 *   Relational operators: ==, !=, <, >, <=, >=
 *   In C++, any non-zero value is considered true; zero is false.
 *   The condition can be any expression that converts to bool.
 *
 *   Dangling Else Problem:
 *     Occurs when nested if statements are paired with else clauses.
 *     C++ uses the "closest unmatched if" rule: an else binds to the
 *     nearest preceding if that does not already have an else.
 *     Always use braces {} to avoid ambiguity, even with single statements.
 *
 * Sample Input/Output:
 *   Enter age: 20
 *   You are eligible to vote.
 *
 *   Enter age: -5
 *   Invalid age entered.
 *
 *   --- Dangling else demo ---
 *   Input: x=10, y=5
 *   Output: (no output — else binds to inner if)
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: What is the dangling else problem?
 *   A: It's an ambiguity where an else can be matched to multiple ifs.
 *      C++ resolves it by binding else to the nearest unmatched if.
 *
 *   Q: Can the condition be an assignment?
 *   A: Yes, but it's often a bug. E.g., if (x = 5) assigns 5 and is
 *      always true. Use if (x == 5) for comparison. Compilers warn.
 *
 * Common Mistakes:
 *   - Using = instead of == in conditions
 *   - Forgetting braces and creating dangling else bugs
 *   - Using floating-point equality checks (imprecise)
 *   - Semicolon after if(...) — the block becomes empty
 * ====================================================================
 */

#include <iostream>

int main()
{
    // --- Basic if with relational expressions ---
    int age{};
    std::cout << "Enter age: ";
    std::cin >> age;

    if (age >= 18)
    {
        std::cout << "You are eligible to vote.\n";
    }

    if (age < 0)
    {
        std::cout << "Invalid age entered.\n";
    }

    // --- Dangling Else Demonstration ---
    std::cout << "\n--- Dangling else demo ---\n";
    int x{10}, y{5};

    // WRONG: ambiguous indentation — else binds to inner if
    if (x > 5)
        if (y > 10)
            std::cout << "x > 5 and y > 10\n";
    else  // This else belongs to `if (y > 10)`, NOT `if (x > 5)`
        std::cout << "x <= 5 (misleading!)\n";

    // CORRECT: use braces to clarify intent
    if (x > 5)
    {
        if (y > 10)
        {
            std::cout << "x > 5 and y > 10\n";
        }
    }
    else
    {
        std::cout << "x <= 5\n";
    }

    return 0;
}
