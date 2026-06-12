// ============================================================================
// Phase 14 — Modern C++
// Topic 176: Type Traits – Compile-Time Type Introspection & Transformation
// ============================================================================
//
// THEORY:
// --------
// Type traits (C++11, expanded in C++14/17/20) are templates that provide
// compile-time information about types. They are the foundation of
// compile-time reflection and are heavily used in templates, SFINAE, and
// concepts.
//
// Categories:
//   - Primary type categories: std::is_void, std::is_integral,
//     std::is_floating_point, std::is_array, std::is_pointer, etc.
//   - Type relationships: std::is_same, std::is_base_of, std::is_convertible
//   - Type properties: std::is_const, std::is_reference, std::is_abstract
//   - Type transformations: std::remove_reference, std::add_pointer,
//     std::conditional, std::enable_if
//
// C++17 added _v aliases (is_integral_v) and C++20 added concepts.
//
// ============================================================================

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <list>
#include <map>

// ---------- 1. Primary Type Categories ----------
template <typename T>
void print_type_category()
{
    std::cout << std::boolalpha;
    std::cout << "  is_void:         " << std::is_void_v<T> << "\n";
    std::cout << "  is_integral:     " << std::is_integral_v<T> << "\n";
    std::cout << "  is_floating_point: " << std::is_floating_point_v<T> << "\n";
    std::cout << "  is_pointer:      " << std::is_pointer_v<T> << "\n";
    std::cout << "  is_array:        " << std::is_array_v<T> << "\n";
    std::cout << "  is_class:        " << std::is_class_v<T> << "\n";
}

// ---------- 2. Type Relationships ----------
template <typename T, typename U>
void check_relationship()
{
    std::cout << std::boolalpha;
    std::cout << "  is_same<T,U>:        " << std::is_same_v<T, U> << "\n";
    std::cout << "  is_convertible<T,U>: "
              << std::is_convertible_v<T, U> << "\n";
}

// ---------- 3. enable_if – SFINAE-based enable/disable ----------
// Enable this overload only for integral types.
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
twice(T x)
{
    std::cout << "  (integral overload) ";
    return x * 2;
}

// Enable this overload only for floating point types.
template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
twice(T x)
{
    std::cout << "  (float overload) ";
    return x * 2.0;
}

// ---------- 4. std::conditional – compile-time If-Then-Else ----------
template <typename T>
struct GetStorageType
{
    // Use larger type for precision if T is floating, else use T directly.
    using type = std::conditional_t<
        std::is_floating_point_v<T>,
        double,
        T
    >;
};

// ---------- 5. Type Transformations ----------
template <typename T>
void demo_transformations()
{
    using RawT = std::remove_cv_t<std::remove_reference_t<T>>;

    std::cout << "  original:   " << typeid(T).name() << "\n";
    std::cout << "  raw:        " << typeid(RawT).name() << "\n";
    std::cout << "  add_pointer:"
              << typeid(std::add_pointer_t<T>).name() << "\n";
    std::cout << "  add_const:  "
              << typeid(std::add_const_t<T>).name() << "\n";
}

// ---------- 6. Custom type trait ----------
template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

template <typename T>
constexpr bool is_vector_v = is_vector<T>::value;

// ---------- 7. void_t detection idiom (C++17) ----------
template <typename, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>>
    : std::true_type {};

// ============================================================================
// SAMPLE I/O:
// -----------
// int:
//   is_void:         false
//   is_integral:     true
//   is_floating_point: false
//   is_pointer:      false
//   is_array:        false
//   is_class:        false
// twice(5) = (integral overload) 10
// twice(3.14) = (float overload) 6.28
// Storage for int: int
// Storage for float: double
// is_vector<int>: false
// is_vector<vector<int>>: true
// has_value_type< vector<int> >: true
// has_value_type< int >: false
// ============================================================================

int main()
{
    std::cout << "int:\n";
    print_type_category<int>();

    std::cout << "\nstd::string:\n";
    print_type_category<std::string>();

    std::cout << "\nint*:\n";
    print_type_category<int*>();

    std::cout << "\nint[5]:\n";
    print_type_category<int[5]>();

    std::cout << "\n--- enable_if ---\n";
    std::cout << "twice(5) = "     << twice(5)   << "\n";
    std::cout << "twice(3.14) = "  << twice(3.14) << "\n";
    // twice("hello");  // ERROR: no matching function

    std::cout << "\n--- conditional ---\n";
    std::cout << "Storage for int:    "
              << typeid(GetStorageType<int>::type).name() << "\n";
    std::cout << "Storage for float:  "
              << typeid(GetStorageType<float>::type).name() << "\n";

    std::cout << "\n--- type transformations ---\n";
    std::cout << "const int&:\n";
    demo_transformations<const int&>();

    std::cout << "\n--- custom type traits ---\n";
    std::cout << "is_vector<int>: "            << is_vector_v<int> << "\n";
    std::cout << "is_vector<vector<int>>: "    << is_vector_v<std::vector<int>> << "\n";

    std::cout << "\n--- void_t detection ---\n";
    std::cout << "has_value_type<vector<int>>: "
              << has_value_type<std::vector<int>>::value << "\n";
    std::cout << "has_value_type<int>: "
              << has_value_type<int>::value << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All type traits: O(1) compile-time — no runtime overhead.
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What's the difference between std::enable_if and concepts?
// A1: enable_if is a type trait that enables/disables overloads via SFINAE;
//     concepts (C++20) provide cleaner, more readable constraints with
//     better error messages. concepts subsume most enable_if use cases.
//
// Q2: How do you create a custom type trait?
// A2: Inherit from std::true_type or std::false_type and specialize for
//     the types you want to detect. Use _v alias for convenience.
//
// Q3: What is void_t and how is it used?
// A3: std::void_t<T...> always maps to void. It's used in the detection
//     idiom: if a nested type (e.g., T::value_type) exists, the
//     specialization is valid (SFINAE succeeds); otherwise fallback is used.
//
// COMMON MISTAKES:
// ----------------
// - Forgetting _v and _t suffixes (C++17) — writing ::type or ::value.
// - Using enable_if in return type vs template parameter — both work but
//   template parameter is cleaner for multiple overloads.
// - Thinking type traits work at runtime — they are purely compile-time.
// - Forgetting to include <type_traits> header.
// ============================================================================
