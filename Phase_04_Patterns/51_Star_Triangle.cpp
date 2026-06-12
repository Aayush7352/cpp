/*
 * 51_Star_Triangle.cpp
 * ---------------
 * Right triangle star pattern (left-aligned).
 * Prints rows of stars where row i has i stars.
 *
 * Theory:
 * - Uses nested loops: outer loop controls rows, inner loop controls columns.
 * - Pattern: For n rows, row number i (1-based) prints i stars.
 * - Simple O(n^2) nested iteration.
 *
 * Sample I/O:
 * Input:  5
 * Output:
 * *
 * **
 * ***
 * ****
 * *****
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: Print this triangle right-aligned instead.
 * A: Add spaces before stars: loop j=1 to n-i print space, then print stars.
 * Q: Can you print a triangle with odd numbers of stars (1,3,5...)?
 * A: Change inner loop to j < 2*i.
 *
 * Common Mistakes:
 * - Off-by-one: using < instead of <= (or vice versa).
 * - Forgetting std::endl or '\n' to move to next line.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows: ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            std::cout << '*';
        }
        std::cout << '\n';
    }

    return 0;
}
