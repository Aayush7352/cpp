// ============================================================================
// Phase 14 — Modern C++
// Topic 174: auto – Type Deduction Deep Dive
// ============================================================================
//
// THEORY:
// --------
// auto type deduction follows the same rules as template type deduction
// (with one exception: std::initializer_list handling).
//
// Three cases for template/auto deduction:
//   1. ParamType is a reference or pointer (not universal reference)
//   2. ParamType is a universal reference (T&&)
//   3. ParamType is neither reference nor pointer (by value)
//
// auto&  → lvalue reference deduction
// auto&& → forwarding reference (deduces T& for lvalues, T for rvalues)
//
// KEY DIFFERENCES from templates:
// - auto deduces std::initializer_list from braced init list {1, 2, 3}
// - Template parameter T doesn't — this is the only difference
//
// ============================================================================

#include <iostream>
#include <type_traits>
#include <vector>
#include <map>
#include <utility>

// Helper to print deduced type info
#define SHOW_TYPE(var)                                                    \
    std::cout << #var << " -> "                                           \
              << (std::is_lvalue_reference_v<decltype(var)> ? "lref" :    \
                  std::is_rvalue_reference_v<decltype(var)> ? "rref" :    \
                  "value")                                                \
              << (std::is_const_v<std::remove_reference_t<decltype(var)>> \
                  ? " const" : "") << "\n"

// ---------- 1. auto by value (case 3: template-like, by value) ----------
void demo_auto_value()
{
    int x = 42;
    const int cx = x;
    const int& crx = x;

    auto a1 = x;      // int (top-level const/ref dropped)
    auto a2 = cx;     // int
    auto a3 = crx;    // int
    auto a4 = 42;     // int

    SHOW_TYPE(a1);
}

// ---------- 2. auto& (case 1: lvalue reference) ----------
void demo_auto_ref()
{
    int x = 42;
    const int cx = x;
    const int& crx = x;

    auto& r1 = x;     // int&
    auto& r2 = cx;    // const int&
    auto& r3 = crx;   // const int&
    // auto& r4 = 42; // ERROR — cannot bind lvalue ref to rvalue

    SHOW_TYPE(r1);
}

// ---------- 3. auto&& (case 2: forwarding/universal reference) ----------
void demo_auto_forwarding()
{
    int x = 42;
    const int cx = x;
    const int& crx = x;

    auto&& ur1 = x;    // int&         (lvalue → T = int& → auto&& = int&)
    auto&& ur2 = cx;   // const int&
    auto&& ur3 = crx;  // const int&
    auto&& ur4 = 42;   // int&&        (rvalue → T = int → auto&& = int&&)

    SHOW_TYPE(ur1);
}

// ---------- 4. auto with initializer list (unique to auto) ----------
void demo_init_list()
{
    // This is the only case where auto differs from template deduction
    auto il = {1, 2, 3};           // std::initializer_list<int>
    // template<typename T> void f(T p); f({1,2,3}); // ERROR: doesn't compile

    // auto a = {1, 2, 3.0};       // ERROR — inconsistent types in il

    std::cout << "initializer_list size = " << il.size() << "\n";
}

// ---------- 5. Structured bindings with auto ----------
void demo_structured_bindings()
{
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}};
    for (const auto& [key, value] : m)
    {
        std::cout << key << " -> " << value << "\n";
    }
}

// ---------- 6. auto in lambda parameters (C++14) ----------
void demo_lambda_auto()
{
    auto add = [](auto&& a, auto&& b) -> decltype(auto)
    {
        return std::forward<decltype(a)>(a) +
               std::forward<decltype(b)>(b);
    };
    std::cout << "add(3, 4) = " << add(3, 4) << "\n";
    std::cout << "add(1.5, 2.5) = " << add(1.5, 2.5) << "\n";
}

// ============================================================================
// SAMPLE I/O:
// -----------
// a1 -> value
// ur1 -> lref
// initializer_list size = 3
// 1 -> one
// 2 -> two
// add(3, 4) = 7
// add(1.5, 2.5) = 4
// ============================================================================

int main()
{
    demo_auto_value();
    demo_auto_ref();
    demo_auto_forwarding();
    demo_init_list();
    demo_structured_bindings();
    demo_lambda_auto();
    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// auto deduction: O(1) compile-time
// All operations: trivial
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What's the difference between auto and template argument deduction?
// A1: Only one difference: auto deduces std::initializer_list<T> from a
//     braced init list (auto x = {1,2,3}), while template deduction fails.
//     All other rules are identical.
//
// Q2: What does decltype(auto) do?
// A2: It uses decltype rules for auto: preserves references and qualifiers
//     exactly as declared, rather than removing them (auto strips
//     references and cv-qualifiers at top level).
//
// Q3: When should I use auto&&?
// A3: In forwarding contexts (template code, range-for with unknown
//     containers). auto&& binds to anything — lvalue becomes T&, rvalue
//     becomes T&&. Perfect for generic lambdas and range-based for loops
//     where you want to preserve value category.
//
// COMMON MISTAKES:
// ----------------
// - Expecting auto to preserve const/ref (auto x = cref; → x is value, not ref).
// - Misunderstanding auto&&: it's a forwarding reference, not an rvalue ref.
// - Using auto with braced init lists and expecting vector<T> instead of
//   initializer_list.
// - Forgetting that auto& cannot bind to rvalues (auto& x = 42; // ERROR).
// ============================================================================
