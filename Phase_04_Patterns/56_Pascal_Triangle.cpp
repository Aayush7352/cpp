/*
 * 56_Pascal_Triangle.cpp
 * ----------------------
 * Pascal's triangle using binomial coefficients C(n,k).
 *
 * Theory:
 * - Each entry is C(row, col) where row and col are 0-based.
 * - C(n,k) = C(n,k-1) * (n-k+1) / k   (efficient iterative formula).
 * - Alternatively, use: C(n,k) = C(n-1,k-1) + C(n-1,k).
 * - Symmetric: C(n,k) = C(n,n-k).
 *
 * Sample I/O:
 * Input:  6
 * Output:
 *       1
 *      1 1
 *     1 2 1
 *    1 3 3 1
 *   1 4 6 4 1
 *  1 5 10 10 5 1
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: Find the nth row of Pascal's triangle.
 * A: Use the multiplicative formula; print C(row,0)..C(row,row).
 * Q: What is the sum of all elements in row n?
 * A: 2^n (each row sums to a power of 2).
 *
 * Common Mistakes:
 * - Using int and overflowing for large rows (>30).
 * - Integer division truncation: compute using long long and multiply before divide.
 */

#include <iostream>
#include <iomanip>

int main() {
    int n;
    std::cout << "Enter number of rows: ";
    std::cin >> n;

    for (int row = 0; row < n; ++row) {
        for (int s = 0; s < n - row - 1; ++s)
            std::cout << ' ';

        long long val = 1;
        for (int col = 0; col <= row; ++col) {
            std::cout << val << ' ';
            val = val * (row - col) / (col + 1);
        }
        std::cout << '\n';
    }

    return 0;
}
