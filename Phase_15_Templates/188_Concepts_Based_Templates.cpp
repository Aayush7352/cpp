// ============================================================================
// Phase 15 — Templates
// Topic 188: Concepts-Based Templates (C++20)
// ============================================================================
//
// THEORY:
// --------
// Concepts (C++20) provide named constraints for template parameters,
// replacing SFINAE with cleaner syntax and better error messages.
//
// This file focuses on using concepts WITH templates — constrained
// templates, not just basic concepts usage.
//
// Concept forms for templates:
//   1. template <Concept T> — constrained type parameter
//   2. template <typename T> requires Concept<T> — requires clause
//   3. template <typename T> requires (Concept<T>) — parenthesized
//   4. auto constrained_func(Concept auto x) — abbreviated syntax
//
// Concepts interact with template deduction, overload resolution,
// and partial ordering — more constrained templates are preferred.
//
// ============================================================================

#include <iostream>
#include <concepts>
#include <type_traits>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <ranges>

// ==================== 1. Defining Concepts for Templates ====================

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <typename T>
concept Addable = requires(T a, T b) { a + b; };

template <typename T>
concept Scalable = Numeric<T> && requires(T a, T b) { a * b; };

// ==================== 2. Constrained Function Templates ====================

// Template with concept in template parameter
template <Numeric T>
T square(T x)
{
    return x * x;
}

// Template with requires clause
template <typename T>
    requires Addable<T>
T add_twice(T a, T b)
{
    return a + b + a + b;
}

// Trailing requires clause
template <typename T>
T triple(T x) requires Numeric<T>
{
    return x * 3;
}

// ==================== 3. Abbreviated Function Template (C++20) ====================

void print_numeric(Numeric auto x)
{
    std::cout << x << "\n";
}

Addable auto add_abbrev(Addable auto a, Addable auto b)
{
    return a + b;
}

// ==================== 4. Constrained Class Templates ====================

template <Numeric T>
class Calculator
{
    T value;

public:
    explicit Calculator(T v) : value{v} {}

    T add(T x) const { return value + x; }
    T multiply(T x) const { return value * x; }
    T power(int exp) const
    {
        T result = static_cast<T>(1);
        for (int i = 0; i < exp; ++i)
            result *= value;
        return result;
    }
};

// ==================== 5. Template Template with Concepts ====================

template <template <typename...> class Container, typename T>
    requires std::ranges::input_range<Container<T>>
void print_container(const Container<T>& c)
{
    std::cout << "[";
    bool first = true;
    for (const auto& elem : c) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]\n";
}

// ==================== 6. Multi-constrained Templates ====================

template <typename T>
    requires Numeric<T> && Addable<T> && std::copyable<T>
T accumulate_twice(const std::vector<T>& vec)
{
    T sum{};
    for (const auto& v : vec)
        sum = sum + v + v;  // Addable ensures +
    return sum;
}

// ==================== 7. Concept-based Overload Resolution ====================

// More constrained overload preferred
template <typename T>
    requires std::integral<T>
std::string categorize(T)
{
    return "integral";
}

template <typename T>
    requires Numeric<T>
std::string categorize(T)
{
    return "numeric (but not integral)";
}

// ==================== 8. Constrained auto return type ====================

Numeric auto return_numeric()
{
    return 42;  // OK: int is Numeric
}

// ==================== 9. Concepts with Variadic Templates ====================

template <Numeric... Args>
auto sum_numeric(Args... args)
{
    return (args + ...);
}

template <typename... Args>
    requires (Numeric<Args> && ...)
auto sum_all_numeric(Args... args)
{
    return (args + ...);
}

// ============================================================================
// SAMPLE I/O:
// -----------
// square(5) = 25
// square(3.14) = 9.8596
// add_twice(10, 20) = 60
// triple(7) = 21
// print_numeric: 42
// add_abbrev(10, 20) = 30
// Calculator<int>.add(5) = 15
// Calculator<int>.multiply(3) = 30
// Calculator<int>.power(3) = 125
// print_container(vector{1,2,3}) = [1, 2, 3]
// accumulate_twice({1,2,3}) = 12
// categorize(42) = integral
// categorize(3.14) = numeric (but not integral)
// sum_numeric(1, 2, 3, 4, 5) = 15
// ============================================================================

int main()
{
    std::cout << "square(5) = "     << square(5)     << "\n";
    std::cout << "square(3.14) = "  << square(3.14)  << "\n";

    std::cout << "add_twice(10, 20) = " << add_twice(10, 20) << "\n";

    std::cout << "triple(7) = " << triple(7) << "\n";

    std::cout << "print_numeric: ";
    print_numeric(42);

    std::cout << "add_abbrev(10, 20) = " << add_abbrev(10, 20) << "\n";

    Calculator<int> calc{5};
    std::cout << "Calculator<int>.add(5) = "       << calc.add(5)       << "\n";
    std::cout << "Calculator<int>.multiply(3) = "  << calc.multiply(3)  << "\n";
    std::cout << "Calculator<int>.power(3) = "     << calc.power(3)     << "\n";

    print_container(std::vector{1, 2, 3});

    std::cout << "accumulate_twice({1,2,3}) = "
              << accumulate_twice(std::vector<int>{1, 2, 3}) << "\n";

    std::cout << "categorize(42) = "   << categorize(42)   << "\n";
    std::cout << "categorize(3.14) = " << categorize(3.14) << "\n";

    std::cout << "sum_numeric(1, 2, 3, 4, 5) = "
              << sum_numeric(1, 2, 3, 4, 5) << "\n";

    static_assert(Numeric<int>);
    static_assert(Numeric<double>);
    static_assert(!Numeric<std::string>);

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All concepts: O(1) compile-time evaluation
// Constrained templates: identical codegen to unconstrained templates
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: How do concepts affect template overload resolution?
// A1: Concepts use a partial ordering of constraints. If one template has
//     a strictly more constrained concept than another, the more constrained
//     one is preferred (subsumption). This replaces the SFINAE-based
//     enable_if ordering tricks.
//
// Q2: Can you use concepts with variadic templates?
// A2: Yes — use fold expressions with concepts: requires (Concept<Ts> && ...)
//     which requires ALL types to satisfy the concept. Or use (Concept<Ts> || ...)
//     for "any of".
//
// Q3: What does it mean for a concept to subsume another?
// A3: Concept A subsumes concept B if every type satisfying A also satisfies B.
//     The compiler can determine this for conjoined constraints (&&, ||).
//     When two templates match, the one with subsuming constraints is preferred
//     without ambiguity (unlike SFINAE overloading).
//
// COMMON MISTAKES:
// ----------------
// - Forgetting that concepts are compile-time predicates — cannot use runtime
//   values in requires expressions.
// - Using requires clause syntax incorrectly: requires Concept<T> vs
//   requires (Concept<T>) — both work but the parenthesized version allows
//   arbitrary boolean constant expressions.
// - Over-constraining types (too restrictive concepts) making templates
//   unusable for legitimate types.
// - Mixing concept-based constraints with SFINAE in the same template
//   (unnecessary — concepts subsume SFINAE use cases).
// ============================================================================
