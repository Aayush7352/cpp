/*
 * ====================================================================
 * Program 27: Ternary Operator (?:)
 * Theory:
 *   The ternary operator is a shorthand for simple if-else expressions.
 *   Syntax: condition ? expression_true : expression_false
 *
 *   It's an expression (returns a value), not a statement. Can be used
 *   in initializations, return statements, and inline inside larger
 *   expressions.
 *
 *   Nested ternaries: ternary inside ternary — use sparingly as they
 *   hurt readability. Prefer if-else for complex conditions.
 *
 *   Performance: Modern compilers generate identical code for ternary
 *   and equivalent if-else. No performance difference. Choose based on
 *   readability.
 *
 * Sample Input/Output:
 *   Enter a number: -5
 *   Absolute value: 5
 *   The number is negative.
 *
 *   Enter three numbers: 10 25 7
 *   Largest: 25
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Is ternary faster than if-else?
 *   A: No. Modern compilers optimize both to identical assembly.
 *
 *   Q: Can ternary return different types?
 *   A: Both branches must be the same type or implicitly convertible
 *      to a common type.
 *
 * Common Mistakes:
 *   - Nesting ternaries too deeply (readability nightmare)
 *   - Using ternary with side effects (e.g., multiple statements)
 *   - Forgetting parentheses in complex expressions
 * ====================================================================
 */

#include <iostream>

int main()
{
    // --- Basic ternary: absolute value ---
    int num{};
    std::cout << "Enter a number: ";
    std::cin >> num;

    int absVal = (num < 0) ? -num : num;
    std::cout << "Absolute value: " << absVal << '\n';

    // --- Ternary in output ---
    std::cout << "The number is "
              << ((num % 2 == 0) ? "even" : "odd") << ".\n";

    // --- Nested ternary: largest of three ---
    int a{}, b{}, c{};
    std::cout << "Enter three numbers: ";
    std::cin >> a >> b >> c;

    int largest = (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
    std::cout << "Largest: " << largest << '\n';

    // --- Equivalent if-else for comparison ---
    int largest2{};
    if (a > b)
    {
        largest2 = (a > c) ? a : c;
    }
    else
    {
        largest2 = (b > c) ? b : c;
    }
    std::cout << "Largest (if-else): " << largest2 << '\n';

    return 0;
}
