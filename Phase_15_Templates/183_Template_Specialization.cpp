// ============================================================================
// Phase 15 — Templates
// Topic 183: Template Specialization
// ============================================================================
//
// THEORY:
// --------
// Template specialization provides custom implementations for specific
// template arguments. Two kinds:
//
//   Full specialization: template<> struct Name<SpecificType> { ... };
//     All template parameters are fixed.
//
//   Partial specialization: template<T, U> struct Name<T*, U> { ... };
//     Some parameters remain generic (classes only — functions cannot
//     be partially specialized).
//
// Function specialization is done via full specialization only. For
// function overloads, prefer overloading over specialization.
//
// ============================================================================

#include <iostream>
#include <type_traits>
#include <cstring>

// ==================== Full Specialization ====================

// ---------- 1. Primary template ----------
template <typename T>
struct TypeInfo
{
    static constexpr const char* name = "unknown";
    static T defaultValue() { return T{}; }
};

// Full specialization for int
template <>
struct TypeInfo<int>
{
    static constexpr const char* name = "int";
    static int defaultValue() { return 42; }
};

// Full specialization for double
template <>
struct TypeInfo<double>
{
    static constexpr const char* name = "double";
    static double defaultValue() { return 3.14; }
};

// Full specialization for const char*
template <>
struct TypeInfo<const char*>
{
    static constexpr const char* name = "C-string";
    static const char* defaultValue() { return "hello"; }
};

// ---------- 2. Full specialization of a function ----------
template <typename T>
T max_val(T a, T b)
{
    return (a > b) ? a : b;
}

// Full specialization for const char*
template <>
const char* max_val(const char* a, const char* b)
{
    return (std::strcmp(a, b) > 0) ? a : b;
}

// ==================== Partial Specialization ====================

// ---------- 3. Partial specialization for pointer types ----------
template <typename T>
struct IsPointer      // primary: false
{
    static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*>   // partial spec: true for any pointer
{
    static constexpr bool value = true;
};

// ---------- 4. Partial specialization for const types ----------
template <typename T>
struct RemoveConst
{
    using type = T;
};

template <typename T>
struct RemoveConst<const T>
{
    using type = T;
};

// ---------- 5. Partial specialization for arrays ----------
template <typename T>
struct ArraySize;

template <typename T, std::size_t N>
struct ArraySize<T[N]>
{
    static constexpr std::size_t size = N;
};

// ---------- 6. Multiple parameter partial specialization ----------
template <typename T, typename U>
struct SameType
{
    static constexpr bool value = false;
};

template <typename T>
struct SameType<T, T>
{
    static constexpr bool value = true;
};

// ---------- 7. SFINAE with specialization ----------
template <typename T, typename = void>
struct HasSize : std::false_type {};

template <typename T>
struct HasSize<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

// ============================================================================
// SAMPLE I/O:
// -----------
// TypeInfo<int>::name = int
// TypeInfo<int>::defaultValue() = 42
// TypeInfo<double>::name = double
// TypeInfo<char>::name = unknown
// TypeInfo<const char*>::name = C-string
// max_val(10, 20) = 20
// max_val("apple", "zebra") = zebra
// IsPointer<int>::value = false
// IsPointer<int*>::value = true
// IsPointer<const char*>::value = true
// RemoveConst<const int>::type is int: true
// ArraySize<int[10]>::size = 10
// SameType<int, int>::value = true
// SameType<int, double>::value = false
// HasSize<vector<int>>::value = true
// HasSize<int>::value = false
// ============================================================================

int main()
{
    std::cout << "TypeInfo<int>::name = "
              << TypeInfo<int>::name << "\n";
    std::cout << "TypeInfo<int>::defaultValue() = "
              << TypeInfo<int>::defaultValue() << "\n";
    std::cout << "TypeInfo<double>::name = "
              << TypeInfo<double>::name << "\n";
    std::cout << "TypeInfo<char>::name = "
              << TypeInfo<char>::name << "\n";
    std::cout << "TypeInfo<const char*>::name = "
              << TypeInfo<const char*>::name << "\n";

    std::cout << "max_val(10, 20) = " << max_val(10, 20) << "\n";
    std::cout << "max_val(\"apple\", \"zebra\") = "
              << max_val("apple", "zebra") << "\n";

    std::cout << "IsPointer<int>::value = "
              << IsPointer<int>::value << "\n";
    std::cout << "IsPointer<int*>::value = "
              << IsPointer<int*>::value << "\n";
    std::cout << "IsPointer<const char*>::value = "
              << IsPointer<const char*>::value << "\n";

    std::cout << "RemoveConst<const int>::type is int: "
              << std::is_same_v<RemoveConst<const int>::type, int> << "\n";

    std::cout << "ArraySize<int[10]>::size = "
              << ArraySize<int[10]>::size << "\n";

    std::cout << "SameType<int, int>::value = "
              << SameType<int, int>::value << "\n";
    std::cout << "SameType<int, double>::value = "
              << SameType<int, double>::value << "\n";

    std::cout << "HasSize<vector<int>>::value = "
              << HasSize<std::vector<int>>::value << "\n";
    std::cout << "HasSize<int>::value = "
              << HasSize<int>::value << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All: O(1) compile-time, zero runtime overhead
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: Why can't functions be partially specialized?
// A1: The language allows overloading instead, which is more powerful.
//     Partial specialization of function templates would interact badly
//     with overload resolution. Use overloading or delegate to a class
//     template (which can be partially specialized).
//
// Q2: What's the difference between specialization and instantiation?
// A2: Instantiation is the compiler generating code for a specific set
//     of template arguments from the primary template. Specialization is
//     the user providing an explicit alternative implementation for
//     specific arguments.
//
// Q3: How does the compiler choose between specializations?
// A3: It uses partial ordering: the most specialized specialization wins.
//     If multiple are equally specialized, the call is ambiguous.
//
// COMMON MISTAKES:
// ----------------
// - Forgetting template<> before a full specialization.
// - Trying to partially specialize a function template (compiler error).
// - Providing a specialization that doesn't match the primary template's
//   interface (missing members, different signatures).
// - Assuming specialization works at runtime (it's entirely compile-time).
// ============================================================================
