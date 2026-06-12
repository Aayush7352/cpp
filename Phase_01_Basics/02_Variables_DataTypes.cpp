/*
 * ====================================================================
 * Program 2: Variables & Data Types
 *
 * THEORY:
 * C++ provides several fundamental data types:
 *
 *   Integer types:   short, int, long, long long (signed & unsigned)
 *   Floating types:  float, double, long double
 *   Character types: char, wchar_t, char8_t (C++20), char16_t, char32_t
 *   Boolean type:    bool
 *   Void type:       void (no value)
 *   Nullptr:         decltype(nullptr)
 *
 * INITIALIZATION STYLES (C++11+):
 *   int a = 5;        // copy initialization
 *   int b(5);         // direct initialization
 *   int c{5};         // brace initialization (prevents narrowing)
 *   int d{};          // value initialization (zero)
 *
 * `auto` deduces the type from the initializer at compile time.
 * `sizeof` returns the size in bytes of a type or expression.
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output:
 *     Size of int:          4 bytes
 *     Size of float:        4 bytes
 *     Size of double:       8 bytes
 *     Size of char:         1 byte
 *     Size of bool:         1 byte
 *     Size of long long:    8 bytes
 *     int a = 42
 *     auto x = 3.14 (double)
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: What is the difference between int, long, and long long?
 *   A: Typically int=4B, long=8B, long long=8B on 64-bit systems,
 *      but sizes are implementation-defined (C++ guarantees int≥16,
 *      long≥32, long long≥64 bits).
 *   Q: What is narrowing conversion and how does brace initialization help?
 *   A: Narrowing loses data (e.g., double→int). Braced lists prevent it.
 *   Q: What is the difference between char, signed char, unsigned char?
 *   A: They are three distinct types. char may be signed or unsigned
 *      depending on the implementation.
 *
 * COMMON MISTAKES:
 *   - Using uninitialized variables — undefined behavior.
 *   - Assuming sizeof gives bits (it gives bytes).
 *   - Mixing signed and unsigned integers (unexpected wrap-around).
 *   - Using `float` when `double` precision is needed.
 * ====================================================================
 */

#include <iostream>
#include <typeinfo>

int main()
{
    // ---- Integer types ----
    short           s   = 10;
    int             i   = 42;
    long            l   = 100000L;
    long long       ll  = 10000000000LL;

    unsigned int    ui  = 500U;
    unsigned long   ul  = 1000UL;

    // ---- Floating-point types ----
    float           f   = 3.14f;
    double          d   = 3.14159265358979;
    long double     ld  = 3.141592653589793238L;

    // ---- Character types ----
    char            c   = 'A';
    wchar_t         wc  = L'\u03A9';      // Greek Omega
    char16_t        c16 = u'\u00E9';      // Latin e-acute
    char32_t        c32 = U'\U0001F600';  // grinning face emoji

    // ---- Boolean ----
    bool            flag = true;

    // ---- Initialization styles ----
    int copy_init    = 5;       // copy initialization
    int direct_init(10);        // direct initialization
    int brace_init{15};         // brace initialization (C++11)
    int value_init{};           // value initialization → 0

    // ---- auto keyword ----
    auto            ax  = 42;            // int
    auto            af  = 3.14f;         // float
    auto            ad  = 3.14;          // double
    auto            ac  = 'Z';           // char

    // ---- sizeof operator ----
    std::cout << "=== Sizes of basic types (in bytes) ===\n";
    std::cout << "sizeof(short):      " << sizeof(short)      << '\n';
    std::cout << "sizeof(int):        " << sizeof(int)        << '\n';
    std::cout << "sizeof(long):       " << sizeof(long)       << '\n';
    std::cout << "sizeof(long long):  " << sizeof(long long)  << '\n';
    std::cout << "sizeof(float):      " << sizeof(float)      << '\n';
    std::cout << "sizeof(double):     " << sizeof(double)     << '\n';
    std::cout << "sizeof(long double):" << sizeof(long double) << '\n';
    std::cout << "sizeof(char):       " << sizeof(char)       << '\n';
    std::cout << "sizeof(wchar_t):    " << sizeof(wchar_t)    << '\n';
    std::cout << "sizeof(bool):       " << sizeof(bool)       << '\n';

    std::cout << "\n=== Values ===\n";
    std::cout << "int:        " << i   << '\n';
    std::cout << "float:      " << f   << '\n';
    std::cout << "double:     " << d   << '\n';
    std::cout << "char:       " << c   << '\n';
    std::cout << "bool:       " << flag << '\n';

    std::cout << "\n=== Initialization styles ===\n";
    std::cout << "copy_init:  " << copy_init   << '\n';
    std::cout << "direct_init:" << direct_init  << '\n';
    std::cout << "brace_init: " << brace_init   << '\n';
    std::cout << "value_init: " << value_init   << '\n';

    std::cout << "\n=== auto deduction ===\n";
    std::cout << "ax (int):           " << ax       << '\n';
    std::cout << "af (float):         " << af       << '\n';
    std::cout << "ad (double):        " << ad       << '\n';
    std::cout << "ac (char):          " << ac       << '\n';

    return 0;
}
