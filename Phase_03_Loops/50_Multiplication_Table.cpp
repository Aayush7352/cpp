/*
 * ====================================================================
 * Program 50: Multiplication Table
 * Theory:
 *   Print multiplication tables up to N using nested loops.
 *   The outer loop iterates through each table (1 to N), and the
 *   inner loop prints the products for that table.
 *
 *   Formatting uses std::setw for aligned columns.
 *
 * Sample Input/Output:
 *   Enter N (1-20): 5
 *
 *   Multiplication Table (1 to 5):
 *   ============================
 *   1 x 1 = 1
 *   1 x 2 = 2
 *   1 x 3 = 3
 *   1 x 4 = 4
 *   1 x 5 = 5
 *   ============================
 *   2 x 1 = 2
 *   2 x 2 = 4
 *   2 x 3 = 6
 *   2 x 4 = 8
 *   2 x 5 = 10
 *   ...
 *   5 x 5 = 25
 *
 * Time Complexity:  O(N²)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Can you print a single table without nested loops?
 *   A: Yes, the inner loop alone produces a single table.
 *      Nested loops produce multiple tables.
 *
 *   Q: How would you print a formatted grid (matrix style)?
 *   A: Use nested loops with std::setw for alignment as shown
 *      in Program 34.
 *
 * Common Mistakes:
 *   - Using same loop variable name for both loops
 *   - Off-by-one: using < N instead of <= N or vice versa
 *   - Not resetting the table format between tables
 * ====================================================================
 */

#include <iostream>
#include <iomanip>

int main()
{
    int n{};
    std::cout << "Enter N (1-20): ";
    std::cin >> n;

    if (n < 1 || n > 20)
    {
        std::cout << "N must be between 1 and 20.\n";
        return 1;
    }

    std::cout << "\nMultiplication Table (1 to " << n << "):\n";
    std::cout << std::string(28, '=') << '\n';

    for (int i{1}; i <= n; ++i)
    {
        for (int j{1}; j <= n; ++j)
        {
            std::cout << i << " x " << std::setw(2) << j
                      << " = " << std::setw(2) << (i * j) << '\n';
        }
        std::cout << std::string(28, '=') << '\n';
    }

    // --- Single table version ---
    std::cout << "\nSingle table for " << n << ":\n";
    for (int i{1}; i <= 10; ++i)
    {
        std::cout << n << " x " << std::setw(2) << i
                  << " = " << std::setw(3) << (n * i) << '\n';
    }

    return 0;
}
