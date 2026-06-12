/*
 * 60_Butterfly_Pattern.cpp
 * ------------------------
 * Butterfly star pattern: symmetric left and right wings.
 *
 * Theory:
 * - Upper half: stars = i on left, 2*(n-i) spaces, stars = i on right.
 * - Lower half: stars = n-i+1 on left, 2*(i-1) spaces, stars = n-i+1 on right.
 * - The pattern resembles butterfly wings.
 *
 * Sample I/O:
 * Input:  5
 * Output:
 * *        *
 * **      **
 * ***    ***
 * ****  ****
 * **********
 * ****  ****
 * ***    ***
 * **      **
 * *        *
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: Modify to print a heart pattern.
 * A: Use two separate upper curves with appropriate conditions.
 * Q: Can you print numbers instead of stars in the butterfly?
 * A: Yes, print column index j or i on each side.
 *
 * Common Mistakes:
 * - Miscalculating middle spaces: should be 2*(n-i) in upper half.
 * - Lower half loop goes n-1 rows (not n) to avoid double mid row.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows (half): ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) std::cout << '*';
        for (int s = 1; s <= 2 * (n - i); ++s) std::cout << ' ';
        for (int j = 1; j <= i; ++j) std::cout << '*';
        std::cout << '\n';
    }

    for (int i = n - 1; i >= 1; --i) {
        for (int j = 1; j <= i; ++j) std::cout << '*';
        for (int s = 1; s <= 2 * (n - i); ++s) std::cout << ' ';
        for (int j = 1; j <= i; ++j) std::cout << '*';
        std::cout << '\n';
    }

    return 0;
}
