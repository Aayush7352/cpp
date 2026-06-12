/*
 * 58_Character_Pyramid.cpp
 * ------------------------
 * Character pyramid: printing characters A, B, C, ... in pyramid form.
 *
 * Theory:
 * - Characters are represented as integers in ASCII ('A' = 65).
 * - Each row prints characters from 'A' to 'A' + i - 1.
 * - Can be extended to palindromic character pyramid like 57.
 *
 * Sample I/O:
 * Input:  5
 * Output:
 *     A
 *    AB
 *   ABC
 *  ABCD
 * ABCDE
 *
 * Complexity: O(n^2) time, O(1) space
 * Interview Questions:
 * Q: Print a pyramid where each row repeats the same character.
 * A: Print char('A' + i - 1) repeatedly in row i.
 * Q: Print in reverse alphabetical order.
 * A: Loop from 'A'+i-1 down to 'A'.
 *
 * Common Mistakes:
 * - Adding integer to char without casting (works, but know that
 *   char + int promotes to int; must cast back if storing).
 * - Forgetting that characters wrap around after 'Z'.
 */

#include <iostream>

int main() {
    int n;
    std::cout << "Enter number of rows: ";
    std::cin >> n;

    for (int i = 1; i <= n; ++i) {
        for (int s = 1; s <= n - i; ++s)
            std::cout << ' ';
        for (int j = 0; j < i; ++j)
            std::cout << char('A' + j);
        std::cout << '\n';
    }

    return 0;
}
