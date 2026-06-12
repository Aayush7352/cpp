/*
 * ====================================================================
 * Program 8: Bitwise Operators
 *
 * THEORY:
 *   &  bitwise AND      0b1100 & 0b1010 = 0b1000
 *   |  bitwise OR       0b1100 | 0b1010 = 0b1110
 *   ^  bitwise XOR      0b1100 ^ 0b1010 = 0b0110
 *   ~  bitwise NOT      ~0b1100 = ...11110011 (two's complement)
 *   << left shift        0b0001 << 2 = 0b0100  (multiply by 2^n)
 *   >> right shift       0b0100 >> 2 = 0b0001  (divide by 2^n)
 *
 * PRACTICAL BIT MANIPULATION (using 0-based index):
 *   Check bit:   (x >> n) & 1
 *   Set bit:     x | (1 << n)
 *   Clear bit:   x & ~(1 << n)
 *   Toggle bit:  x ^ (1 << n)
 *   Power of 2:  x > 0 && (x & (x - 1)) == 0
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:  x = 29 (0b11101), n = 3
 *   Output:
 *     Check bit 3: 1
 *     Set bit 2:   29 → 29
 *     Clear bit 2: 29 → 25
 *     Toggle bit 3: 29 → 21
 *     Is power of 2? false
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: How do you check if a number is a power of 2?
 *   A: x > 0 && (x & (x - 1)) == 0
 *   Q: How do you swap two numbers without a temp variable?
 *   A: a ^= b; b ^= a; a ^= b;  (XOR swap)
 *   Q: How do you count set bits (popcount)?
 *   A: Use std::popcount (C++20) or Brian Kernighan's algorithm.
 *   Q: What does ~0 << 2 produce?
 *   A: All bits set, shifted left by 2 → ...11111100.
 *
 * COMMON MISTAKES:
 *   - Confusing bitwise & and | with logical && and ||.
 *   - Forgetting operator precedence: (x & mask) == 0 vs x & mask == 0.
 *   - Shifting by negative or too-large count (undefined behavior).
 *   - Right shift on signed negative numbers is implementation-defined.
 * ====================================================================
 */

#include <iostream>
#include <bitset>
#include <bit>  // C++20: std::popcount

template <typename T>
void print_bin(T x)
{
    std::cout << std::bitset<8>(static_cast<unsigned int>(x)) << " (" << x << ")";
}

int main()
{
    unsigned char x = 0b01101101;  // 109 in decimal
    unsigned char y = 0b00011011;  // 27

    std::cout << "x = "; print_bin(x); std::cout << '\n';
    std::cout << "y = "; print_bin(y); std::cout << '\n';

    // ---- Basic bitwise operation ----
    std::cout << "\n=== Basic operations ===\n";
    std::cout << "x & y  = "; print_bin(x & y);  std::cout << '\n';
    std::cout << "x | y  = "; print_bin(x | y);  std::cout << '\n';
    std::cout << "x ^ y  = "; print_bin(x ^ y);  std::cout << '\n';
    std::cout << "~x     = "; print_bin(~x);     std::cout << '\n';
    std::cout << "x << 2 = "; print_bin(x << 2); std::cout << '\n';
    std::cout << "x >> 2 = "; print_bin(x >> 2); std::cout << '\n';

    // ---- Practical: Check, Set, Clear, Toggle ----
    std::cout << "\n=== Bit manipulation (n = 3) ===\n";
    int n = 3;
    unsigned char val = 0b00101101;  // 45

    // Check bit n
    bool bit_n = (val >> n) & 1;
    std::cout << "Bit " << n << " of "; print_bin(val);
    std::cout << " is " << bit_n << '\n';

    // Set bit n
    unsigned char set_val = val | (1 << n);
    std::cout << "Set bit " << n << ":  "; print_bin(set_val); std::cout << '\n';

    // Clear bit n
    unsigned char clear_val = val & ~(1 << n);
    std::cout << "Clear bit " << n << ": "; print_bin(clear_val); std::cout << '\n';

    // Toggle bit n
    unsigned char toggle_val = val ^ (1 << n);
    std::cout << "Toggle bit " << n << ": "; print_bin(toggle_val); std::cout << '\n';

    // ---- Power of 2 check ----
    std::cout << "\n=== Power of 2 check ===\n";
    for (unsigned int v : {0U, 1U, 2U, 3U, 16U, 31U, 64U})
    {
        bool is_pow2 = v > 0 && (v & (v - 1)) == 0;
        std::cout << v << " is power of 2? " << std::boolalpha << is_pow2 << '\n';
    }

    // ---- C++20 std::popcount ----
    std::cout << "\n=== C++20 popcount ===\n";
    unsigned int pop_val = 0b1101101;  // 5 set bits
    std::cout << "popcount(" << pop_val << ") = "
              << std::popcount(pop_val) << '\n';

    // ---- XOR swap ----
    std::cout << "\n=== XOR Swap ===\n";
    int a = 10, b = 20;
    std::cout << "Before: a = " << a << ", b = " << b << '\n';
    a ^= b;
    b ^= a;
    a ^= b;
    std::cout << "After:  a = " << a << ", b = " << b << '\n';

    return 0;
}
