/*
 * ====================================================================
 * Program 34: Nested Loops
 * Theory:
 *   A nested loop is a loop inside another loop. For each iteration
 *   of the outer loop, the inner loop runs completely. Total iterations
 *   = outer_count × inner_count.
 *
 *   Common uses:
 *   - Multiplication tables
 *   - Matrix operations (traversal, addition, multiplication)
 *   - Pattern printing
 *   - 2D array processing
 *
 * Sample Input/Output:
 *   Multiplication Table (1 to 5):
 *         1   2   3   4   5
 *     1   1   2   3   4   5
 *     2   2   4   6   8  10
 *     3   3   6   9  12  15
 *     4   4   8  12  16  20
 *     5   5  10  15  20  25
 *
 * Time Complexity:  O(rows × cols) for matrix ops
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: How to optimize nested loops?
 *   A: Reduce work inside inner loops, move invariant calculations
 *      outside, loop interchange for cache-friendly access.
 *
 *   Q: What is the complexity of nested loops?
 *   A: If outer runs N and inner runs M, complexity is O(N×M).
 *      For triangular loops (j=i+1), it's O(N²/2) = O(N²).
 *
 * Common Mistakes:
 *   - Using same loop variable name in inner and outer loops
 *   - Wrong loop bounds (off-by-one)
 *   - Not resetting inner loop variable correctly
 * ====================================================================
 */

#include <iostream>
#include <iomanip>

int main()
{
    // --- Multiplication table ---
    int n{5};
    std::cout << "Multiplication Table (1 to " << n << "):\n";

    // Print header
    std::cout << std::setw(4) << "";
    for (int j{1}; j <= n; ++j)
        std::cout << std::setw(4) << j;
    std::cout << '\n';

    // Print rows
    for (int i{1}; i <= n; ++i)
    {
        std::cout << std::setw(4) << i;
        for (int j{1}; j <= n; ++j)
        {
            std::cout << std::setw(4) << (i * j);
        }
        std::cout << '\n';
    }

    // --- Matrix addition ---
    constexpr int rows{3}, cols{3};
    int mat1[rows][cols] = {{1,2,3},{4,5,6},{7,8,9}};
    int mat2[rows][cols] = {{9,8,7},{6,5,4},{3,2,1}};
    int result[rows][cols]{};

    for (int i{}; i < rows; ++i)
    {
        for (int j{}; j < cols; ++j)
        {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }

    std::cout << "\nMatrix addition result:\n";
    for (int i{}; i < rows; ++i)
    {
        for (int j{}; j < cols; ++j)
        {
            std::cout << std::setw(3) << result[i][j];
        }
        std::cout << '\n';
    }

    return 0;
}
