/*
 * ====================================================================
 * Program 6: Relational Operators
 *
 * THEORY:
 *   ==   equal to
 *   !=   not equal to
 *   <    less than
 *   >    greater than
 *   <=   less than or equal to
 *   >=   greater than or equal to
 *
 * All relational operators return bool (true or false).
 *
 * CHAINING TRAP:
 *   `a < b < c` is parsed as `(a < b) < c`.
 *   Since `(a < b)` is bool (0 or 1), this compares 0/1 with c.
 *   Use `(a < b && b < c)` instead.
 *
 * MANIPULATORS:
 *   std::boolalpha — prints true/false instead of 1/0.
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:  5 10
 *   Output:
 *     a == b ? false
 *     a != b ? true
 *     a < b  ? true
 *     a > b  ? false
 *     a <= b ? true
 *     a >= b ? false
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: Why is `a < b < c` wrong in C++?
 *   A: It evaluates left to right: (a < b) yields bool, then
 *      (bool) < c compares 0/1 with c. Use a < b && b < c.
 *   Q: Can relational operators be chained like Python?
 *   A: No. C++ does not support mathematical chaining.
 *   Q: What does `5 < 10 > 3` evaluate to?
 *   A: (5 < 10) = true = 1, then 1 > 3 = false.
 *
 * COMMON MISTAKES:
 *   - Using = instead of == for comparison.
 *   - Chaining comparisons incorrectly.
 *   - Comparing floating-point numbers with == (precision issues).
 *   - Forgetting that signed/unsigned comparison can behave oddly.
 * ====================================================================
 */

#include <iostream>

int main()
{
    int a = 10;
    int b = 20;
    int c = 15;

    std::cout << "a = " << a << ", b = " << b << ", c = " << c << "\n\n";

    // ---- All relational operators ----
    std::cout << std::boolalpha;  // print true/false instead of 1/0

    std::cout << "=== Basic comparisons ===\n";
    std::cout << "a == b : " << (a == b) << '\n';
    std::cout << "a != b : " << (a != b) << '\n';
    std::cout << "a <  b : " << (a < b)  << '\n';
    std::cout << "a >  b : " << (a > b)  << '\n';
    std::cout << "a <= b : " << (a <= b) << '\n';
    std::cout << "a >= b : " << (a >= b) << '\n';

    // ---- Chaining trap (a < b < c) ----
    std::cout << "\n=== Chaining trap ===\n";
    // Intended: 10 < 15 < 20? Should be true.
    // Actual:   (10 < 15) = true = 1, then 1 < 20 = true (by coincidence)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
    std::cout << "10 < 15 < 20 : " << (10 < 15 < 20)
              << "  (WRONG! Parsed as (10 < 15) < 20)\n";

    // Now a counter-example:
    std::cout << "10 < 5 < 20  : " << (10 < 5 < 20)
              << "  (WRONG! (10 < 5) = false = 0, 0 < 20 = true)\n";
#pragma GCC diagnostic pop

    // Correct way:
    std::cout << "10 < 15 && 15 < 20 : " << (10 < 15 && 15 < 20)
              << "  (correct)\n";

    // ---- Float comparison issue ----
    std::cout << "\n=== Float comparison (precision) ===\n";
    double x = 0.1 + 0.2;
    double y = 0.3;
    std::cout << "0.1 + 0.2 == 0.3 : " << (x == y) << '\n';
    std::cout << "x = " << x << ", y = " << y << '\n';

    // Better: check with epsilon
    constexpr double eps = 1e-9;
    bool approx_equal = (std::abs(x - y) < eps);
    std::cout << "abs(x - y) < eps : " << approx_equal << '\n';

    // ---- Signed / unsigned comparison ----
    std::cout << "\n=== Signed / unsigned comparison ===\n";
    int si = -1;
    unsigned int ui = 1;
    std::cout << "-1 < 1u : " << (static_cast<unsigned>(si) < ui)
              << "  (surprising! -1 converts to large unsigned)\n";

    return 0;
}
