/*
 * 57_Number_Pyramid.cpp
 * ---------------------
 * Palindromic number pyramid: numbers ascend then descend in each row.
 *
 * Theory:
 * - Row i has spaces (n-i), then ascending numbers 1..i,
 *   then descending numbers i-1..1.
 * - Creates a palindromic pattern centered at the peak (i).
 *
 * Sample I/O:
 * Input:  5
 * Output:
 *     1
 *    121
 *   12321
 *  1234321
 * 123454321
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: Modify to print only odd rows.
 * A: Increment i by 2 instead of 1.
 * Q: How would you print this with a single loop per half?
 * A: First loop j=1..i ascending; second loop j=i-1..1 descending.
 *
 * Common Mistakes:
 * - Descending loop starting from i (duplicates the peak number).
 * - Off-by-one in the descending loop: must start from i-1.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows: ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int s = 1; s <= n - i; ++s)
            std::cout << ' ';
        for (int j = 1; j <= i; ++j)
            std::cout << j;
        for (int j = i - 1; j >= 1; --j)
            std::cout << j;
        std::cout << '\n';
    }

    return 0;
}
