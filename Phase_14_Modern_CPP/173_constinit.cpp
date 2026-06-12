// ============================================================================
// Phase 14 — Modern C++
// Topic 173: constinit – Guaranteed Static Initialization (C++20)
// ============================================================================
//
// THEORY:
// --------
// constinit (C++20): Forces a variable with static storage duration to be
// initialized at compile time (constant initialization), avoiding the "static
// initialization order fiasco".
//
// The "static initialization order fiasco" occurs when one translation unit's
// dynamic initializer depends on another's global variable that hasn't been
// initialized yet.
//
// constinit guarantees the variable is initialized with a constant expression
// (like constexpr), but the variable itself is NOT const — it can be mutated
// later.
//
// KEY POINTS:
// - constinit applies to variables with static storage duration (global,
//   namespace, static class members, function-static).
// - constinit does NOT make the variable immutable (unlike constexpr).
// - constinit must be used in combination with initialization that is a
//   constant expression.
// - constinit helps with thread-safe static initialization: local static
//   variables with constinit are guaranteed to be initialized before any
//   dynamic initialization, so they are safe.
//
// ============================================================================

#include <iostream>
#include <array>
#include <mutex>
#include <atomic>
#include <cassert>

// ---------- 1. constinit global variable ----------
// Guaranteed compile-time initialized, but mutable at runtime.
constinit int global_counter = 100;    // OK — constant expression

// constinit int bad = std::rand();    // ERROR — not a constant expression

// ---------- 2. constinit with non-trivial types ----------
// constinit works with types that have constexpr constructors.
struct Point
{
    int x, y;
    constexpr Point(int a, int b) noexcept : x{a}, y{b} {}
};

constinit Point origin{0, 0};          // OK — constexpr constructor + constexpr args

// ---------- 3. constinit vs constexpr ----------
constexpr int compile_time_val = 42;   // constexpr → const + compile-time init
constinit int mutable_global = 42;     // mutable, but compile-time init

// ---------- 4. constinit with static local (thread-safe) ----------
// Guarantees compile-time initialization, no thread-safety overhead.
struct Logger
{
    int id;
    constexpr Logger(int i) noexcept : id{i} {}
    void log(const char* msg) const
    {
        std::cout << "[Logger " << id << "] " << msg << "\n";
    }
};

void use_logger()
{
    // Guaranteed compile-time init — no mutex needed, no dynamic init.
    constinit static Logger logger{42};
    logger.log("Hello from constinit local static");
}

// ---------- 5. constinit with std::array ----------
constinit std::array<int, 3> primes = {2, 3, 5};

// ---------- 6. Solving static init order fiasco ----------
// File-scope variables in different translation units can have undefined
// initialization order. constinit forces constant init, removing the problem.
// In practice: mark any global that can be const-init with constinit.

// ============================================================================
// SAMPLE I/O:
// -----------
// global_counter = 100
// origin = (0, 0)
// mutable_global before: 42
// mutable_global after:  99
// [Logger 42] Hello from constinit local static
// primes: 2 3 5
// ============================================================================

int main()
{
    std::cout << "global_counter = " << global_counter << "\n";
    std::cout << "origin = (" << origin.x << ", " << origin.y << ")\n";

    // constinit variables are NOT const — we can modify them
    std::cout << "mutable_global before: " << mutable_global << "\n";
    mutable_global = 99;
    std::cout << "mutable_global after:  " << mutable_global << "\n";

    use_logger();

    std::cout << "primes:";
    for (auto p : primes) std::cout << ' ' << p;
    std::cout << "\n";

    // Demonstrate that constinit is not constexpr
    // mutable_global = 42;  // OK — not const
    // constinit int x;      // ERROR — must be initialized

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// All operations: O(1) initialization, O(1) access
// constinit local static: zero overhead at runtime (no guard variable)
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What problem does constinit solve?
// A1: The static initialization order fiasco — when global variables in
//     different translation units depend on each other's initialization
//     order, which is undefined. constinit forces constant initialization,
//     so the variable is ready before any dynamic init occurs.
//
// Q2: constinit vs constexpr — what's the difference?
// A2: constexpr implies const (immutable after init). constinit only
//     guarantees that initialization is constant — the variable can be
//     mutated later. Both require constant expressions for initialization.
//
// Q3: Can constinit be used with non-static local variables?
// A3: No. constinit is only for variables with static storage duration
//     (global, namespace-scope, static locals, static class members).
//
// COMMON MISTAKES:
// ----------------
// - Assuming constinit makes the variable const (like constexpr).
// - Using constinit with runtime-dependent initializers — compile error.
// - Forgetting that constinit requires an initializer at the point of
//   declaration.
// - Applying constinit to non-static local variables or member variables.
// ============================================================================
