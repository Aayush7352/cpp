/*
 * ====================================================================
 * Program 24: Nested Conditions
 * Theory:
 *   Nested if-else means placing an if-else block inside another
 *   if-else block. This is useful when a decision depends on
 *   multiple conditions that build on each other.
 *
 *   Best Practices:
 *     1. Limit nesting depth to 3-4 levels max
 *     2. Use braces {} consistently even for single lines
 *     3. Consider early returns / guard clauses to flatten nesting
 *     4. Extract complex conditions into named boolean variables
 *     5. Prefer logical operators (&&, ||) over deep nesting when possible
 *
 * Real Example: ATM Withdrawal Validator
 *   Checks: account active → sufficient balance → daily limit → process
 *
 * Sample Input/Output:
 *   Enter account balance: 5000
 *   Enter withdrawal amount: 2000
 *   Enter daily limit: 3000
 *   Transaction approved! Collect your cash.
 *
 *   Enter account balance: 5000
 *   Enter withdrawal amount: 6000
 *   Enter daily limit: 3000
 *   Insufficient balance.
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: How deep should nesting go?
 *   A: Most style guides recommend max 3-4 levels. Deeper nesting
 *      indicates the logic should be refactored into functions.
 *
 *   Q: What are guard clauses?
 *   A: Early returns at the start of a function that handle edge
 *      cases, allowing the main logic to be flat and readable.
 *
 * Common Mistakes:
 *   - Deeply nested code that is hard to read/test
 *   - Missing braces leading to dangling else
 *   - Neglecting to handle the "everything else" case
 * ====================================================================
 */

#include <iostream>

int main()
{
    double balance{}, amount{}, dailyLimit{};

    std::cout << "Enter account balance: ";
    std::cin >> balance;
    std::cout << "Enter withdrawal amount: ";
    std::cin >> amount;
    std::cout << "Enter daily limit: ";
    std::cin >> dailyLimit;

    // --- Nested decision making ---
    if (balance > 0.0)
    {
        if (amount <= balance)
        {
            if (amount <= dailyLimit)
            {
                std::cout << "Transaction approved! Collect your cash.\n";
                balance -= amount;
            }
            else
            {
                std::cout << "Amount exceeds daily limit.\n";
            }
        }
        else
        {
            std::cout << "Insufficient balance.\n";
        }
    }
    else
    {
        std::cout << "Account has no funds.\n";
    }

    // --- Flattened version using guard clauses (for comparison) ---
    std::cout << "\n--- Flattened version (guard clauses) ---\n";
    balance = 5000; amount = 2000; dailyLimit = 3000;

    if (balance <= 0.0)
    {
        std::cout << "Account has no funds.\n";
        return 1;
    }
    if (amount > balance)
    {
        std::cout << "Insufficient balance.\n";
        return 1;
    }
    if (amount > dailyLimit)
    {
        std::cout << "Amount exceeds daily limit.\n";
        return 1;
    }

    std::cout << "Transaction approved!\n";
    balance -= amount;

    return 0;
}
