// ============================================================================
// Phase 14 — Modern C++
// Topic 175: decltype – Type Deduction with decltype
// ============================================================================
//
// THEORY:
// --------
// decltype(expr) yields the exact declared type of an expression — including
// references and cv-qualifiers — without evaluating it (unevaluated context).
//
// decltype(auto) (C++14): Combines auto deduction with decltype semantics:
// the deduced type preserves references and qualifiers exactly.
//
// Two special rules:
//   - If expr is an unparenthesized id-expression or class member access,
//     decltype yields the declared type of the entity.
//   - If expr is any other expression (including parenthesized), decltype
//     yields T& for lvalues, T&& for xvalues, T for prvalues.
//
// decltype is used in:
//   - Return type deduction (forwarding functions, perfect forwarding)
//   - SFINAE / detection idiom
//   - result_of / invoke_result
//   - Declaring types dependent on expressions
//
// ============================================================================

#include <iostream>
#include <type_traits>
#include <vector>
#include <utility>   // declval

struct Widget
{
    int value;
    const int& getValue() const { return value; }
};

// ---------- 1. Basic decltype ----------
void demo_basic()
{
    int x = 42;
    const int& rx = x;

    // Unparenthesized id-expression -> declared type
    decltype(x)  a = x;     // int
    decltype(rx) b = x;     // const int&
    // decltype(rx) b2;     // ERROR: reference must be initialized

    // Parenthesized expression -> T& for lvalue
    decltype((x)) c = x;    // int& (x is lvalue, parenthesized)

    std::cout << std::boolalpha;
    std::cout << "a is int: "      << std::is_same_v<decltype(a), int> << "\n";
    std::cout << "b is const int&: "
              << std::is_same_v<decltype(b), const int&> << "\n";
    std::cout << "c is int&: "     << std::is_same_v<decltype(c), int&> << "\n";
}

// ---------- 2. decltype(auto) ----------
decltype(auto) forwarding_func(auto&& arg)
{
    return std::forward<decltype(arg)>(arg);
}

void demo_decltype_auto()
{
    int x = 10;
    // forwarding_func returns int& (perfect forwarding)
    decltype(auto) ref = forwarding_func(x);
    ref = 20;
    std::cout << "x after forwarding_func: " << x << "\n";  // 20

    // forwarding_func(42) returns int&& -> binds to int&&
    // int&& rref = forwarding_func(42);
}

// ---------- 3. decltype for return types ----------
template <typename Container, typename Index>
decltype(auto) get_at(Container& c, Index i)
{
    return c[i];    // preserves reference: std::vector::operator[] returns T&
}

void demo_return_type()
{
    std::vector<int> v = {10, 20, 30};
    get_at(v, 1) = 99;                  // modifies v[1]
    std::cout << "v[1] = " << v[1] << "\n";  // 99

    const auto& cv = v;
    // get_at(cv, 0) = 100;             // ERROR: returns const int&
}

// ---------- 4. decltype in unevaluated context ----------
struct S { int f(int) { return 0; } };

void demo_unevaluated()
{
    // std::declval<T>() produces T&& without constructing T
    using FnType = decltype(std::declval<S>().f(0));  // int

    // sizeof also unevaluated
    constexpr auto sz = sizeof(decltype(std::declval<S>().f(0)));  // sizeof(int)
    std::cout << "FnType is int: " << std::is_same_v<FnType, int> << "\n";
}

// ---------- 5. decltype with trailing return type ----------
template <typename T, typename U>
auto add(const T& a, const U& b) -> decltype(a + b)
{
    return a + b;
}

// ============================================================================
// SAMPLE I/O:
// -----------
// a is int: true
// b is const int&: true
// c is int&: true
// x after forwarding_func: 20
// v[1] = 99
// FnType is int: true
// add(3, 4.5) = 7.5
// ============================================================================

int main()
{
    demo_basic();
    demo_decltype_auto();
    demo_return_type();
    demo_unevaluated();
    std::cout << "add(3, 4.5) = " << add(3, 4.5) << "\n";
    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All decltype operations: O(1) compile-time
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What's the difference between auto and decltype(auto)?
// A1: auto decays (strips references and top-level cv). decltype(auto)
//     preserves the exact declared type including references. E.g.,
//     const int& f(); auto x = f();   // x is int
//     decltype(auto) y = f();         // y is const int&
//
// Q2: Why does decltype((x)) give int& but decltype(x) gives int?
// A2: C++ standard distinguishes: unparenthesized id-expressions yield the
//     declared type. Parenthesized expressions (including (x)) are general
//     expressions — lvalues yield T&.
//
// Q3: What are unevaluated contexts?
// A3: Contexts where the expression is not evaluated at runtime: decltype,
//     sizeof, noexcept, typeid, requires (concepts). These can safely use
//     std::declval<T>() without constructing T.
//
// COMMON MISTAKES:
// ----------------
// - Expecting decltype(x) and decltype((x)) to be the same type.
// - Using decltype(auto) on a function returning a local variable
//   (dangling reference!).
// - Forgetting that decltype(expr) does not evaluate expr — no side effects.
// - Using decltype without understanding that member access returns the
//   declared type of the member (not the expression type).
// ============================================================================
