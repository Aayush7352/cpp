/*
 * ====================================================================
 * Program 3: Constants & Literals
 *
 * THEORY:
 * CONST KEYWORDS:
 *   const      − Runtime constant (cannot be modified after init)
 *   constexpr  − Compile-time constant (evaluated at compile time)
 *   consteval  − C++20: must be evaluated at compile time (immediate
 *                function)
 *   constinit  − C++20: guarantees static/thread_local init at compile
 *                time (not const; variable can still be mutated)
 *
 * LITERALS:
 *   Integer:   42, 0xFF (hex), 077 (octal), 0b1010 (binary C++14)
 *   Float:     3.14f, 2.71e-5, 1.0L (long double)
 *   Char:      'A', '\n', '\x41' (hex escape)
 *   String:    "hello", L"wide", u8"UTF-8", u"UTF-16", U"UTF-32"
 *   Boolean:   true, false
 *   Pointer:   nullptr
 *   User-defined literals (C++11): "42"_suffix
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra -o const_lit 03_Constants_Literals.cpp
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output:
 *     const:       42
 *     constexpr:   144
 *     constinit:   100
 *     consteval:   50
 *     Binary:      10
 *     String:      Hello
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: const vs constexpr vs consteval vs constinit?
 *   A: const = runtime constant; constexpr = compile-time constant
 *      (can be used at runtime too); consteval = must be compile-time
 *      only; constinit = compile-time init, but mutable.
 *   Q: What is a user-defined literal?
 *   A: A literal with a custom suffix, e.g. "42"_km, implemented
 *      via operator"" _km.
 *
 * COMMON MISTAKES:
 *   - Modifying a const variable → compile error.
 *   - Using constexpr with runtime values → compile error.
 *   - Integer literals starting with 0 are octal (e.g., 077 = 63).
 *   - Forgetting the suffix 'f' for float literals (default is double).
 * ====================================================================
 */

#include <iostream>
#include <string>
#include <string_view>

// ---- constinit: compile-time initialization (C++20), but mutable ----
constinit int global_counter = 100;

// ---- constexpr function ----
constexpr int square(int x)
{
    return x * x;
}

// ---- consteval function (C++20) - must be called with constant args ----
consteval int double_it(int x)
{
    return 2 * x;
}

// ---- User-defined literal (C++11) ----
constexpr long long operator"" _km(unsigned long long km)
{
    return static_cast<long long>(km) * 1000;
}

int main()
{
    // ---- const ----
    const int ci = 42;
    // ci = 99;  // Error: assignment of read-only variable

    // ---- constexpr (compile-time constant) ----
    constexpr int cm = 12;
    constexpr int area = cm * cm;   // 144, evaluated at compile time
    constexpr int sq = square(5);   // 25

    // ---- consteval ----
    constexpr int dv = double_it(25);   // OK, constant argument
    // int x = 10; double_it(x);        // Error: x not constant

    // ---- constinit ----
    std::cout << "constinit global_counter = " << global_counter << '\n';
    global_counter = 200;   // OK — constinit allows mutation
    std::cout << "After mutation:              " << global_counter << '\n';

    // ---- Integer literals ----
    int dec    = 42;        // decimal
    int hex    = 0x2A;      // hexadecimal
    int oct    = 052;       // octal
    int bin    = 0b101010;  // binary (C++14)

    // ---- Floating-point literals ----
    float       f = 3.14f;
    double      d = 3.14159;
    long double ld = 3.141592653589793L;

    // ---- Character literals ----
    char   c  = 'A';
    char   nl = '\n';
    char   he = '\x41';     // hex escape → 'A'

    // ---- String literals ----
    const char*     s   = "Hello";
    const wchar_t*  ws  = L"Wide";
    const char8_t*  u8s = u8"UTF-8";   // C++20
    const char16_t* u16s = u"UTF-16";
    const char32_t* u32s = U"UTF-32";

    // ---- Boolean literals ----
    bool t = true;
    bool f_bool = false;

    // ---- nullptr ----
    int* ptr = nullptr;

    // ---- User-defined literal ----
    constexpr long long dist = 42_km;   // 42000

    // ---- Output ----
    std::cout << "=== Constants ===\n";
    std::cout << "const ci:          " << ci   << '\n';
    std::cout << "constexpr area:    " << area << '\n';
    std::cout << "constexpr sq(5):   " << sq   << '\n';
    std::cout << "consteval dv:      " << dv   << '\n';

    std::cout << "\n=== Literals ===\n";
    std::cout << "Decimal:           " << dec    << '\n';
    std::cout << "Hex:               " << hex    << '\n';
    std::cout << "Octal:             " << oct    << '\n';
    std::cout << "Binary:            " << bin    << '\n';
    std::cout << "Float:             " << f      << '\n';
    std::cout << "Double:            " << d      << '\n';
    std::cout << "Char:              " << c      << '\n';
    std::cout << "String literal:    " << s      << '\n';
    std::cout << "Bool true:         " << t      << '\n';
    std::cout << "Bool false:        " << f_bool << '\n';
    std::cout << "nullptr:           " << ptr    << '\n';
    std::cout << "42_km (user-def):  " << dist   << " meters\n";

    return 0;
}
