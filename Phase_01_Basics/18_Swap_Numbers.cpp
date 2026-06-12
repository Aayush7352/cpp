/*
 * ====================================================================
 * Program 18: Swap Numbers
 *
 * THEORY:
 * Swapping two values is a fundamental programming operation. Several
 * techniques exist:
 *
 * 1. Using a temporary variable
 *    int temp = a; a = b; b = temp;
 *    — Simple, clear, works with all types.
 *
 * 2. Using + and - (integer only, overflow risk)
 *    a = a + b;  b = a - b;  a = a - b;
 *
 * 3. Using XOR (integer only, bitwise)
 *    a ^= b;  b ^= a;  a ^= b;
 *    — No overflow, but only works on integral types.
 *
 * 4. Using std::swap (C++11, recommended)
 *    std::swap(a, b);
 *    — Generic, efficient, noexcept, uses move semantics.
 *
 * 5. Using references or pointers
 *    void swap(int& a, int& b);
 *    void swap(int* a, int* b);
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:  a = 5, b = 10
 *   Output (for each method):
 *     Before: a = 5, b = 10
 *     After:  a = 10, b = 5
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)  (temp method uses 1 extra variable)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: Which swap method is best?
 *   A: std::swap (generic, safe, optimized). Only use XOR swap
 *      in embedded systems with extreme memory constraints.
 *   Q: Why is XOR swap dangerous?
 *   A: It fails if a and b refer to the same location (a ^= a → 0).
 *      The temp method is safer and just as fast with optimization.
 *   Q: Is a + b - a overflow-safe?
 *   A: No — signed overflow is UB. Use the temp method or std::swap.
 *
 * COMMON MISTAKES:
 *   - XOR swap when a and b alias the same variable (sets to 0).
 *   - Arithmetic swap with signed overflow (undefined behavior).
 *   - Forgetting to pass by reference (pass-by-value won't swap).
 *   - Using std::swap without <utility> (though <iostream> may include
 *     it indirectly — always include <utility> to be safe).
 * ====================================================================
 */

#include <iostream>
#include <utility>  // std::swap

// ---- Method 1: Reference-based temp swap ----
void swap_with_temp(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// ---- Method 2: Pointer-based temp swap ----
void swap_ptr(int* a, int* b)
{
    if (a == nullptr || b == nullptr)
        return;
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ---- Method 3: Arithmetic swap (+/-) ----
void swap_arithmetic(int& a, int& b)
{
    if (&a == &b) return;  // same variable guard
    a = a + b;
    b = a - b;
    a = a - b;
}

// ---- Method 4: XOR swap (bitwise) ----
void swap_xor(int& a, int& b)
{
    if (&a == &b) return;  // critical: XOR with self gives 0
    a ^= b;
    b ^= a;
    a ^= b;
}

// ---- Template version using temp (works with any type) ----
template <typename T>
void swap_template(T& a, T& b)
{
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

int main()
{
    int a = 5, b = 10;

    // ---- Method 1: Temp variable ----
    std::cout << "=== Method 1: Temp variable ===\n";
    std::cout << "Before: a = " << a << ", b = " << b << '\n';
    swap_with_temp(a, b);
    std::cout << "After:  a = " << a << ", b = " << b << "\n\n";

    // Reset
    a = 5; b = 10;

    // ---- Method 2: Pointers ----
    std::cout << "=== Method 2: Pointers ===\n";
    std::cout << "Before: a = " << a << ", b = " << b << '\n';
    swap_ptr(&a, &b);
    std::cout << "After:  a = " << a << ", b = " << b << "\n\n";

    // Reset
    a = 5; b = 10;

    // ---- Method 3: Arithmetic ----
    std::cout << "=== Method 3: Arithmetic (+/-) ===\n";
    std::cout << "Before: a = " << a << ", b = " << b << '\n';
    swap_arithmetic(a, b);
    std::cout << "After:  a = " << a << ", b = " << b << "\n\n";

    // Reset
    a = 5; b = 10;

    // ---- Method 4: XOR ----
    std::cout << "=== Method 4: XOR ===\n";
    std::cout << "Before: a = " << a << ", b = " << b << '\n';
    swap_xor(a, b);
    std::cout << "After:  a = " << a << ", b = " << b << "\n\n";

    // Reset
    a = 5; b = 10;

    // ---- Method 5: std::swap ----
    std::cout << "=== Method 5: std::swap ===\n";
    std::cout << "Before: a = " << a << ", b = " << b << '\n';
    std::swap(a, b);
    std::cout << "After:  a = " << a << ", b = " << b << "\n\n";

    // ---- Method 6: Template swap ----
    double dx = 1.5, dy = 9.9;
    std::cout << "=== Method 6: Template swap (double) ===\n";
    std::cout << "Before: dx = " << dx << ", dy = " << dy << '\n';
    swap_template(dx, dy);
    std::cout << "After:  dx = " << dx << ", dy = " << dy << '\n';

    // ---- Edge case: XOR with same variable ----
    std::cout << "\n=== Edge case: XOR with self ===\n";
    a = 42;
    swap_xor(a, a);  // &a == &a → guarded, no-op
    std::cout << "swap_xor(a, a) = " << a << " (should still be 42)\n";

    return 0;
}
