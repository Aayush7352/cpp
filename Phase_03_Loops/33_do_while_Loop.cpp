/*
 * ====================================================================
 * Program 33: do-while Loop
 * Theory:
 *   The do-while loop is a post-test loop: the body executes at least
 *   once, then the condition is checked. This makes it ideal for menus
 *   and input validation where one iteration is required.
 *
 *   Syntax: do { body } while (condition);
 *   Note the semicolon after while().
 *
 * Sample Input/Output:
 *   --- Menu pattern ---
 *   1. Option A
 *   2. Option B
 *   3. Exit
 *   Choice: 1
 *   You chose Option A.
 *   ...
 *   Choice: 3
 *   Goodbye!
 *
 *   --- Input validation ---
 *   Enter a positive number: -3
 *   Invalid! Try again.
 *   Enter a positive number: 7
 *   You entered: 7
 *
 * Time Complexity:  O(n) for validation loops
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: What is the main difference between while and do-while?
 *   A: do-while guarantees at least one execution; while may execute
 *      zero times.
 *
 *   Q: Where is do-while commonly used?
 *   A: Menus, input validation, and any scenario where the loop body
 *      must run before a condition can be meaningfully evaluated.
 *
 * Common Mistakes:
 *   - Forgetting the semicolon after while(condition);
 *   - Using do-while when while is more appropriate (body should run
 *     zero times for some inputs)
 * ====================================================================
 */

#include <iostream>
#include <limits>

int main()
{
    // --- Input validation: ensure positive number ---
    int num{};
    do
    {
        std::cout << "Enter a positive number: ";
        std::cin >> num;
        if (num <= 0)
        {
            std::cout << "Invalid! Try again.\n";
        }
    } while (num <= 0);

    std::cout << "You entered: " << num << '\n';

    // --- Menu pattern ---
    int choice{};
    do
    {
        std::cout << "\n1. Option A\n";
        std::cout << "2. Option B\n";
        std::cout << "3. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                std::cout << "You chose Option A.\n";
                break;
            case 2:
                std::cout << "You chose Option B.\n";
                break;
            case 3:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    } while (choice != 3);

    return 0;
}
