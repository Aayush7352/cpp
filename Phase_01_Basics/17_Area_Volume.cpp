/*
 * ====================================================================
 * Program 17: Area & Volume Calculator
 *
 * THEORY:
 * Geometric formulas implemented in C++:
 *
 * Circle area:     A = π × r²
 * Rectangle area:  A = w × h
 * Triangle area:   A = (b × h) / 2
 * Sphere volume:   V = (4/3) × π × r³
 * Cylinder volume: V = π × r² × h
 * Cube volume:     V = s³
 *
 * Uses `static_cast<double>` for proper floating division,
 * `constexpr` for constants, and `std::numbers::pi` (C++20).
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:
 *     Circle radius: 5
 *     Rectangle w h: 4 6
 *     Triangle b h:  8 3
 *     Sphere radius: 3
 *     Cylinder r h:  3 7
 *     Cube side:     4
 *   Output:
 *     Circle area:    78.5397
 *     Rectangle area: 24
 *     Triangle area:  12
 *     Sphere volume:  113.097
 *     Cylinder volume: 197.92
 *     Cube volume:    64
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: How do you properly compute (4/3) in C++?
 *   A: Use 4.0 / 3.0 or static_cast<double>(4) / 3 — otherwise
 *      integer division yields 1.
 *   Q: Why use constexpr for PI?
 *   A: constexpr ensures compile-time evaluation — zero runtime cost.
 *
 * COMMON MISTAKES:
 *   - Integer division for fraction constants (4/3 gives 1).
 *   - Not including <numbers> for std::numbers::pi (C++20).
 *   - Using pow() from <cmath> when simple multiplication suffices.
 *   - Precision loss due to using float instead of double.
 * ====================================================================
 */

#include <iostream>
#include <numbers>   // C++20 std::numbers::pi
#include <cmath>     // std::pow if needed, but we'll avoid it

constexpr double circle_area(double r)
{
    return std::numbers::pi * r * r;
}

constexpr double rectangle_area(double w, double h)
{
    return w * h;
}

constexpr double triangle_area(double base, double height)
{
    return (base * height) / 2.0;
}

constexpr double sphere_volume(double r)
{
    return (4.0 / 3.0) * std::numbers::pi * r * r * r;
}

constexpr double cylinder_volume(double r, double h)
{
    return std::numbers::pi * r * r * h;
}

constexpr double cube_volume(double side)
{
    return side * side * side;
}

int main()
{
    std::cout << "=== Area & Volume Calculator ===\n\n";

    double r{}, w{}, h{}, base{}, height{}, side{};

    // ---- Circle area ----
    std::cout << "Circle radius: ";
    std::cin >> r;
    std::cout << "Circle area = " << circle_area(r) << "\n\n";

    // ---- Rectangle area ----
    std::cout << "Rectangle width and height: ";
    std::cin >> w >> h;
    std::cout << "Rectangle area = " << rectangle_area(w, h) << "\n\n";

    // ---- Triangle area ----
    std::cout << "Triangle base and height: ";
    std::cin >> base >> height;
    std::cout << "Triangle area = " << triangle_area(base, height) << "\n\n";

    // ---- Sphere volume ----
    std::cout << "Sphere radius: ";
    std::cin >> r;
    std::cout << "Sphere volume = " << sphere_volume(r) << "\n\n";

    // ---- Cylinder volume ----
    std::cout << "Cylinder radius and height: ";
    std::cin >> r >> h;
    std::cout << "Cylinder volume = " << cylinder_volume(r, h) << "\n\n";

    // ---- Cube volume ----
    std::cout << "Cube side: ";
    std::cin >> side;
    std::cout << "Cube volume = " << cube_volume(side) << '\n';

    return 0;
}
