// ============================================================================
// Phase 14 — Modern C++
// Topic 172: consteval – Immediate Functions (C++20)
// ============================================================================
//
// THEORY:
// --------
// consteval (C++20): Immediate functions MUST produce a constant expression.
// Every call to a consteval function yields a compile-time constant.
// constexpr vs consteval:
//   constexpr    : "may" be evaluated at compile time; can be called at runtime.
//   consteval    : "must" be evaluated at compile time; calling at runtime is
//                  ill-formed.
//
// Immediate functions are implicitly inline. They cannot be called at runtime
// even if all arguments are constexpr-known — they produce a constant, not a
// runtime value. Use consteval when you want to guarantee compile-time
// evaluation (e.g., for static_assert, template args, array sizes).
//
// KEY POINTS:
// - consteval functions are always evaluated during translation (constant
//   evaluation).
// - A consteval function cannot be declared constexpr (redundant).
// - A consteval function's address cannot be taken at runtime.
// - consteval allows an immediate function context: any expressions inside
//   the function are also forced to be constant.
//
// ============================================================================

#include <iostream>
#include <array>
#include <cassert>

// ---------- 1. Basic consteval ----------
// Computes square — guaranteed at compile time.
consteval int square(int x)
{
    return x * x;
}

// ---------- 2. consteval with complex logic ----------
consteval std::size_t count_digits(std::size_t n)
{
    std::size_t count = 0;
    do {
        ++count;
        n /= 10;
    } while (n != 0);
    return count;
}

// ---------- 3. consteval used for template argument ----------
consteval std::size_t factorial_small(std::size_t n)
{
    // Only valid for small n (compile-time constraints)
    std::size_t r = 1;
    for (std::size_t i = 2; i <= n; ++i)
        r *= i;
    return r;
}

template <std::size_t N>
struct Factorial
{
    static constexpr std::size_t value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0>
{
    static constexpr std::size_t value = 1;
};

// ---------- 4. consteval vs constexpr — subtle distinction ----------
constexpr int constexpr_fn(int x) { return x * 2; }
consteval int consteval_fn(int x) { return x * 2; }

void demo_runtime_vs_compiletime()
{
    int a = 42;

    // constexpr_fn can be called at runtime:
    int b = constexpr_fn(a);    // OK — runtime call

    // consteval_fn cannot:
    // int c = consteval_fn(a); // ERROR — a is not constant expression
    // consteval_fn(a);         // ERROR

    // But both work with constant expression:
    constexpr int c1 = constexpr_fn(21);     // OK
    constexpr int c2 = consteval_fn(21);     // OK
}

// ============================================================================
// SAMPLE I/O:
// -----------
// square(11) = 121
// count_digits(12345) = 5
// 6! = 720
// static_assert passed: factorial_small(6) == 720
// ============================================================================

int main()
{
    // consteval guarantees compile-time — we can use in constant expressions
    constexpr int s = square(11);
    std::cout << "square(11) = " << s << "\n";

    constexpr auto digits = count_digits(12345);
    std::cout << "count_digits(12345) = " << digits << "\n";

    // Using consteval for compile-time factorial
    constexpr std::size_t fact6 = factorial_small(6);
    static_assert(fact6 == 720, "6! should be 720");
    std::cout << "6! = " << fact6 << "\n";

    // Also works with Factorial metafunction (template metaprogramming)
    constexpr auto fact5 = Factorial<5>::value;
    std::cout << "5! = " << fact5 << "\n";

    static_assert(square(10) == 100);
    std::cout << "static_assert passed: square(10) == 100\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// square: O(1)
// count_digits: O(log10 n)
// factorial_small: O(n)
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: When would you choose consteval over constexpr?
// A1: When you MUST guarantee compile-time evaluation — e.g., computing
//     a template argument, array size, or static_assert condition.
//     consteval catches accidental runtime calls.
//
// Q2: Can a consteval function call a constexpr function?
// A2: Yes. Immediately functions can call non-immediate constexpr functions.
//     The reverse is also true — constexpr functions can call consteval
//     functions (they are forced to be constant in that path).
//
// Q3: What happens if a consteval function cannot be evaluated?
// A3: The program is ill-formed — a compile error. Unlike constexpr which
//     silently falls back to runtime, consteval requires the constant.
//
// COMMON MISTAKES:
// ----------------
// - Passing runtime variables to consteval functions — leads to compile error.
// - Thinking consteval makes a function "faster" — it moves work to compile
//   time, but the generated constant is just an immediate value.
// - Forgetting that consteval implies inline; ODR issues are rare but possible
//   with C++20 modules.
// ============================================================================
