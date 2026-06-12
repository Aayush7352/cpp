// ============================================================================
// Phase 15 — Templates
// Topic 182: Class Templates
// ============================================================================
//
// THEORY:
// --------
// Class templates define a family of classes parameterized by types/values.
// Unlike function templates, class templates do NOT deduce template
// parameters from constructor arguments (C++17 CTAD changes this).
//
// Key features:
//   - Member functions defined inside class are inline; outside need
//     template parameter list.
//   - Static members are per-instantiation — each T has its own static
//     variable.
//   - CTAD (C++17): Class Template Argument Deduction from constructor.
//   - Template friend functions and nested classes.
//
// ============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

// ---------- 1. Basic class template ----------
template <typename T>
class Box
{
    T value;

public:
    explicit Box(T val) : value{val} {}

    T get() const { return value; }
    void set(T val) { value = val; }
};

// ---------- 2. Class template with multiple parameters ----------
template <typename Key, typename Value, int Capacity = 100>
class Cache
{
    std::vector<std::pair<Key, Value>> data;

public:
    void insert(Key k, Value v)
    {
        if (data.size() < static_cast<std::size_t>(Capacity))
            data.emplace_back(k, v);
    }

    void print() const
    {
        for (const auto& [k, v] : data)
            std::cout << "  " << k << ": " << v << "\n";
    }
};

// ---------- 3. Member function defined outside class ----------
template <typename T>
class Holder
{
    T value;
public:
    explicit Holder(T v);
    T get() const;
};

template <typename T>
Holder<T>::Holder(T v) : value{v} {}

template <typename T>
T Holder<T>::get() const { return value; }

// ---------- 4. Static member per instantiation ----------
template <typename T>
struct Counter
{
    static int count;

    Counter() { ++count; }
    ~Counter() { --count; }
};

template <typename T>
int Counter<T>::count = 0;

// ---------- 5. CTAD (C++17) ----------
template <typename T>
struct Wrapper
{
    T value;
    Wrapper(T v) : value{v} {}
};
// CTAD: Wrapper w{42}; // deduces Wrapper<int>

// ---------- 6. Template template parameters ----------
template <template <typename> class Container, typename T>
struct Adapter
{
    Container<T> container;
    void add(T val) { /* container.push(val); */ }
};

// ---------- 7. Template alias ----------
template <typename T>
using StringMap = std::vector<std::pair<std::string, T>>;

// ============================================================================
// SAMPLE I/O:
// -----------
// Box<int>(42).get() = 42
// Box<double>(3.14).get() = 3.14
// Cache contents:
//   1: one
//   2: two
// Holder(100).get() = 100
// Counter<int>::count = 2
// Counter<double>::count = 1
// Wrapper w{42} type is int: true
// StringMap<int> type check passed
// ============================================================================

int main()
{
    // Box
    Box<int> b1{42};
    Box<double> b2{3.14};
    std::cout << "Box<int>(42).get() = "    << b1.get() << "\n";
    std::cout << "Box<double>(3.14).get() = " << b2.get() << "\n";

    // Cache
    Cache<int, std::string, 10> cache;
    cache.insert(1, "one");
    cache.insert(2, "two");
    std::cout << "Cache contents:\n";
    cache.print();

    // Holder
    Holder<int> h{100};
    std::cout << "Holder(100).get() = " << h.get() << "\n";

    // Static member per instantiation
    Counter<int> c1, c2;
    Counter<double> d1;
    std::cout << "Counter<int>::count = "    << Counter<int>::count    << "\n";
    std::cout << "Counter<double>::count = " << Counter<double>::count << "\n";

    // CTAD
    Wrapper w{42};  // deduces Wrapper<int>
    std::cout << "Wrapper w{42} type is int: "
              << std::is_same_v<decltype(w), Wrapper<int>> << "\n";

    // Template alias
    StringMap<int> sm;
    sm.emplace_back("hello", 1);
    std::cout << "StringMap<int> type check passed\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// Box: O(1) all operations
// Cache: O(1) insert, O(N) print
// Counter: O(1) construction/destruction
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What is CTAD and when was it introduced?
// A1: Class Template Argument Deduction (C++17) allows the compiler to
//     deduce template parameters from constructor arguments:
//     std::pair p{1, 2.0}; // deduces pair<int, double>
//
// Q2: How do static members work in class templates?
// A2: Each instantiation of a class template has its own copy of static
//     members. Counter<int>::count and Counter<double>::count are
//     distinct variables.
//
// Q3: What are template template parameters?
// A3: Parameters that are themselves templates — e.g.,
//     template<template<typename> class Container, typename T>.
//     Used to accept any container type (vector, list, etc.) while
//     keeping type flexibility.
//
// COMMON MISTAKES:
// ----------------
// - Defining member functions outside class without repeating template<...>.
// - Forgetting that static members need a separate definition in a .cpp file
//   (or inline since C++17).
// - Assuming template aliases create new types (they are aliases, not types).
// - Using CTAD with class templates that have non-deducible parameters
//   (deduction guides needed).
// ============================================================================
