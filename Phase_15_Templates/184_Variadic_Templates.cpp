// ============================================================================
// Phase 15 — Templates
// Topic 184: Variadic Templates
// ============================================================================
//
// THEORY:
// --------
// Variadic templates (C++11) accept an arbitrary number of template
// arguments using parameter packs (typename... Args).
//
// Key concepts:
//   - Parameter pack: template<typename... Args> or template<int... Ns>
//   - Pack expansion: Args... expands the pack
//   - sizeof...(Args) gives the number of arguments
//   - Recursive instantiation: process one element, recurse on the rest
//   - Fold expressions (C++17) often replace recursion
//     (see 185_Fold_Expressions.cpp)
//
// ============================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

// ---------- 1. Basic variadic template (recursive) ----------
// Recursive case: process first argument, recurse on rest.
void print() {}  // base case — no arguments

template <typename T, typename... Args>
void print(const T& first, const Args&... rest)
{
    std::cout << first;
    if constexpr (sizeof...(rest) > 0)
        std::cout << ", ";
    print(rest...);  // recursive call with remaining pack
}

// ---------- 2. Variadic template with non-type parameters ----------
template <int... Ns>
struct Sum
{
    static constexpr int value = 0;
};

template <int First, int... Rest>
struct Sum<First, Rest...>
{
    static constexpr int value = First + Sum<Rest...>::value;
};

// ---------- 3. Type-safe printf using variadic templates ----------
void safe_print(const char* format)
{
    std::cout << format;
}

template <typename T, typename... Args>
void safe_print(const char* format, const T& value, const Args&... args)
{
    while (*format)
    {
        if (*format == '%' && *(format + 1) == '%')
        {
            std::cout << value;
            safe_print(format + 2, args...);
            return;
        }
        std::cout << *format;
        ++format;
    }
}

// ---------- 4. Pack expansion in different contexts ----------
// Expanding in function arguments
template <typename... Args>
double sum_all(const Args&... args)
{
    return (args + ...);  // fold expression (C++17)
}

// Expanding in initializer list
template <typename... Args>
std::string concat_all(const Args&... args)
{
    std::ostringstream oss;
    ((oss << args), ...);  // comma fold
    return oss.str();
}

// Expanding in template argument
template <typename... Args>
struct TypeList {};

// ---------- 5. Variadic template with inheritance ----------
template <typename... Bases>
struct MultiDerived : public Bases...
{
    MultiDerived(const Bases&... bases) : Bases(bases)... {}
};

struct A { int a = 1; };
struct B { int b = 2; };
struct C { int c = 3; };

// ---------- 6. sizeof... operator ----------
template <typename... Args>
void count_args()
{
    std::cout << "Number of args: " << sizeof...(Args) << "\n";
}

// ---------- 7. Perfect forwarding with variadic templates ----------
template <typename T, typename... Args>
T make_object(Args&&... args)
{
    return T(std::forward<Args>(args)...);
}

// ============================================================================
// SAMPLE I/O:
// -----------
// print: 1, hello, 3.14, world
// Sum<1, 2, 3, 4, 5>::value = 15
// safe_print with args: Value is 42 and pi is 3.14
// sum_all(1, 2, 3, 4, 5) = 15
// concat_all("Hello", " ", "World") = Hello World
// Number of args: 0
// Number of args: 3
// MultiDerived values: 1 2 3
// make_object<string>(5, 'A') = AAAAA
// ============================================================================

int main()
{
    // Recursive print
    std::cout << "print: ";
    print(1, "hello", 3.14, "world");
    std::cout << "\n";

    // Non-type pack
    std::cout << "Sum<1, 2, 3, 4, 5>::value = "
              << Sum<1, 2, 3, 4, 5>::value << "\n";

    // Type-safe printf
    std::cout << "safe_print with args: ";
    safe_print("Value is %% and pi is %%\n", 42, 3.14);

    // Fold expression
    std::cout << "sum_all(1, 2, 3, 4, 5) = "
              << sum_all(1, 2, 3, 4, 5) << "\n";

    // Comma fold
    std::cout << "concat_all(\"Hello\", \" \", \"World\") = "
              << concat_all("Hello", " ", "World") << "\n";

    // sizeof...
    count_args<>();
    count_args<int, double, char>();

    // Variadic inheritance
    MultiDerived<A, B, C> md{A{}, B{}, C{}};
    std::cout << "MultiDerived values: "
              << md.a << ' ' << md.b << ' ' << md.c << "\n";

    // Perfect forwarding
    auto s = make_object<std::string>(5, 'A');
    std::cout << "make_object<string>(5, 'A') = " << s << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// Variadic recursion: O(N) compile-time instantiation depth (recursive)
// Fold expressions: O(N) compile-time expansion (no recursion depth issue)
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What is a parameter pack?
// A1: A parameter pack (typename... Args) represents zero or more template
//     arguments. It is expanded using Args... in various contexts.
//
// Q2: How do variadic templates differ from varargs (C-style ...)?
// A2: Variadic templates are type-safe — each argument retains its type.
//     C varargs (...) are not type-safe; all arguments undergo default
//     promotions. Variadic templates also allow perfect forwarding,
//     arbitrary types, and compile-time processing.
//
// Q3: What is sizeof... used for?
// A3: Returns the number of elements in a parameter pack as a compile-time
//     constant. Useful for base cases and conditional logic.
//
// COMMON MISTAKES:
// ----------------
// - Forgetting the base case in recursive variadic templates (infinite
//   recursion at compile time).
// - Expanding the pack in the wrong context (needs ... after the pattern).
// - Not using std::forward for forwarding references in variadic templates.
// - Confusing pack expansion order (left-to-right, but unspecified
//   evaluation order of function arguments).
// ============================================================================
