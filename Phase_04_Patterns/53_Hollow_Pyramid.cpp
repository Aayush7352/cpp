/*
 * 53_Hollow_Pyramid.cpp
 * ---------------------
 * Hollow pyramid: only border stars are printed, interior is spaces.
 *
 * Theory:
 * - Same space and star counts as full pyramid (52).
 * - Stars appear at first column (j==1), last column (j==2*i-1),
 *   and the last row (i==n). All other positions are spaces.
 *
 * Sample I/O:
 * Input:  5
 * Output:
 *     *
 *    * *
 *   *   *
 *  *     *
 * *********
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: How would you create a hollow inverted pyramid?
 * A: Reverse outer loop; stars at j==1, j==2*i-1, or i==1.
 * Q: Can you make this a number hollow pyramid?
 * A: Print i or j at border positions instead of '*'.
 *
 * Common Mistakes:
 * - Printing spaces for all interior cells (correct) but forgetting
 *   that the last row must be fully filled.
 * - Confusing the condition for first/last column: first is j==1,
 *   last is j==2*i-1.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows: ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int s = 1; s <= n - i; ++s)
            std::cout << ' ';

        for (int j = 1; j <= 2 * i - 1; ++j) {
            if (j == 1 || j == 2 * i - 1 || i == n)
                std::cout << '*';
            else
                std::cout << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
