/*
 * ====================================================================
 * Program 23: else-if Ladder
 * Theory:
 *   The else-if ladder is a multi-way decision construct.
 *   It evaluates conditions top-to-bottom and executes the first
 *   block whose condition is true. Once a match is found, the rest
 *   of the ladder is skipped. The optional final else acts as default.
 *
 *   Ideal for categorizing into multiple ranges (e.g., grades, BMI).
 *
 * Sample Input/Output:
 *   Enter marks (0-100): 85
 *   Grade: A
 *
 *   Enter marks (0-100): 72
 *   Grade: B
 *
 *   Enter marks (0-100): -5
 *   Invalid marks entered.
 *
 * Time Complexity:  O(1)  (at most N branches checked, N small/fixed)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: What is the difference between else-if and switch?
 *   A: else-if handles ranges and complex boolean conditions;
 *      switch handles exact discrete values (integers/enums).
 *
 *   Q: Is an else-if ladder always O(n) comparisons?
 *   A: In source code yes, but the compiler may optimize it into a
 *      jump table or binary search for switch-like patterns.
 *
 * Common Mistakes:
 *   - Not handling edge cases (negative marks, marks > 100)
 *   - Overlapping conditions (e.g., if(x>50) ... else if(x>70))
 *     — the second condition will never be reached
 *   - Forgetting the final else (no default case)
 * ====================================================================
 */

#include <iostream>

int main()
{
    int marks{};
    std::cout << "Enter marks (0-100): ";
    std::cin >> marks;

    if (marks < 0 || marks > 100)
    {
        std::cout << "Invalid marks entered.\n";
    }
    else if (marks >= 90)
    {
        std::cout << "Grade: A\n";
    }
    else if (marks >= 75)
    {
        std::cout << "Grade: B\n";
    }
    else if (marks >= 60)
    {
        std::cout << "Grade: C\n";
    }
    else if (marks >= 40)
    {
        std::cout << "Grade: D\n";
    }
    else
    {
        std::cout << "Grade: F\n";
    }

    return 0;
}
