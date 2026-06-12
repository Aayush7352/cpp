// ============================================================================
// Phase 14 — Modern C++
// Topic 179: Views – C++20 Range Views (Lazy Adaptors)
// ============================================================================
//
// THEORY:
// --------
// Views are lightweight, non-owning range adaptors that perform lazy
// evaluation. They compose via the pipe operator |.
//
// Key views:
//   std::views::filter   — keep elements satisfying a predicate
//   std::views::transform — apply a function to each element
//   std::views::take     — take first N elements
//   std::views::drop     — skip first N elements
//   std::views::reverse  — iterate in reverse
//   std::views::iota     — infinite/generated sequence
//   std::views::common   — convert to common range (beg/end same type)
//   std::views::keys / values — pair/tuple key/value extraction
//   std::views::join     — flatten nested ranges
//   std::views::split    — split on delimiter
//   std::views::stride   — (C++23) take every N-th element
//
// Lazy evaluation: views compute elements only when iterated. The transform
// function runs once per element during iteration, not at construction.
//
// ============================================================================

#include <iostream>
#include <ranges>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

// ---------- 1. filter view ----------
void demo_filter()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    auto evens = v | std::views::filter([](int n) { return n % 2 == 0; });

    std::cout << "Even numbers:";
    for (int e : evens) std::cout << ' ' << e;
    std::cout << "\n";
}

// ---------- 2. transform view ----------
void demo_transform()
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    auto doubled = v | std::views::transform([](int n) { return n * 2; });

    std::cout << "Doubled:";
    for (int d : doubled) std::cout << ' ' << d;
    std::cout << "\n";
}

// ---------- 3. take and drop ----------
void demo_take_drop()
{
    std::vector<int> v = {10, 20, 30, 40, 50};

    std::cout << "Take 3:";
    for (int x : v | std::views::take(3)) std::cout << ' ' << x;
    std::cout << "\n";

    std::cout << "Drop 3:";
    for (int x : v | std::views::drop(3)) std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 4. reverse view ----------
void demo_reverse()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::cout << "Reversed:";
    for (int x : v | std::views::reverse) std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 5. iota view (infinite range) ----------
void demo_iota()
{
    std::cout << "iota(5, 12):";
    for (int x : std::views::iota(5, 12)) std::cout << ' ' << x;
    std::cout << "\n";

    std::cout << "First 7 powers of 2:";
    for (int x : std::views::iota(0)
               | std::views::transform([](int n) { return 1 << n; })
               | std::views::take(7))
        std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 6. keys / values view (map) ----------
void demo_keys_values()
{
    std::map<int, std::string> m = {
        {1, "one"}, {2, "two"}, {3, "three"}
    };

    std::cout << "Keys:";
    for (int k : m | std::views::keys) std::cout << ' ' << k;
    std::cout << "\n";

    std::cout << "Values:";
    for (const auto& v : m | std::views::values) std::cout << ' ' << v;
    std::cout << "\n";
}

// ---------- 7. join view (flatten nested ranges) ----------
void demo_join()
{
    std::vector<std::vector<int>> nested = {
        {1, 2}, {3, 4, 5}, {6}
    };

    auto flat = nested | std::views::join;

    std::cout << "Joined:";
    for (int x : flat) std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 8. split view ----------
void demo_split()
{
    std::string text = "hello,world,from,views";

    std::cout << "Split by ',':";
    for (const auto& word : text | std::views::split(','))
    {
        // Each element is a subrange — construct string from it
        std::cout << ' ' << std::string_view(word.begin(), word.end());
    }
    std::cout << "\n";
}

// ---------- 9. Combining multiple views ----------
void demo_composition()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Chain: filter → transform → take → reverse
    auto pipeline = v
                  | std::views::filter([](int n) { return n % 3 == 0; })
                  | std::views::transform([](int n) { return n * n; })
                  | std::views::take(3)
                  | std::views::reverse;

    std::cout << "Pipeline result:";
    for (int x : pipeline) std::cout << ' ' << x;
    std::cout << "\n";
}

// ---------- 10. Lazy demonstration ----------
void demo_lazy_computation()
{
    std::vector<int> v = {10, 20, 30, 40, 50};

    std::cout << "Lazy transform — transform prints on demand:\n";
    auto view = v | std::views::transform([](int x) {
        std::cout << "    compute(" << x << ")\n";
        return x / 10;
    });

    std::cout << "  Created view, no computation yet.\n";
    std::cout << "  Iterating:\n";
    for (int x : view | std::views::take(2))
        std::cout << "    got: " << x << "\n";
}

// ============================================================================
// SAMPLE I/O:
// -----------
// Even numbers: 2 4 6
// Doubled: 2 4 6 8 10
// Take 3: 10 20 30
// Drop 3: 40 50
// Reversed: 5 4 3 2 1
// iota(5, 12): 5 6 7 8 9 10 11
// First 7 powers of 2: 1 2 4 8 16 32 64
// Keys: 1 2 3
// Values: one two three
// Joined: 1 2 3 4 5 6
// Split by ',': hello world from views
// Pipeline result: 81 36 9
// Lazy — no computation until iteration...
// ============================================================================

int main()
{
    demo_filter();
    demo_transform();
    demo_take_drop();
    demo_reverse();
    demo_iota();
    demo_keys_values();
    demo_join();
    demo_split();
    demo_composition();
    demo_lazy_computation();
    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// View construction: O(1)
// Iteration: O(N) — but only as many elements as actually visited
// filter+take(k): O(N) worst, but only k results are returned
// join: O(total elements)
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What does "lazy evaluation" mean for views?
// A1: Elements are computed on-demand during iteration, not at view
//     construction. The transform/filter function runs when the element
//     is accessed, not when the view is created.
//
// Q2: What's the difference between views::take and views::filter?
// A2: take(N) takes the first N elements (eager truncation), filter
//     selects elements matching a predicate (may skip elements).
//
// Q3: Can you modify elements through a view?
// A3: Yes, if the view provides a mutable reference (e.g., filter on
//     non-const range). But transform returns prvalues (copies) — cannot
//     modify originals through transform.
//
// COMMON MISTAKES:
// ----------------
// - Storing a view and using it after the underlying range is destroyed
//   (dangling reference — undefined behavior).
// - Expecting views to own data (they are non-owning).
// - Forgetting that transform returns a prvalue (not a reference).
// - Using views on temporary rvalue containers (lifetime issue).
// - Not including <ranges> header.
// ============================================================================
