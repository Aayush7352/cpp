/*
 * 54_Diamond.cpp
 * --------------
 * Full diamond: pyramid (upper half) + inverted pyramid (lower half).
 *
 * Theory:
 * - Upper half: rows 1..n, stars = 2*i-1, spaces = n-i.
 * - Lower half: rows 1..n-1, stars = 2*(n-i)-1, spaces = i.
 * - n is the number of rows in the upper half (the "middle" row).
 *
 * Sample I/O:
 * Input:  5
 * Output:
 *     *
 *    ***
 *   *****
 *  *******
 * *********
 *  *******
 *   *****
 *    ***
 *     *
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: How would you print a hollow diamond?
 * A: Same layout, but only print stars at borders (j==1 or j==last).
 * Q: Can you print a diamond with an even number of rows?
 * A: Divide into two equal halves; you may adjust n*2 total rows.
 *
 * Common Mistakes:
 * - Lower half loop boundary: only n-1 rows to avoid duplicating middle.
 * - Forgetting spaces in the lower half increase as i increases.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows (upper half): ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int s = 1; s <= n - i; ++s) std::cout << ' ';
        for (int j = 1; j <= 2 * i - 1; ++j) std::cout << '*';
        std::cout << '\n';
    }

    for (int i = 1; i <= n - 1; ++i) {
        for (int s = 1; s <= i; ++s) std::cout << ' ';
        for (int j = 1; j <= 2 * (n - i) - 1; ++j) std::cout << '*';
        std::cout << '\n';
    }

    return 0;
}
