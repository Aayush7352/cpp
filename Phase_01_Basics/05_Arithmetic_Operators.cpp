/*
 * ====================================================================
 * Program 5: Arithmetic Operators
 *
 * THEORY:
 *   +   Addition
 *   -   Subtraction
 *   *   Multiplication
 *   /   Division (integer division truncates toward zero)
 *   %   Modulus (remainder, only for integer types)
 *
 * OPERATOR PRECEDENCE:  * / %  >  + -
 * Use parentheses to override: (a + b) * c
 *
 * INTEGER OVERFLOW:
 *   If an arithmetic operation exceeds the range of the type, it
 *   wraps around for unsigned types (modulo 2^n) and is undefined
 *   behavior (UB) for signed types.
 *
 * DIVISION BY ZERO:
 *   Integer division or modulus by zero is undefined behavior (UB).
 *   Floating-point division by zero yields +inf, -inf, or NaN.
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:
 *     a = 10, b = 3
 *   Output:
 *     a + b = 13
 *     a - b = 7
 *     a * b = 30
 *     a / b = 3          (integer division)
 *     a % b = 1
 *     a / 0 would be undefined behavior
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: What is the result of -5 % 2 in C++?
 *   A: -1 (sign of the dividend is preserved)
 *   Q: What happens when a signed integer overflows?
 *   A: Undefined behavior — the program may crash or give wrong results.
 *   Q: Explain integer division vs floating-point division.
 *   A: Integer division truncates toward zero (e.g., 10/3 = 3).
 *      Use static_cast<double>(a) / b for floating division.
 *
 * COMMON MISTAKES:
 *   - Integer division when expecting fractional results.
 *   - Ignoring overflow in competitive programming (use long long).
 *   - Modulus with negative operands can be confusing.
 *   - Division by zero — always check before dividing.
 * ====================================================================
 */

#include <iostream>
#include <limits>
#include <cfenv>
#include <cmath>

int main()
{
    int a = 20;
    int b = 7;

    std::cout << "a = " << a << ", b = " << b << "\n\n";

    // ---- Basic arithmetic ----
    int sum  = a + b;
    int diff = a - b;
    int prod = a * b;
    int quot = a / b;       // integer division → truncates toward zero
    int rem  = a % b;

    std::cout << "=== Basic arithmetic ===\n";
    std::cout << "a + b = " << sum  << '\n';
    std::cout << "a - b = " << diff << '\n';
    std::cout << "a * b = " << prod << '\n';
    std::cout << "a / b = " << quot << "  (integer division)\n";
    std::cout << "a % b = " << rem  << "  (modulus)\n";

    // ---- Negative modulus ----
    std::cout << "\n=== Negative modulus ===\n";
    std::cout << "(-5) % 2 = " << (-5) % 2 << '\n';   // -1
    std::cout << "5 % (-2) = " << 5 % (-2) << '\n';   // 1

    // ---- Integer overflow demonstration ----
    std::cout << "\n=== Integer overflow ===\n";
    int max_int = std::numeric_limits<int>::max();
    std::cout << "INT_MAX = " << max_int << '\n';
    std::cout << "INT_MAX + 1 = " << max_int + 1 << "  (overflow, UB)\n";

    unsigned int max_uint = std::numeric_limits<unsigned int>::max();
    std::cout << "UINT_MAX = " << max_uint << '\n';
    std::cout << "UINT_MAX + 1 = " << max_uint + 1
              << "  (well-defined wrap-around)\n";

    // ---- Floating-point division ----
    std::cout << "\n=== Floating-point division ===\n";
    double da = 20.0;
    double db = 7.0;
    double fquot = da / db;
    std::cout << "20.0 / 7.0 = " << fquot << '\n';

    // ---- Division by zero (floating point) ----
    std::cout << "\n=== Division by zero (FP) ===\n";
    double pos_inf = 1.0 / 0.0;
    double neg_inf = -1.0 / 0.0;
    double nan_val = 0.0 / 0.0;
    std::cout << "1.0 / 0.0  = " << pos_inf << "  (+inf)\n";
    std::cout << "-1.0 / 0.0 = " << neg_inf << "  (-inf)\n";
    std::cout << "0.0 / 0.0  = " << nan_val << "  (NaN)\n";

    // ---- Guard against integer division by zero ----
    std::cout << "\n=== Safety check ===\n";
    int divisor = 0;
    if (divisor != 0)
    {
        std::cout << "10 / divisor = " << 10 / divisor << '\n';
    }
    else
    {
        std::cout << "Cannot divide by zero (integer).\n";
    }

    return 0;
}
