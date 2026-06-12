// ============================================================================
// Phase 14 — Modern C++ (C++11/14/17/20)
// Topic 171: constexpr – Compile-time Evaluation
// ============================================================================
//
// THEORY:
// --------
// constexpr (C++11): Specifies that a function or variable is evaluable at
// compile time. constexpr functions may also be called at runtime.
// if constexpr (C++17): Compile-time branching — the discarded branch is not
// instantiated (no codegen). Reduces need for SFINAE.
// constexpr containers (C++20): std::vector and std::string can be used in
// constexpr context; dynamic allocations must be deallocated during evaluation.
//
// KEY POINTS:
// - constexpr function == "possibly evaluated at compile time if inputs are constexpr"
// - if constexpr requires a bool constant expression
// - constexpr lambdas (C++17), constexpr virtual functions (C++20)
//
// ============================================================================

#include <iostream>
#include <array>
#include <vector>
#include <cassert>

// ---------- 1. constexpr function ----------
// Computes factorial at compile time when called with constant expression.
constexpr int factorial(int n)
{
    // C++14 allowed local vars and loops inside constexpr functions
    int result = 1;
    for (int i = 2; i <= n; ++i)
        result *= i;
    return result;
}

// ---------- 2. if constexpr ----------
// Compile-time branching: only the selected branch is compiled.
template <typename T>
constexpr auto type_info() -> const char*
{
    if constexpr (std::is_integral_v<T>)
        return "integral type";
    else if constexpr (std::is_floating_point_v<T>)
        return "floating-point type";
    else
        return "other type";
}

// ---------- 3. constexpr container (C++20) ----------
// std::vector or std::string can be used in constexpr context in C++20.
constexpr std::size_t build_and_sum()
{
    std::vector<int> v{1, 2, 3, 4, 5};
    std::size_t s = 0;
    for (auto x : v) s += static_cast<std::size_t>(x);
    return s;                         // must deallocate — OK in C++20
}

// ---------- 4. constexpr std::array (compile-time known size) ----------
constexpr std::array<int, 5> make_squares()
{
    std::array<int, 5> arr{};
    for (std::size_t i = 0; i < arr.size(); ++i)
        arr[i] = static_cast<int>((i + 1) * (i + 1));
    return arr;
}

// ============================================================================
// SAMPLE I/O:
// -----------
// Factorial(5) = 120
// int is integral type
// double is floating-point type
// char* is other type
// constexpr vector sum = 15
// Squares: 1 4 9 16 25
// ============================================================================

int main()
{
    // Compile-time evaluation (verified with static_assert)
    static_assert(factorial(5) == 120, "factorial(5) should be 120");

    int n = 0;
    std::cout << "Enter a number: ";
    std::cin >> n;

    // Runtime evaluation (inputs not constant)
    std::cout << "Factorial(" << n << ") = " << factorial(n) << "\n";

    // if constexpr
    std::cout << "int is " << type_info<int>() << "\n";
    std::cout << "double is " << type_info<double>() << "\n";
    std::cout << "char* is " << type_info<char*>() << "\n";

    // constexpr container
    constexpr auto sum = build_and_sum();
    std::cout << "constexpr vector sum = " << sum << "\n";

    // constexpr array
    constexpr auto sq = make_squares();
    std::cout << "Squares:";
    for (auto x : sq) std::cout << ' ' << x;
    std::cout << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// factorial: O(n) time, O(1) space
// build_and_sum: O(n) time/space (compile-time)
// make_squares: O(n) time, O(1) space
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What's the difference between constexpr and const?
// A1: const implies runtime const-ness; constexpr guarantees compile-time
//     evaluation when possible. constexpr variables are implicitly const.
//
// Q2: Can a constexpr function have side effects?
// A2: Yes, at runtime. At compile time, side effects are evaluated only
//     as part of constant expression evaluation and produce no observable effect.
//
// Q3: When does if constexpr matter vs regular if?
// A3: if constexpr discards the false branch entirely at compile time — the
//     code is not instantiated. This avoids compile errors in unused branches.
//
// COMMON MISTAKES:
// ----------------
// - Calling a constexpr function with runtime variables expecting compile-time
//   guarantee — it will run at runtime.
// - In C++17, using std::vector in constexpr context is still forbidden
//   (requires C++20).
// - Forgetting that if constexpr requires a constant expression condition.
// ============================================================================
