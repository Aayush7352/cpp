/*
 * 59_Hollow_Diamond.cpp
 * ---------------------
 * Hollow diamond shape: only border stars, interior is spaces.
 *
 * Theory:
 * - Upper half: spaces = n-i, stars at border of 2*i-1 width.
 * - Lower half: spaces = i, stars at border of 2*(n-i)-1 width.
 * - A star is printed only at j==1 or j==last (2*i-1).
 *
 * Sample I/O:
 * Input:  5
 * Output:
 *     *
 *    * *
 *   *   *
 *  *     *
 * *       *
 *  *     *
 *   *   *
 *    * *
 *     *
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: How would you fill only the diagonals of the diamond?
 * A: Add conditions for j==i (left diagonal) and j==2*i-1 - (i-1).
 * Q: Can this be combined with numbers inside?
 * A: Yes, print row/col numbers in the interior instead of spaces.
 *
 * Common Mistakes:
 * - Forgetting that the middle row (i==n) has only 2 stars (j==1 and j==last).
 * - Off-by-one in the lower half space calculation.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows (upper half): ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int s = 1; s <= n - i; ++s) std::cout << ' ';
        for (int j = 1; j <= 2 * i - 1; ++j) {
            if (j == 1 || j == 2 * i - 1) std::cout << '*';
            else std::cout << ' ';
        }
        std::cout << '\n';
    }

    for (int i = 1; i <= n - 1; ++i) {
        for (int s = 1; s <= i; ++s) std::cout << ' ';
        for (int j = 1; j <= 2 * (n - i) - 1; ++j) {
            if (j == 1 || j == 2 * (n - i) - 1) std::cout << '*';
            else std::cout << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
