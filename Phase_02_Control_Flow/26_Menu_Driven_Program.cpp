/*
 * ====================================================================
 * Program 26: Menu-Driven Program (Calculator)
 * Theory:
 *   A menu-driven program presents a list of options to the user and
 *   executes corresponding actions. Combining do-while (guarantees at
 *   least one iteration) with switch-case creates a clean interaction
 *   loop that continues until the user chooses to exit.
 *
 *   do-while is ideal for menus because the menu must display at least
 *   once before the exit condition is checked.
 *
 * Sample Input/Output:
 *   === Calculator Menu ===
 *   1. Add
 *   2. Subtract
 *   3. Multiply
 *   4. Divide
 *   5. Exit
 *   Choose an option: 1
 *   Enter two numbers: 10 5
 *   Result: 15
 *
 *   Choose an option: 4
 *   Enter two numbers: 10 3
 *   Result: 3.33333
 *
 *   Choose an option: 5
 *   Goodbye!
 *
 * Time Complexity:  O(1) per operation
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Why do-while instead of while for menus?
 *   A: A menu must display at least once. do-while guarantees one
 *      iteration, while while may skip if the condition is initially
 *      false.
 *
 *   Q: How to handle invalid menu input?
 *   A: Validate input with if or default in switch. Clear cin flags
 *      and ignore bad input to prevent infinite loops.
 *
 * Common Mistakes:
 *   - Using while instead of do-while (menu never shows)
 *   - Not clearing cin after invalid input leading to infinite loop
 *   - Division by zero not handled
 * ====================================================================
 */

#include <iostream>
#include <limits>

int main()
{
    int choice{};
    double a{}, b{};

    do
    {
        std::cout << "\n=== Calculator Menu ===\n";
        std::cout << "1. Add\n";
        std::cout << "2. Subtract\n";
        std::cout << "3. Multiply\n";
        std::cout << "4. Divide\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice >= 1 && choice <= 4)
        {
            std::cout << "Enter two numbers: ";
            std::cin >> a >> b;
        }

        switch (choice)
        {
            case 1:
                std::cout << "Result: " << (a + b) << '\n';
                break;
            case 2:
                std::cout << "Result: " << (a - b) << '\n';
                break;
            case 3:
                std::cout << "Result: " << (a * b) << '\n';
                break;
            case 4:
                if (b != 0.0)
                    std::cout << "Result: " << (a / b) << '\n';
                else
                    std::cout << "Error: Division by zero!\n";
                break;
            case 5:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    } while (choice != 5);

    return 0;
}
