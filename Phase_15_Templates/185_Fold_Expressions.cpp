// ============================================================================
// Phase 15 — Templates
// Topic 185: Fold Expressions (C++17)
// ============================================================================
//
// THEORY:
// --------
// Fold expressions (C++17) reduce a parameter pack over a binary operator.
// They eliminate recursive template instantiation for common operations.
//
// Four forms:
//   Unary right fold:  (pack op ...)  →  E1 op (E2 op (E3 op E4))
//   Unary left fold:   (... op pack)  →  ((E1 op E2) op E3) op E4
//   Binary right fold: (pack op ... op init)  →  E1 op (E2 op (E3 op init))
//   Binary left fold:  (init op ... op pack)  →  ((init op E1) op E2) op E3
//
// Supported operators: +, -, *, /, %, ^, &, |, <<, >>, &&, ||, , (comma),
// ==, !=, <, >, <=, >=, = (assignment), +=, etc.
//
// Empty pack behavior:
//   Unary && → true, unary || → false, unary , → void()
//   Other unary ops on empty pack are ill-formed.
//
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

// ---------- 1. Unary right fold (sum) ----------
template <typename... Args>
auto sum_right(Args... args)
{
    return (args + ...);    // right fold: a + (b + (c + ...))
}

// ---------- 2. Unary left fold (sum) ----------
template <typename... Args>
auto sum_left(Args... args)
{
    return (... + args);    // left fold: ((a + b) + c) + ...
}

// ---------- 3. Binary left fold (with initial value) ----------
template <typename... Args>
auto sum_with_init(Args... args)
{
    return (0 + ... + args);  // binary left fold: (((0 + a) + b) + c)
}

// ---------- 4. Logical AND fold (all true?) ----------
template <typename... Args>
bool all_true(Args... args)
{
    return (... && args);   // left fold: ((a && b) && c) && ...
}

// ---------- 5. Logical OR fold (any true?) ----------
template <typename... Args>
bool any_true(Args... args)
{
    return (... || args);   // left fold: ((a || b) || c) || ...
}

// ---------- 6. Comma fold (execute for each argument) ----------
template <typename... Args>
void print_all(const Args&... args)
{
    // Comma fold — left fold with comma operator
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

// ---------- 7. Comma fold with side effects ----------
template <typename... Args>
void push_all(std::vector<int>& v, Args... args)
{
    (v.push_back(args), ...);   // expand with comma
}

// ---------- 8. Fold with custom operator ----------
template <typename... Args>
auto min_all(Args... args)
{
    // Binary left fold using ternary operator
    return (args < ...);  // NOT valid — use initial value
}

// Helper: min using binary fold with init
template <typename T, typename... Args>
T min_first(T first, Args... args)
{
    // Binary left fold with std::min
    return (first < ... < args);  // NO — fold only works with specific ops
}

// Correct min with fold
template <typename T>
T min_of(T val) { return val; }

template <typename T, typename... Args>
T min_of(T first, Args... args)
{
    // NOT a fold, but the simplest approach
    T result = first;
    ((result = (args < result ? args : result)), ...);
    return result;
}

// ---------- 9. Fold over comparison ----------
template <typename... Args>
bool are_all_equal(Args... args)
{
    return (... && (args == (args, ...)));  // tricky — use binary
}

// Simpler: check all equal to first
template <typename T, typename... Args>
bool all_equal_to_first(T first, Args... args)
{
    return (... && (args == first));
}

// ---------- 10. String concatenation with fold ----------
template <typename... Args>
std::string concat(Args... args)
{
    std::string result;
    ((result += std::to_string(args) + " "), ...);
    return result;
}

// ============================================================================
// SAMPLE I/O:
// -----------
// sum_right(1, 2, 3, 4, 5) = 15
// sum_left(1, 2, 3, 4, 5) = 15
// sum_with_init(1, 2, 3) = 6
// all_true(true, true, false) = false
// any_true(false, false, true) = true
// print_all: hello 42 3.14 world
// push_all: 1 2 3 4 5
// min_of(5, 2, 8, 1, 9) = 1
// all_equal_to_first(42, 42, 42) = true
// all_equal_to_first(42, 43, 42) = false
// concat(1, 2, 3) = 1 2 3
// ============================================================================

int main()
{
    std::cout << "sum_right(1, 2, 3, 4, 5) = "
              << sum_right(1, 2, 3, 4, 5) << "\n";
    std::cout << "sum_left(1, 2, 3, 4, 5) = "
              << sum_left(1, 2, 3, 4, 5) << "\n";
    std::cout << "sum_with_init(1, 2, 3) = "
              << sum_with_init(1, 2, 3) << "\n";

    std::cout << "all_true(true, true, false) = "
              << std::boolalpha << all_true(true, true, false) << "\n";
    std::cout << "any_true(false, false, true) = "
              << any_true(false, false, true) << "\n";

    std::cout << "print_all: ";
    print_all("hello", 42, 3.14, "world");

    std::vector<int> vec;
    push_all(vec, 1, 2, 3, 4, 5);
    std::cout << "push_all:";
    for (int x : vec) std::cout << ' ' << x;
    std::cout << "\n";

    std::cout << "min_of(5, 2, 8, 1, 9) = "
              << min_of(5, 2, 8, 1, 9) << "\n";

    std::cout << "all_equal_to_first(42, 42, 42) = "
              << all_equal_to_first(42, 42, 42) << "\n";
    std::cout << "all_equal_to_first(42, 43, 42) = "
              << all_equal_to_first(42, 43, 42) << "\n";

    std::cout << "concat(1, 2, 3) = " << concat(1, 2, 3) << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All fold expressions: O(N) compile-time expansion, O(N) runtime
// (no recursive template instantiation)
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What problem do fold expressions solve?
// A1: They eliminate recursive template instantiation for operations on
//     parameter packs. Before C++17, you needed recursive variadic
//     templates for sum/print/any/all; folds do it in a single expression.
//
// Q2: What's the difference between left fold and right fold?
// A2: Left fold: ((a + b) + c) + d  — associates left-to-right.
//     Right fold: a + (b + (c + d)) — associates right-to-left.
//     For commutative operators (+, *, &&, ||) they are equivalent.
//     For non-commutative (-, /) the result differs.
//
// Q3: What happens with an empty parameter pack?
// A3: For && → true, || → false, comma → void(). For other operators
//     (+, -, *, etc.) an empty pack is ill-formed. Binary folds with an
//     initial value don't have this problem.
//
// COMMON MISTAKES:
// ----------------
// - Using fold with unsupported operators (like <, >, <=, >= for
//   chaining — they don't mean what you expect).
// - Forgetting that unary fold on empty pack for + is ill-formed.
// - Thinking fold is a runtime feature (it's compile-time expansion).
// - Mixing left/right fold when associativity matters for non-commutative
//   operators.
// ============================================================================
