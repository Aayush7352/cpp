/*
 * ====================================================================
 * Program 7: Logical Operators
 *
 * THEORY:
 *   &&   Logical AND  — true if both operands are true
 *   ||   Logical OR   — true if at least one operand is true
 *   !    Logical NOT  — negates the operand
 *
 * SHORT-CIRCUIT EVALUATION:
 *   In `a && b`, if `a` is false, `b` is never evaluated.
 *   In `a || b`, if `a` is true,  `b` is never evaluated.
 *   This is crucial for guarding: `ptr != nullptr && *ptr > 0`
 *
 * DE MORGAN'S LAWS:
 *   !(A && B) = !A || !B
 *   !(A || B) = !A && !B
 *
 * TRUTH TABLE:
 *   A     B     A&&B   A||B   !A
 *   T     T     T      T      F
 *   T     F     F      T      F
 *   F     T     F      T      T
 *   F     F     F      F      T
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input (age, hasLicense): 20 1 (or 20 true)
 *   Output: Can drive? true
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: What is short-circuit evaluation?
 *   A: The right operand is only evaluated if needed. Used for
 *      null-pointer safety, range checks, etc.
 *   Q: What is the difference between & and &&?
 *   A: & is bitwise AND (evaluates both sides). && is logical AND
 *      (short-circuits).
 *   Q: State De Morgan's laws.
 *   A: !(A && B) = !A || !B; !(A || B) = !A && !B.
 *
 * COMMON MISTAKES:
 *   - Using & instead of && (bitwise vs logical).
 *   - Forgetting that && has higher precedence than ||.
 *   - Relying on side effects in expressions that may short-circuit.
 *   - Writing `if (a == true)` instead of `if (a)`.
 * ====================================================================
 */

#include <iostream>

int main()
{
    bool a = true;
    bool b = false;

    std::cout << std::boolalpha;
    std::cout << "a = " << a << ", b = " << b << "\n\n";

    // ---- Truth table ----
    std::cout << "=== Truth table ===\n";
    std::cout << "a && b : " << (a && b) << '\n';
    std::cout << "a || b : " << (a || b) << '\n';
    std::cout << "!a     : " << (!a)     << '\n';
    std::cout << "!b     : " << (!b)     << '\n';

    // ---- Short-circuit demonstration ----
    std::cout << "\n=== Short-circuit evaluation ===\n";
    int x = 5;

    // Short-circuit: the division by zero is never executed
    bool result1 = (x != 0) && (10 / x > 1);
    std::cout << "(x != 0) && (10 / x > 1) : " << result1 << '\n';

    int y = 0;
    bool result2 = (y != 0) && (10 / y > 1);   // safe, short-circuit
    std::cout << "(y != 0) && (10 / y > 1) : " << result2
              << "  (short-circuited, no division)\n";

    // || short-circuit
    bool result3 = (y == 0) || (10 / y > 1);   // short-circuit, safe
    std::cout << "(y == 0) || (10 / y > 1) : " << result3
              << "  (short-circuited)\n";

    // ---- De Morgan's laws ----
    std::cout << "\n=== De Morgan's Laws ===\n";
    bool p = true;
    bool q = false;

    std::cout << "!(p && q) = " << !(p && q) << '\n';
    std::cout << "!p || !q = " << (!p || !q) << '\n';
    std::cout << "!(p || q) = " << !(p || q) << '\n';
    std::cout << "!p && !q = " << (!p && !q) << '\n';

    // ---- Practical: driving eligibility ----
    std::cout << "\n=== Practical: Can you drive? ===\n";
    int age = 20;
    bool has_license = true;
    bool can_drive = (age >= 18) && has_license;
    std::cout << "Age: " << age << ", Has license: " << has_license
              << " → Can drive: " << can_drive << '\n';

    // ---- Guarded access (nullptr) ----
    std::cout << "\n=== Guarded pointer access ===\n";
    int* ptr = nullptr;
    // Safe: short-circuit prevents dereferencing nullptr
    if (ptr != nullptr && *ptr > 0)
    {
        std::cout << "Value positive\n";
    }
    else
    {
        std::cout << "Pointer is null or value <= 0\n";
    }

    return 0;
}
