/*
 * 52_Pyramid.cpp
 * --------------
 * Full pyramid (centered triangle star pattern).
 *
 * Theory:
 * - Row i has (n-i) leading spaces and (2*i-1) stars.
 * - Centering is achieved by printing spaces before stars.
 * - Each row has odd number of stars: 1, 3, 5, ...
 *
 * Sample I/O:
 * Input:  5
 * Output:
 *     *
 *    ***
 *   *****
 *  *******
 * *********
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: How would you print an inverted pyramid?
 * A: Reverse the outer loop: for i=n down to 1.
 * Q: Can you print a number pyramid using the same logic?
 * A: Yes, replace '*' with the row number or column number.
 *
 * Common Mistakes:
 * - Incorrect space count: should be n-i, not i-1.
 * - Forgetting that stars per row = 2*i-1.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows: ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int s = 1; s <= n - i; ++s)
            std::cout << ' ';
        for (int j = 1; j <= 2 * i - 1; ++j)
            std::cout << '*';
        std::cout << '\n';
    }

    return 0;
}
