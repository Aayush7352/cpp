// ============================================================================
// Phase 15 — Templates
// Topic 187: SFINAE – Substitution Failure Is Not An Error
// ============================================================================
//
// THEORY:
// --------
// SFINAE: When template argument substitution fails, the compiler doesn't
// emit an error — it simply removes that overload from the candidate set.
// If no overload remains, compilation fails.
//
// Key techniques:
//   - std::enable_if: conditionally enable/disable overloads
//   - void_t (C++17): detection idiom for type members
//   - decltype SFINAE: detect valid expressions
//   - Partial specialization with SFINAE
//
// C++20 concepts largely supersede SFINAE for most use cases.
//
// ============================================================================

#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <string>
#include <utility>

// ==================== 1. enable_if in return type ====================

// Only enabled for integral types
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
twice(T x)
{
    return x * 2;
}

// ==================== 2. enable_if in template parameter ====================

template <typename T,
          std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
T half(T x)
{
    return x / 2.0;
}

// ==================== 3. void_t detection idiom ====================

// Detect if a type has a nested ::value_type
template <typename T, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>>
    : std::true_type {};

// Detect if a type has a member function size()
template <typename T, typename = void>
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<decltype(std::declval<const T&>().size())>>
    : std::true_type {};

// ==================== 4. decltype SFINAE ====================

// Detect if T + T is valid (type has operator+)
template <typename T, typename = void>
struct is_addable : std::false_type {};

template <typename T>
struct is_addable<T,
    std::void_t<decltype(std::declval<const T&>() +
                         std::declval<const T&>())>>
    : std::true_type {};

// ==================== 5. SFINAE with partial specialization ====================

template <typename T, typename = void>
struct SmartPrinter
{
    static void print(const T& val)
    {
        std::cout << val;
    }
};

// Specialization for types that have a .c_str() method (std::string, etc.)
template <typename T>
struct SmartPrinter<T,
    std::void_t<decltype(std::declval<const T&>().c_str())>>
{
    static void print(const T& val)
    {
        std::cout << val.c_str();
    }
};

// Specialization for containers with begin/end
template <typename T>
struct SmartPrinter<T,
    std::void_t<decltype(std::begin(std::declval<const T&>())),
                decltype(std::end(std::declval<const T&>()))>>
{
    static void print(const T& val)
    {
        std::cout << "[";
        bool first = true;
        for (const auto& elem : val) {
            if (!first) std::cout << ", ";
            SmartPrinter<std::decay_t<decltype(elem)>>::print(elem);
            first = false;
        }
        std::cout << "]";
    }
};

// ==================== 6. Variadic SFINAE with enable_if ====================

template <typename T>
constexpr bool is_one_of() { return false; }

template <typename T, typename First, typename... Rest>
constexpr std::enable_if_t<!std::is_same_v<T, First>, bool>
is_one_of()
{
    return is_one_of<T, Rest...>();
}

template <typename T, typename First, typename... Rest>
constexpr std::enable_if_t<std::is_same_v<T, First>, bool>
is_one_of()
{
    return true;
}

// ==================== 7. SFINAE-friendly type trait ====================

// Check if a type supports .begin() and .end() (is iterable)
template <typename, typename = void>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T,
    std::void_t<decltype(std::begin(std::declval<T&>())),
                decltype(std::end(std::declval<T&>()))>>
    : std::true_type {};

template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

// ============================================================================
// SAMPLE I/O:
// -----------
// twice(5) = 10
// half(3.0) = 1.5
// has_value_type<vector<int>> = true
// has_value_type<int> = false
// has_size<vector<int>> = true
// has_size<int> = false
// is_addable<int> = true
// is_addable<std::vector<int>> = false
// SmartPrinter<string>: hello
// SmartPrinter<vector>: [1, 2, 3]
// is_one_of<int, double, char, int> = true
// is_iterable_v<vector<int>> = true
// is_iterable_v<int> = false
// ============================================================================

int main()
{
    std::cout << "twice(5) = " << twice(5) << "\n";
    // std::cout << half(5);  // ERROR: int is not floating point
    std::cout << "half(3.0) = " << half(3.0) << "\n";

    std::cout << std::boolalpha
              << "has_value_type<vector<int>> = "
              << has_value_type<std::vector<int>>::value << "\n"
              << "has_value_type<int> = "
              << has_value_type<int>::value << "\n";

    std::cout << "has_size<vector<int>> = "
              << has_size<std::vector<int>>::value << "\n"
              << "has_size<int> = "
              << has_size<int>::value << "\n";

    std::cout << "is_addable<int> = "
              << is_addable<int>::value << "\n"
              << "is_addable<vector<int>> = "
              << is_addable<std::vector<int>>::value << "\n";

    // SmartPrinter
    std::cout << "SmartPrinter<string>: ";
    SmartPrinter<std::string>::print("hello");
    std::cout << "\nSmartPrinter<vector>: ";
    SmartPrinter<std::vector<int>>::print({1, 2, 3});
    std::cout << "\n";

    std::cout << "is_one_of<int, double, char, int> = "
              << is_one_of<int, double, char, int>() << "\n";

    std::cout << "is_iterable_v<vector<int>> = "
              << is_iterable_v<std::vector<int>> << "\n";
    std::cout << "is_iterable_v<int> = "
              << is_iterable_v<int> << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All SFINAE checks: O(1) compile-time (constant template instantiation depth)
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What does SFINAE stand for and what does it mean?
// A1: Substitution Failure Is Not An Error. When template parameter
//     substitution produces invalid code (e.g., accessing a non-existent
//     member), the compiler doesn't fail — it removes that overload from
//     consideration and tries others.
//
// Q2: How does void_t enable the detection idiom?
// A2: void_t<T...> always maps to void. When we write
//     std::void_t<typename T::value_type>, the expression is valid only if
//     T::value_type exists. If it does, the partial specialization matches;
//     if not, SFINAE kicks in and the primary template (false_type) is used.
//
// Q3: What replaced SFINAE in C++20?
// A3: Concepts (requires clauses and requires expressions) provide cleaner,
//     more readable constraints with better diagnostics. However, SFINAE is
//     still useful for type traits and detection in C++17 codebases.
//
// COMMON MISTAKES:
// ----------------
// - Using enable_if in both return type and template parameter
//   (unnecessary duplication).
// - Forgetting the = 0 default value in enable_if template parameter.
// - Not understanding that SFINAE only applies during immediate context
//   of template argument substitution (not in the body of the function).
// - Overcomplicating with SFINAE when a simple static_if or constexpr
//   would work (C++17 if constexpr).
// ============================================================================
