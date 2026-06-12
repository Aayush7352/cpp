/*
 * 64_Inline_Functions.cpp
 * -----------------------
 * inline functions: request to the compiler to avoid function call overhead.
 *
 * Theory:
 * - inline keyword suggests the compiler replace call with function body.
 * - Macros vs inline: macros are textual substitution (no type safety),
 *   inline functions respect scope, types, and have debug info.
 * - Inline functions in headers: ok (multiple definitions allowed if
 *   identical). OdR (One definition Rule) allows inline in headers.
 * - Compiler is free to ignore inline; modern compilers decide based
 *   on optimization level.
 *
 * Sample I/O:
 * max(10, 20) = 20
 * square(5) = 25
 * Inline is a hint, not a command.
 *
 * Complexity: O(1) time (eliminates call overhead)
 * Interview Questions:
 * Q: When should you use inline?
 * A: For very small, frequently called functions (getters, simple math).
 * Q: What are the downsides of inline?
 * A: Code bloat, slower compile times, instruction cache pressure if
 *    overused.
 * Q: Does inline force the compiler to inline?
 * A: No, it's a suggestion; compilers have heuristics.
 *
 * Common Mistakes:
 * - Defining large functions as inline (code bloat).
 * - Putting inline in the definition but not declaration.
 * - Expecting inline to guarantee performance improvement.
 */

#include <iostream>

inline int max(int a, int b) {
    return a > b ? a : b;
}

inline long square(long x) {
    return x * x;
}

int main() {
    int x = 10, y = 20;
    std::cout << "max(" << x << ", " << y << ") = " << max(x, y) << '\n';
    std::cout << "square(" << x << ") = " << square(x) << '\n';

    return 0;
}
