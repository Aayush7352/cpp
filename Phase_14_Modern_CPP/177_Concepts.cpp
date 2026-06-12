// ============================================================================
// Phase 14 — Modern C++
// Topic 177: Concepts – C++20 Compile-Time Constraints
// ============================================================================
//
// THEORY:
// --------
// Concepts (C++20) specify constraints on template parameters as named
// boolean predicates evaluated at compile time. They replace SFINAE for
// most use cases, providing clearer syntax and better error messages.
//
// Syntax forms:
//   1. template <Concept T> void f();          — abbreviated
//   2. template <typename T> requires Concept<T> — requires clause
//   3. template <typename T> Concept auto f();  — constrained auto
//   4. auto f(Concept auto x);                  — abbreviated function template
//
// Standard concepts (<concepts>):
//   std::integral, std::floating_point, std::signed_integral,
//   std::unsigned_integral, std::same_as, std::derived_from,
//   std::convertible_to, std::common_with, std::invocable,
//   std::predicate, std::equality_comparable, std::totally_ordered,
//   std::copyable, std::movable, std::assignable_from, etc.
//
// ============================================================================

#include <iostream>
#include <concepts>
#include <type_traits>
#include <vector>
#include <list>
#include <string>

// ---------- 1. Defining a concept ----------
template <typename T>
concept Printable = requires(T t) {
    std::cout << t;           // expression must be valid
};

template <typename T>
concept HasSize = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

// ---------- 2. Compound concept ----------
template <typename T>
concept Container = requires(T t, const T ct) {
    typename T::value_type;              // nested type check
    typename T::iterator;
    typename T::const_iterator;
    { t.begin() } -> std::same_as<typename T::iterator>;
    { ct.begin() } -> std::same_as<typename T::const_iterator>;
    { t.end() }   -> std::same_as<typename T::iterator>;
    { ct.end() }  -> std::same_as<typename T::const_iterator>;
    { t.size() }  -> std::convertible_to<std::size_t>;
};

// ---------- 3. Using requires clause ----------
template <typename T>
    requires std::integral<T>
T add_one(T x)
{
    return x + 1;
}

// ---------- 4. Abbreviated template syntax (constrained auto) ----------
void print_value(const Printable auto& p)
{
    std::cout << p << "\n";
}

// ---------- 5. Trailing requires clause ----------
template <typename T>
T twice(T x) requires std::integral<T> || std::floating_point<T>
{
    return x * 2;
}

// ---------- 6. Requires expression (inline) ----------
template <typename T>
constexpr bool is_addable_v = requires(T a, T b) {
    a + b;               // just check validity
};

template <typename T>
    requires is_addable_v<T>
auto sum(const T& a, const T& b)
{
    return a + b;
}

// ---------- 7. Combining concepts ----------
template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <Arithmetic T>
T multiply(T a, T b)
{
    return a * b;
}

// ---------- 8. Standard concepts library ----------
template <std::copyable T>
void demo_copyable(const T& val)
{
    T copy = val;   // guaranteed to work
    std::cout << "Copyable: " << copy << "\n";
}

template <std::equality_comparable T>
bool are_equal(const T& a, const T& b)
{
    return a == b;
}

// ============================================================================
// SAMPLE I/O:
// -----------
// add_one(5) = 6
// 3.14
// twice(5) = 10
// twice(3.14) = 6.28
// sum(10, 20) = 30
// multiply(6, 7) = 42
// Copyable: 42
// are_equal(1, 2) = false
// ============================================================================

int main()
{
    // requires clause
    std::cout << "add_one(5) = " << add_one(5) << "\n";
    // add_one(3.14);  // ERROR: not integral

    // constrained auto
    print_value(3.14);
    print_value("Hello");
    // print_value(std::vector<int>{});  // ERROR: not Printable (vector has no operator<<)

    // trailing requires
    std::cout << "twice(5) = "     << twice(5)   << "\n";
    std::cout << "twice(3.14) = "  << twice(3.14) << "\n";

    // requires expression
    std::cout << "sum(10, 20) = "  << sum(10, 20) << "\n";

    // combined concept
    std::cout << "multiply(6, 7) = " << multiply(6, 7) << "\n";

    // standard concepts
    demo_copyable(42);
    std::cout << "are_equal(1, 2) = " << std::boolalpha
              << are_equal(1, 2) << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// Concepts: O(1) compile-time evaluation.
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: Concepts vs SFINAE — when to use which?
// A1: Prefer concepts for new code: they are cleaner, have better error
//     messages, and are easier to maintain. SFINAE is still needed for
//     C++17 and earlier, or for detection idiom with void_t.
//
// Q2: What's a requires expression vs requires clause?
// A2: A requires clause (requires (T t) { ... }) checks the validity of
//     expressions at compile time. A requires clause is the
//     requires keyword after template parameters (or on a function) that
//     constrains the template using a concept or boolean expression.
//
// Q3: Can a concept be recursive?
// A3: Yes, but with care to avoid infinite recursion at compile time.
//     Recursive concepts are evaluated with short-circuit logic.
//
// COMMON MISTAKES:
// ----------------
// - Mismatched order of requires clause (before or after function
//   declaration) leading to syntax errors.
// - Forgetting that concepts are compile-time predicates — no runtime cost.
// - Over-constraining templates with overly restrictive concepts.
// - Writing requires(T t) { {expr} -> Concept } — note the braces and arrow.
// ============================================================================
