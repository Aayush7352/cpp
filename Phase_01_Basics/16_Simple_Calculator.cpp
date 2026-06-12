/*
 * ====================================================================
 * Program 16: Simple Calculator
 *
 * THEORY:
 * A practical program combining:
 *   - Input/output (cin, cout)
 *   - Arithmetic operators (+, -, *, /, %)
 *   - Control flow (switch, if-else)
 *   - Loops (do-while for menu)
 *   - Error handling (division by zero, invalid input)
 *
 * SAMPLE INPUT/OUTPUT:
 *   === Simple Calculator ===
 *   1. Add
 *   2. Subtract
 *   3. Multiply
 *   4. Divide
 *   5. Modulus
 *   6. Exit
 *   Choose operation (1-6): 1
 *   Enter two numbers: 10 5
 *   Result: 10 + 5 = 15
 *
 * TIME COMPLEXITY:   O(1) per operation
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: How would you handle division by zero?
 *   A: Check if the divisor is 0 before performing the operation.
 *   Q: Switch vs if-else — when to use which?
 *   A: Switch for discrete values (like menu options); if-else for
 *      ranges and complex conditions.
 *   Q: Why use do-while vs while?
 *   A: do-while guarantees at least one iteration — perfect for
 *      menu-driven programs.
 *
 * COMMON MISTAKES:
 *   - Integer division truncating decimal results.
 *   - Not checking for division by zero.
 *   - Not handling invalid input (cin.fail()).
 *   - Falling through switch cases without break.
 * ====================================================================
 */

#include <iostream>
#include <limits>

int main()
{
    double a{}, b{}, result{};
    char op{};
    bool keep_running = true;

    std::cout << "=== Simple Calculator ===\n";
    std::cout << "Supported: +  -  *  /  %\n";
    std::cout << "Type 'q' as operator to quit.\n\n";

    while (keep_running)
    {
        // ---- Input ----
        std::cout << "Enter expression (a op b): ";
        std::cin >> a >> op >> b;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n\n";
            continue;
        }

        // ---- Operator handling ----
        switch (op)
        {
        case '+':
            result = a + b;
            std::cout << a << " + " << b << " = " << result << '\n';
            break;

        case '-':
            result = a - b;
            std::cout << a << " - " << b << " = " << result << '\n';
            break;

        case '*':
            result = a * b;
            std::cout << a << " * " << b << " = " << result << '\n';
            break;

        case '/':
            if (b == 0.0)
            {
                std::cout << "Error: Division by zero!\n";
            }
            else
            {
                result = a / b;
                std::cout << a << " / " << b << " = " << result << '\n';
            }
            break;

        case '%':
        {
            // % only works on integers
            int ia = static_cast<int>(a);
            int ib = static_cast<int>(b);
            if (ib == 0)
            {
                std::cout << "Error: Modulus by zero!\n";
            }
            else
            {
                std::cout << ia << " % " << ib << " = " << (ia % ib) << '\n';
            }
            break;
        }

        case 'q':
        case 'Q':
            keep_running = false;
            break;

        default:
            std::cout << "Unknown operator '" << op << "'\n";
            break;
        }
        std::cout << '\n';
    }

    std::cout << "Goodbye!\n";
    return 0;
}
