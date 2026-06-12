/*
 * ====================================================================
 * Program 25: Switch-Case
 * Theory:
 *   switch evaluates an integral/enum expression and jumps to the
 *   matching case label. It's more efficient than else-if for
 *   discrete value matching because compilers can use jump tables.
 *
 *   - enum class: Strongly-typed scoped enum. Requires explicit
 *     casting or static_cast<int> for switch (C++ allows switch
 *     directly on enum class).
 *   - Fall-through: If a case lacks a break, execution "falls through"
 *     to the next case. This is sometimes intentional but often a bug.
 *     Use [[fallthrough]] attribute (C++17) to document intentional
 *     fall-through.
 *   - default case: Handles all values not covered by explicit cases.
 *     Always include it, even if empty, for robustness.
 *
 * Sample Input/Output:
 *   Enter day number (1-7): 3
 *   Wednesday
 *
 *   Enter day number (1-7): 8
 *   Invalid day number!
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Can switch work with strings?
 *   A: No, not directly. Use if-else or std::map<std::string, int>.
 *
 *   Q: What types can switch use?
 *   A: char, short, int, long, long long, enum, enum class, and
 *      their unsigned variants. C++17 also allows constexpr if.
 *
 *   Q: How is switch compiled?
 *   A: Often with a jump table (O(1)), a binary search (O(log n)),
 *      or a conditional chain (O(n)) depending on density.
 *
 * Common Mistakes:
 *   - Forgetting break — unintentional fall-through
 *   - Declaring variables inside a case without braces
 *   - Omitting default case
 * ====================================================================
 */

#include <iostream>

int main()
{
    int day{};
    std::cout << "Enter day number (1-7): ";
    std::cin >> day;

    switch (day)
    {
        case 1: std::cout << "Monday\n";    break;
        case 2: std::cout << "Tuesday\n";   break;
        case 3: std::cout << "Wednesday\n"; break;
        case 4: std::cout << "Thursday\n";  break;
        case 5: std::cout << "Friday\n";    break;
        case 6: std::cout << "Saturday\n";  break;
        case 7: std::cout << "Sunday\n";    break;
        default:
            std::cout << "Invalid day number!\n";
            break;
    }

    // --- enum class with switch ---
    enum class Color { Red, Green, Blue };
    Color c{Color::Green};

    switch (c)
    {
        case Color::Red:   std::cout << "Red\n";   break;
        case Color::Green: std::cout << "Green\n"; break;
        case Color::Blue:  std::cout << "Blue\n";  break;
    }

    // --- Intentional fall-through with [[fallthrough]] ---
    int x{2};
    switch (x)
    {
        case 1:
            std::cout << "One\n";
            [[fallthrough]];
        case 2:
            std::cout << "Two\n";
            [[fallthrough]];
        case 3:
            std::cout << "Three\n";
            break;
        default:
            break;
    }

    return 0;
}
