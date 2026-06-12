/*
 * ====================================================================
 * Program 13: Structured Bindings
 *
 * THEORY:
 * Structured bindings (C++17) allow you to decompose a tuple-like
 * object into named variables in a single declaration.
 *
 * Syntax:
 *   auto [a, b, c] = expression;
 *   const auto& [a, b, c] = expression;   // read-only references
 *   auto& [a, b, c] = expression;         // mutable references
 *
 * WORKS WITH:
 *   - Arrays:               auto [a, b, c] = arr;
 *   - std::tuple:           auto [a, b] = std::tuple<int, double>{...};
 *   - std::pair:            auto [key, value] = some_map;
 *   - Structs with public members:
 *     struct Point { double x, y, z; };
 *     auto [x, y, z] = point;
 *
 * QUALIFIERS:
 *   const auto&  — bind by const reference (no copy)
 *   auto&        — bind by mutable reference (can modify original)
 *   auto&&       — forwarding reference bindings
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output demonstrates each decomposition method.
 *
 * TIME COMPLEXITY:   O(1)    (same as accessing members)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: How do structured bindings work with structs?
 *   A: They decompose public non-static data members in declaration
 *      order. Private members or base classes are not decomposed.
 *   Q: Can you use structured bindings with std::map iteration?
 *   A: Yes: for (const auto& [key, value] : my_map) { ... }
 *   Q: How are structured bindings implemented internally?
 *   A: The compiler creates an anonymous hidden variable and binds
 *      the names as aliases to its members/parts.
 *
 * COMMON MISTAKES:
 *   - Trying to apply structured bindings to a struct with private
 *     or static data members.
 *   - Assuming the bound variables are references when using auto
 *     (they're copies unless you use auto&).
 *   - Trying to take the address of individual bindings (they're
 *     not real variables — they're aliases).
 *   - Using structured bindings with std::array but treating the
 *     result as a reference to the original (use auto& for that).
 * ====================================================================
 */

#include <iostream>
#include <tuple>
#include <map>
#include <string>
#include <array>

// ---- Struct with public members ----
struct Point
{
    double x;
    double y;
    double z;
};

// ---- Tuple-like struct via std::tuple_size ----
struct Student
{
    std::string name;
    int id;
    double gpa;
};

// Specialize tuple_size and tuple_element for Student (C++17)
namespace std
{
    template <>
    struct tuple_size<::Student> : std::integral_constant<size_t, 3> {};

    template <>
    struct tuple_element<0, ::Student> { using type = std::string; };
    template <>
    struct tuple_element<1, ::Student> { using type = int; };
    template <>
    struct tuple_element<2, ::Student> { using type = double; };
}

template <size_t I>
auto get(const Student& s)
{
    if constexpr (I == 0) return s.name;
    else if constexpr (I == 1) return s.id;
    else return s.gpa;
}

int main()
{
    std::cout << "=== Structured bindings with arrays ===\n";
    int arr[3] = {10, 20, 30};
    auto [a1, a2, a3] = arr;   // copies
    std::cout << a1 << ", " << a2 << ", " << a3 << '\n';

    auto& [ra1, ra2, ra3] = arr;  // references — can modify
    ra1 = 99;
    std::cout << "arr[0] after auto&: " << arr[0] << '\n';

    // ---- With std::tuple ----
    std::cout << "\n=== With std::tuple ===\n";
    auto tup = std::make_tuple(42, 3.14, "hello");
    auto [i, d, s] = tup;
    std::cout << i << ", " << d << ", " << s << '\n';

    // ---- With std::pair / map iteration ----
    std::cout << "\n=== With std::map ===\n";
    std::map<int, std::string> fruits = {{1, "apple"}, {2, "banana"}};
    for (const auto& [key, value] : fruits)
    {
        std::cout << key << " → " << value << '\n';
    }

    // ---- With struct (public members) ----
    std::cout << "\n=== With struct Point ===\n";
    Point p{1.5, 2.5, 3.5};
    auto [px, py, pz] = p;
    std::cout << "Point: x = " << px << ", y = " << py << ", z = " << pz << '\n';

    // ---- const auto& — avoid copy ----
    std::cout << "\n=== const auto& (no copy) ===\n";
    const auto& [cx, cy, cz] = p;
    std::cout << cx << ", " << cy << ", " << cz << '\n';

    // ---- auto&& — forwarding references ----
    std::cout << "\n=== auto&& ===\n";
    auto&& [fx, fy, fz] = Point{7.0, 8.0, 9.0};
    std::cout << fx << ", " << fy << ", " << fz << '\n';

    // ---- With std::array ----
    std::cout << "\n=== With std::array ===\n";
    std::array<int, 4> arr4 = {1, 2, 3, 4};
    auto [x1, x2, x3, x4] = arr4;
    std::cout << x1 << " " << x2 << " " << x3 << " " << x4 << '\n';

    return 0;
}
