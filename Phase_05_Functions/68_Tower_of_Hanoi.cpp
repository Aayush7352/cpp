/*
 * 68_Tower_of_Hanoi.cpp
 * ---------------------
 * Classic Tower of Hanoi puzzle with move count.
 *
 * Theory:
 * - Move n disks from source to destination using auxiliary peg.
 * - Recursive formula:
 *   1) Move n-1 disks from source to auxiliary.
 *   2) Move nth disk from source to destination.
 *   3) Move n-1 disks from auxiliary to destination.
 * - Total moves = 2^n - 1 (minimum).
 *
 * Sample I/O:
 * Enter number of disks: 3
 * Move disk 1 from A to C
 * Move disk 2 from A to B
 * Move disk 1 from C to B
 * Move disk 3 from A to C
 * Move disk 1 from B to A
 * Move disk 2 from B to C
 * Move disk 1 from A to C
 * Total moves: 7
 *
 * Complexity: O(2^n) time, O(n) stack space
 * Interview Questions:
 * Q: Why is the minimum number of moves 2^n - 1?
 * A: Recurrence T(n) = 2T(n-1) + 1 solves to 2^n - 1.
 * Q: Can this be solved iteratively?
 * A: Yes, using a stack or the binary representation of moves.
 *
 * Common Mistakes:
 * - Swapping source/destination arguments in recursive calls.
 * - Off-by-one in disk numbering (should print disk numbers correctly).
 * - Stack overflow for large n (n > 30 is impractical).
 */

#include <iostream>
#include <string>

void hanoi(int n, const std::string &src, const std::string &dst,
           const std::string &aux, int &moves) {
    if (n == 0) return;
    hanoi(n - 1, src, aux, dst, moves);
    std::cout << "Move disk " << n << " from " << src << " to " << dst << '\n';
    ++moves;
    hanoi(n - 1, aux, dst, src, moves);
}

int main() {
    int n, moves = 0;
    std::cout << "Enter number of disks: ";
    std::cin >> n;

    hanoi(n, "A", "C", "B", moves);
    std::cout << "Total moves: " << moves << '\n';

    return 0;
}
