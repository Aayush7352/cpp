/*
 * ====================================================================
 * Program 35: break/continue
 * Theory:
 *   break: terminates the innermost enclosing loop or switch immediately.
 *   continue: skips the rest of the current iteration and proceeds
 *             to the next iteration of the innermost loop.
 *
 *   Labeled break (C++ goto): C++ does NOT have Java-style labeled
 *   break/continue. To break out of nested loops, use a goto statement
 *   or a flag variable. goto is acceptable for breaking nested loops
 *   (the one case where it's considered clean).
 *
 * Sample Input/Output:
 *   --- break in nested loops ---
 *   Found 7 at position [1][2]
 *
 *   --- continue ---
 *   Odd numbers: 1 3 5 7 9
 *
 *   --- goto (labeled break) ---
 *   Exited all loops at i=2, j=2
 *
 * Time Complexity:  O(n×m) worst case
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Can you break out of nested loops?
 *   A: C++ doesn't have labeled break. Use goto, a flag, or
 *      refactor into a function with return.
 *
 *   Q: Difference between break and continue?
 *   A: break exits the loop entirely; continue skips to next iteration.
 *
 * Common Mistakes:
 *   - Using break/continue outside a loop or switch
 *   - Thinking break exits multiple nested loops (it only exits one)
 *   - Using continue in switch (it works but confusing — be careful)
 * ====================================================================
 */

#include <iostream>
#include <vector>

int main()
{
    // --- break: find first occurrence ---
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int target{7};
    bool found{false};

    std::cout << "--- break in nested loops ---\n";
    for (std::size_t i{}; i < matrix.size(); ++i)
    {
        for (std::size_t j{}; j < matrix[i].size(); ++j)
        {
            if (matrix[i][j] == target)
            {
                std::cout << "Found " << target << " at position [" << i << "][" << j << "]\n";
                found = true;
                break;  // breaks inner loop only
            }
        }
        if (found) break;  // break outer loop
    }

    // --- continue: skip even numbers ---
    std::cout << "\n--- continue ---\n";
    std::cout << "Odd numbers: ";
    for (int i{1}; i <= 10; ++i)
    {
        if (i % 2 == 0) continue;
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // --- goto: "labeled break" for nested loops ---
    std::cout << "\n--- goto (labeled break) ---\n";
    for (int i{}; i < 5; ++i)
    {
        for (int j{}; j < 5; ++j)
        {
            if (i == 2 && j == 2)
            {
                std::cout << "Exited all loops at i=" << i << ", j=" << j << '\n';
                goto exit_all;
            }
        }
    }
exit_all:
    // continue here after goto

    return 0;
}
