// ============================================================================
// Phase 14 — Modern C++
// Topic 178: Ranges – C++20 Ranges Library
// ============================================================================
//
// THEORY:
// --------
// C++20 Ranges (<ranges>) provide a new way to work with sequences:
// composable algorithms, lazy evaluation, and uniform iteration.
//
// Key components:
//   - Range: an object with begin()/end() (or free functions) — anything
//     iterable
//   - Views: lightweight, non-owning, lazy-evaluated range adaptors
//   - Actions: eager range adaptors (C++23, not covered here)
//   - Pipe operator |: composes views/adaptors cleanly
//
// std::ranges algorithms:
//   Drop prefix "std::" in many cases: std::ranges::sort(v) instead of
//   std::sort(v.begin(), v.end()). Support projections and range directly.
//
// ============================================================================

#include <iostream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>
#include <list>

// ---------- 1. std::ranges::sort (projection) ----------
void demo_sort()
{
    struct Person {
        std::string name;
        int age;
    };

    std::vector<Person> people = {
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}
    };

    // Sort by age using projection
    std::ranges::sort(people, {}, &Person::age);

    for (const auto& [name, age] : people)
        std::cout << name << " (" << age << ") ";
    std::cout << "\n";
}

// ---------- 2. Basic ranges adaptors ----------
void demo_adaptors()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Pipe operator — compose views
    auto result = v
                | std::views::filter([](int x) { return x % 2 == 0; })
                | std::views::transform([](int x) { return x * 10; })
                | std::views::take(3);

    std::cout << "First 3 even numbers * 10:";
    for (int x : result) std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 3. Lazy evaluation (views don't own/copy) ----------
void demo_lazy()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    auto view = v | std::views::transform([](int x) {
        std::cout << "  transform(" << x << ")\n";
        return x * 2;
    });

    std::cout << "Before iteration — nothing printed yet (lazy)\n";
    // Uncommenting next line triggers lazy evaluation:
    for (int x : view) {
        std::cout << "Got: " << x << "\n";
        break;   // only 1 element transformed!
    }
}

// ---------- 4. Range adaptors: reverse, take, drop, filter ----------
void demo_more_adaptors()
{
    std::vector<int> v = {10, 20, 30, 40, 50};

    std::cout << "Reverse:";
    for (int x : v | std::views::reverse) std::cout << ' ' << x;
    std::cout << "\n";

    std::cout << "Drop 2:";
    for (int x : v | std::views::drop(2)) std::cout << ' ' << x;
    std::cout << "\n";

    std::cout << "Filter > 25, then take 1:";
    for (int x : v | std::views::filter([](int n) { return n > 25; })
                     | std::views::take(1))
        std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 5. std::views::iota (infinite range) ----------
void demo_iota()
{
    std::cout << "First 5 odd squares:";
    for (int x : std::views::iota(1)
               | std::views::filter([](int n) { return n % 2 == 1; })
               | std::views::transform([](int n) { return n * n; })
               | std::views::take(5))
        std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 6. ranges::to (C++23) — use manual collection for C++20 ----------
template <std::ranges::range R>
auto to_vector(R&& r)
{
    using value_type = std::ranges::range_value_t<R>;
    std::vector<value_type> vec;
    if constexpr (std::ranges::sized_range<R>) {
        vec.reserve(std::ranges::size(r));
    }
    for (auto&& elem : r)
        vec.push_back(std::forward<decltype(elem)>(elem));
    return vec;
}

// ---------- 7. Projection with ranges algorithms ----------
void demo_projection()
{
    std::vector<std::string> words = {"apple", "kiwi", "banana", "strawberry"};
    // Find shortest string
    auto it = std::ranges::min_element(words, {}, &std::string::size);
    if (it != words.end())
        std::cout << "Shortest word: " << *it << "\n";
}

// ============================================================================
// SAMPLE I/O:
// -----------
// Bob (25) Alice (30) Charlie (35)
// First 3 even numbers * 10: 20 40 60
// Before iteration... (lazy)
//   transform(1)
// Got: 2
// Reverse: 50 40 30 20 10
// Drop 2: 30 40 50
// Filter > 25, then take 1: 30
// First 5 odd squares: 1 9 25 49 81
// Shortest word: kiwi
// ============================================================================

int main()
{
    demo_sort();
    demo_adaptors();
    demo_lazy();
    demo_more_adaptors();
    demo_iota();
    demo_projection();

    // to_vector example
    auto vec = to_vector(std::views::iota(1, 6));
    std::cout << "to_vector:";
    for (int x : vec) std::cout << ' ' << x;
    std::cout << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// std::ranges::sort: O(N log N)
// Views: O(1) construction, O(N) iteration (lazy)
// filter + take(k): O(N) worst-case but k elements returned
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What's the advantage of ranges over iterator-pair algorithms?
// A1: Composability (pipe operator), readability, lazy evaluation,
//     projection support, and elimination of begin/end boilerplate.
//
// Q2: Are views owning or non-owning?
// A2: Non-owning by default — they hold references to the underlying range.
//     Dangling references are possible if the original range is destroyed.
//
// Q3: What do projections do?
// A3: Projections transform each element before comparison/processing in
//     algorithms — e.g., sort by a member, min by a getter. They avoid
//     needing to write custom comparators for common cases.
//
// COMMON MISTAKES:
// ----------------
// - Storing a view when the underlying range goes out of scope (dangling).
// - Expecting eager evaluation from views (they are lazy).
// - Using std::views::filter on a temporary rvalue range (undefined behavior).
// - Forgetting to #include <ranges> and <algorithm> separately.
// ============================================================================
