/*
 * ====================================================================
 * Program 14: Enumerations
 *
 * THEORY:
 * C++ has two kinds of enumerations:
 *
 * 1. Unscoped enum (enum):
 *      enum Color { Red, Green, Blue };
 *      – Names leak into the enclosing scope (can collide).
 *      – Implicitly converts to int.
 *      – Underlying type is implementation-defined (often int).
 *
 * 2. Scoped enum (enum class / enum struct) — C++11:
 *      enum class Color { Red, Green, Blue };
 *      – Names are scoped (Color::Red).
 *      – No implicit conversion to int.
 *      – Underlying type defaults to int, but can be specified:
 *        enum class Color : std::uint8_t { ... };
 *
 * ENUM TO/FROM INTEGER:
 *   – unscoped:  static_cast<int>(Red) or implicitly.
 *   – scoped:    static_cast<int>(Color::Red) is required.
 *   – int → enum: static_cast<Color>(42).
 *
 * BIT FLAGS with enums:
 *   – Use scoped enums with underlying unsigned type.
 *   – Overload bitwise operators.
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output demonstrates enum usage.
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: enum vs enum class — which should you use?
 *   A: Prefer enum class (scoped, no implicit conversion, avoids
 *      name collisions).
 *   Q: Can you specify the underlying type of an enum?
 *   A: Yes: enum class Name : std::uint8_t { ... };
 *   Q: How do you use enums as bit flags?
 *   A: Assign powers of 2 to enumerators, overload | & etc.,
 *      or use std::bitset.
 *
 * COMMON MISTAKES:
 *   - Using unscoped enums that pollute the namespace.
 *   - Assuming sizeof(enum) == sizeof(int) (it's implementation).
 *   - Forgetting to cast when converting enum to/from integer.
 *   - Using the wrong underlying type (default int may be too small).
 * ====================================================================
 */

#include <iostream>
#include <type_traits>

// ---- Unscoped enum (old-style) ----
enum Color
{
    Red,
    Green,
    Blue
};

// ---- Scoped enum (C++11) ----
enum class TrafficLight
{
    Red,
    Yellow,
    Green
};

// ---- Scoped enum with explicit underlying type ----
enum class Permission : unsigned int
{
    None    = 0,
    Read    = 1 << 0,   // 1
    Write   = 1 << 1,   // 2
    Execute = 1 << 2    // 4
};

// Bitwise operators for Permission
constexpr Permission operator|(Permission a, Permission b)
{
    return static_cast<Permission>(
        static_cast<unsigned>(a) | static_cast<unsigned>(b));
}
constexpr Permission operator&(Permission a, Permission b)
{
    return static_cast<Permission>(
        static_cast<unsigned>(a) & static_cast<unsigned>(b));
}
constexpr bool has_flag(Permission val, Permission flag)
{
    return (static_cast<unsigned>(val) & static_cast<unsigned>(flag)) != 0;
}

int main()
{
    // ---- Unscoped enum ----
    std::cout << "=== Unscoped enum ===\n";
    Color c = Green;
    std::cout << "Green = " << Green << '\n';         // 2
    std::cout << "Red   = " << Red << '\n';           // 0

    // Implicit conversion to int
    int ci = Red;
    std::cout << "int ci = Red: " << ci << '\n';

    // ---- Scoped enum ----
    std::cout << "\n=== Scoped enum ===\n";
    TrafficLight tl = TrafficLight::Yellow;

    // int x = tl;  // Error: no implicit conversion
    int tl_int = static_cast<int>(tl);
    std::cout << "TrafficLight::Yellow = " << tl_int << '\n';

    // ---- Enum with custom underlying type ----
    std::cout << "\n=== Scoped enum with custom underlying type ===\n";
    std::cout << "sizeof(Permission) = " << sizeof(Permission) << " bytes\n";
    std::cout << "underlying type = "
              << typeid(std::underlying_type_t<Permission>).name() << '\n';

    // ---- Enum as bit flags ----
    std::cout << "\n=== Enum as bit flags ===\n";
    Permission perms = Permission::Read | Permission::Write;

    std::cout << "Has Read?    " << has_flag(perms, Permission::Read) << '\n';
    std::cout << "Has Write?   " << has_flag(perms, Permission::Write) << '\n';
    std::cout << "Has Execute? " << has_flag(perms, Permission::Execute) << '\n';

    // ---- int to enum ----
    std::cout << "\n=== int to enum ===\n";
    auto tl2 = static_cast<TrafficLight>(1);  // Yellow
    std::cout << "static_cast<TrafficLight>(1) = "
              << static_cast<int>(tl2) << '\n';

    // ---- Iterating over enum values ----
    std::cout << "\n=== Enum iteration pattern ===\n";
    for (int i = 0; i <= static_cast<int>(TrafficLight::Green); ++i)
    {
        auto light = static_cast<TrafficLight>(i);
        std::cout << "TrafficLight(" << i << ") = " << static_cast<int>(light)
                  << '\n';
    }

    return 0;
}
