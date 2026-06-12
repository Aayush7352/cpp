/*
 * 65_Recursive_Factorial.cpp
 * --------------------------
 * Recursive factorial with tail recursion optimization (TRO).
 *
 * Theory:
 * - n! = n * (n-1)!
 * - Tail recursion: the recursive call is the LAST operation in the
 *   function. The compiler can optimize (TCO) to reuse stack frame.
 * - Non-tail version: n * fact(n-1) -- multiplication happens after
 *   the call. Tail version: fact(n, acc) => fact(n-1, n*acc).
 * - C++ compilers may or may not perform TCO (depends on optimization).
 *
 * Sample I/O:
 * Enter n: 6
 * 6! = 720
 *
 * Complexity: O(n) time, O(1) stack (if TCO applied, else O(n))
 * Interview Questions:
 * Q: Why is tail recursion better?
 * A: It can be optimized to iterative form, avoiding stack overflow.
 * Q: Does gcc/clang support TCO for C++?
 * A: Yes, with -O2 optimization, but debug builds typically don't.
 *
 * Common Mistakes:
 * - Assuming TCO always happens (it's compiler/optimization dependent).
 * - Overflow for large n (use long long or arbitrary precision).
 * - Non-tail version causing stack overflow for large n.
 */

#include <iostream>

long long tailFactorial(int n, long long acc = 1) {
    if (n <= 1) return acc;
    return tailFactorial(n - 1, n * acc);
}

int main() {
    int n;
    std::cout << "Enter n: ";
    std::cin >> n;

    std::cout << n << "! = " << tailFactorial(n) << '\n';

    return 0;
}
