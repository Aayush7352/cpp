// ============================================================================
// Phase 15 — Templates
// Topic 181: Function Templates
// ============================================================================
//
// THEORY:
// --------
// Function templates define a family of functions parameterized by types
// (or values). The compiler instantiates a concrete function for each set
// of template arguments.
//
// Type deduction: compiler deduces template arguments from function arguments.
//   template <typename T> T max(T a, T b);
//   max(3, 7)       → T = int
//   max(3.0, 2.5)   → T = double
//   max(3, 7.0)     → ERROR: ambiguous (T could be int or double)
//
// Explicit specialization: override the template for specific types.
// Overloading: multiple function templates/same name, resolved by overload
//   resolution.
//
// ============================================================================

#include <iostream>
#include <string>
#include <cstring>
#include <type_traits>

// ---------- 1. Basic function template ----------
template <typename T>
T max_value(T a, T b)
{
    return (a > b) ? a : b;
}

// ---------- 2. Multiple template parameters ----------
template <typename T, typename U>
auto max_mixed(const T& a, const U& b) -> decltype(a + b)
{
    return (a > b) ? a : b;
}

// ---------- 3. Non-type template parameters ----------
template <typename T, int N>
T multiply_by(T x)
{
    return x * N;
}

// ---------- 4. Explicit specialization ----------
// Primary template
template <typename T>
const char* describe()
{
    return "unknown";
}

// Full specialization for int
template <>
const char* describe<int>()
{
    return "integer";
}

// Full specialization for double
template <>
const char* describe<double>()
{
    return "double";
}

// Specialization of function template for const char*
template <>
const char* max_value(const char* a, const char* b)
{
    return (std::strcmp(a, b) > 0) ? a : b;
}

// ---------- 5. Overloading function templates ----------
// Overload 1: integral types
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
twice(T x)
{
    std::cout << "  (integral) ";
    return x * 2;
}

// Overload 2: floating point types
template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
twice(T x)
{
    std::cout << "  (floating) ";
    return x * 2;
}

// ---------- 6. Template auto parameters (C++17) ----------
template <auto N>
auto constexpr_value()
{
    return N;
}

// ---------- 7. Abbreviated function template (C++20) ----------
auto add_concept(auto a, auto b)
{
    return a + b;
}

// ============================================================================
// SAMPLE I/O:
// -----------
// max_value(10, 20) = 20
// max_value(3.14, 2.72) = 3.14
// max_mixed(10, 5.5) = 10
// multiply_by<int, 5>(3) = 15
// describe<int>() = integer
// describe<double>() = double
// describe<char>() = unknown
// max_value("apple", "zebra") = zebra
// twice(5) = (integral) 10
// twice(3.14) = (floating) 6.28
// constexpr_value<42>() = 42
// add_concept(3, 4) = 7
// ============================================================================

int main()
{
    // Basic
    std::cout << "max_value(10, 20) = "       << max_value(10, 20)   << "\n";
    std::cout << "max_value(3.14, 2.72) = "   << max_value(3.14, 2.72) << "\n";
    std::cout << "max_mixed(10, 5.5) = "      << max_mixed(10, 5.5)  << "\n";

    // Non-type
    std::cout << "multiply_by<int, 5>(3) = "  << multiply_by<int, 5>(3) << "\n";

    // Specialization
    std::cout << "describe<int>() = "    << describe<int>()    << "\n";
    std::cout << "describe<double>() = " << describe<double>() << "\n";
    std::cout << "describe<char>() = "   << describe<char>()   << "\n";

    // Specialization for const char*
    std::cout << "max_value(\"apple\", \"zebra\") = "
              << max_value("apple", "zebra") << "\n";

    // Overload resolution with enable_if
    std::cout << "twice(5) = "   << twice(5)   << "\n";
    std::cout << "twice(3.14) = "<< twice(3.14) << "\n";

    // auto template parameter
    std::cout << "constexpr_value<42>() = "
              << constexpr_value<42>() << "\n";

    // Abbreviated function template (C++20)
    std::cout << "add_concept(3, 4) = " << add_concept(3, 4) << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All functions: O(1) runtime, instantiation is compile-time O(N) for N uses
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: How does template argument deduction work?
// A1: The compiler compares function parameter types with argument types
//     to determine template parameters. References are stripped unless
//     the parameter is a forwarding reference (T&&).
//
// Q2: What's the difference between specialization and overloading?
// A2: Overloading adds a new function template/function with the same name;
//     overload resolution picks the best match. Specialization customizes
//     a specific template for a specific type — it's still the same
//     template. Overloads participate in name resolution; specializations
//     are only considered after the primary template is selected.
//
// Q3: When would you use a non-type template parameter?
// A3: For compile-time constants: array sizes, buffer sizes, dimension
//     parameters for math types, policy choices (e.g., threading model).
//
// COMMON MISTAKES:
// ----------------
// - Providing too many/few template arguments explicitly.
// - Mixing types in arguments causing deduction failure.
// - Forgetting that specialization is not overloading — can't add new
//   parameters or change signatures arbitrarily.
// - Not marking specialization with template<> properly.
// ============================================================================
