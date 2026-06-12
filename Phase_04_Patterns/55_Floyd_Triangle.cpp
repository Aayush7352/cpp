/*
 * 55_Floyd_Triangle.cpp
 * ---------------------
 * Floyd's triangle: consecutive natural numbers in a right triangle.
 *
 * Theory:
 * - Row i contains i numbers.
 * - Numbers are incremented consecutively from 1.
 * - Named after Robert Floyd.
 *
 * Sample I/O:
 * Input:  5
 * Output:
 * 1
 * 2  3
 * 4  5  6
 * 7  8  9  10
 * 11 12 13 14 15
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: Print Floyd's triangle with binary (0/1) values.
 * A: Use a counter and print num%2 (toggle) or use row/col parity.
 * Q: Can you print it in reverse (descending numbers)?
 * A: Calculate total numbers = n*(n+1)/2, then decrement.
 *
 * Common Mistakes:
 * - Forgetting to increment the number variable inside the inner loop.
 * - Not setting width for alignment (numbers > 9 overflow columns).
 */

#include <iostream>
#include <iomanip>

int main() {
    int n, num = 1;
    std::cout << "Enter number of rows: ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            std::cout << std::setw(3) << num++;
        }
        std::cout << '\n';
    }

    return 0;
}
