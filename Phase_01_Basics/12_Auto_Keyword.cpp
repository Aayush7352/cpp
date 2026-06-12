/*
 * ====================================================================
 * Program 12: The auto Keyword
 *
 * THEORY:
 * `auto` asks the compiler to deduce the type from the initializer.
 *
 *   auto          — deduces by value (copies, strips top-level cv-quals)
 *   auto&         — deduces as lvalue reference
 *   const auto    — deduces as const value
 *   const auto&   — deduces as const lvalue reference
 *   auto&&        — forwarding reference (deduces as T& for lvalues,
 *                   T&& for rvalues) — perfect forwarding
 *
 *   decltype(auto) — deduces the exact type of the expression
 *                    (preserves references and cv-qualifiers)
 *
 * RETURN TYPE DEDUCTION (C++14):
 *   auto function() { return expr; }
 *   The compiler deduces the return type from the return statement.
 *
 * TRAILING RETURN TYPES (C++11):
 *   auto function() -> decltype(expr) { ... }
 *   Useful when the return type depends on template parameters.
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output demonstrates type deduction.
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: auto vs decltype(auto) — what's the difference?
 *   A: auto strips references and top-level cv-quals; decltype(auto)
 *      preserves the exact type including references.
 *   Q: Can auto be used for function parameters?
 *   A: Before C++20, no (use templates). In C++20, auto parameters
 *      are allowed (abbreviated function templates).
 *   Q: What is a forwarding reference (auto&&)?
 *   A: auto&& in a range-for or lambda parameter binds to both
 *      lvalues and rvalues (perfect forwarding).
 *
 * COMMON MISTAKES:
 *   - Using auto when a reference is needed (unnecessary copy).
 *   - Using auto& with rvalues (won't compile).
 *   - Forgetting that auto strips const/volatile/reference.
 *   - Using decltype(auto) with a parenthesized expression returns
 *     a reference type even for local variables.
 * ====================================================================
 */

#include <iostream>
#include <type_traits>
#include <vector>
#include <string>

// ---- Return type deduction (C++14) ----
auto add(int a, int b)
{
    return a + b;  // deduces int
}

// ---- Trailing return type (C++11) ----
template <typename T, typename U>
auto multiply(T a, U b) -> decltype(a * b)
{
    return a * b;
}

// ---- decltype(auto) ----
decltype(auto) get_value(int& x)
{
    return x;            // deduces int&
}

int main()
{
    std::cout << "=== Basic auto deduction ===\n";
    auto a      = 42;       // int
    auto b      = 3.14;     // double
    auto c      = 'Z';      // char
    auto d      = true;     // bool
    auto e      = "hello";  // const char*
    auto f      = std::string{"world"};  // std::string

    std::cout << "a (int):        " << a << '\n';
    std::cout << "b (double):     " << b << '\n';
    std::cout << "c (char):       " << c << '\n';
    std::cout << "d (bool):       " << d << '\n';
    std::cout << "e (const char*): " << e << '\n';
    std::cout << "f (string):     " << f << '\n';

    // ---- auto avoids copying ----
    std::cout << "\n=== auto& and const auto& ===\n";
    std::vector<int> vec = {10, 20, 30};

    // auto — copy (expensive for large objects)
    for (auto v : vec)
        v = 0;  // modifies the copy, not the original

    // auto& — reference (no copy, can modify)
    for (auto& v : vec)
        v *= 2;

    // const auto& — reference (no copy, read-only)
    std::cout << "vec after auto&: ";
    for (const auto& v : vec)
        std::cout << v << ' ';
    std::cout << '\n';

    // ---- auto&& (forwarding reference) ----
    std::cout << "\n=== auto&& ===\n";
    int x = 42;
    auto&& r1 = x;         // int&  (lvalue)
    auto&& r2 = 100;       // int&& (rvalue)

    std::cout << "auto&& r1 (lvalue ref): " << r1 << '\n';
    std::cout << "auto&& r2 (rvalue ref): " << r2 << '\n';

    // ---- decltype(auto) ----
    std::cout << "\n=== decltype(auto) ===\n";
    int val = 100;
    int& ref = val;
    decltype(auto) result = get_value(val);  // int&
    result = 200;
    std::cout << "After decltype(auto) write: val = " << val << '\n';

    // ---- auto return type deduction ----
    std::cout << "\n=== Return type deduction ===\n";
    auto sum = add(10, 20);
    std::cout << "add(10, 20) = " << sum << '\n';

    // ---- Trailing return type ----
    auto product = multiply(5, 3.5);
    std::cout << "multiply(5, 3.5) = " << product << '\n';

    // ---- Type traits to inspect ----
    std::cout << "\n=== Type introspection ===\n";
    std::cout << "decltype(42) is int: "
              << std::is_same_v<decltype(42), int> << '\n';
    std::cout << "decltype(3.14) is double: "
              << std::is_same_v<decltype(3.14), double> << '\n';

    return 0;
}
