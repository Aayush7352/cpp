/*
 * ====================================================================
 * Program 15: Namespaces
 *
 * THEORY:
 * A namespace is a declarative region that prevents name collisions.
 *
 *   namespace Name { /* declarations * / }
 *
 * FEATURES:
 *   1. Nested namespaces (C++17: namespace A::B::C { })
 *   2. Namespace alias:   namespace alias = original;
 *   3. Using declaration:  using std::cout;   (specific name)
 *   4. Using directive:    using namespace std; (all names — risky)
 *   5. ADL (Argument-Dependent Lookup / Koenig lookup):
 *      – When calling a function, the compiler looks in the
 *        namespaces of the arguments' types.
 *   6. Inline namespace (C++11):
 *      – Members are treated as if they're in the parent namespace.
 *      – Used for versioning / ABI compatibility.
 *   7. Anonymous namespace — internal linkage (equivalent to static).
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output demonstrates namespace features.
 *
 * TIME COMPLEXITY:   O(1)   (compile-time concept)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: What is ADL (Argument-Dependent Lookup)?
 *   A: The compiler looks for the function in the namespaces of
 *      its arguments. E.g., std::cout << "hi" works via ADL for <<.
 *   Q: What is an inline namespace used for?
 *   A: Versioning library APIs. The current version is inline so
 *      code just uses Parent::Symbol, while old versions remain
 *      accessible via Parent::v1::Symbol.
 *   Q: What is the difference between `using std::cout` and
 *      `using namespace std`?
 *   A: The first brings a single name into scope; the second
 *      brings the entire namespace (may cause name collisions).
 *
 * COMMON MISTAKES:
 *   - `using namespace std` in header files (pollutes global namespace).
 *   - Name collisions from using directives.
 *   - Forgetting that anonymous namespaces give internal linkage.
 *   - Not qualifying names correctly in nested namespaces.
 * ====================================================================
 */

#include <iostream>
#include <string>
#include <vector>

// ---- Basic namespace ----
namespace Math
{
    constexpr double pi = 3.14159265358979;

    int add(int a, int b)
    {
        return a + b;
    }

    // Nested namespace (C++17 compact syntax)
    namespace Geometry
    {
        double circle_area(double r)
        {
            return pi * r * r;
        }
    }
}

// ---- Inline namespace for versioning ----
namespace Library
{
    inline namespace v2
    {
        void greet()
        {
            std::cout << "Library v2: Hello!\n";
        }
    }

    namespace v1
    {
        void greet()
        {
            std::cout << "Library v1: Hello!\n";
        }
    }
}

// ---- Anonymous namespace (internal linkage) ----
namespace
{
    int internal_counter = 0;

    void increment()
    {
        ++internal_counter;
    }
}

// ---- ADL demonstration ----
namespace ADL
{
    struct Point
    {
        int x, y;
    };

    void print(const Point& p)
    {
        std::cout << "ADL::Point(" << p.x << ", " << p.y << ")\n";
    }
}

int main()
{
    // ---- Basic namespace usage ----
    std::cout << "=== Basic namespace ===\n";
    std::cout << "Math::pi = " << Math::pi << '\n';
    std::cout << "Math::add(3, 4) = " << Math::add(3, 4) << '\n';

    // ---- Nested namespace ----
    std::cout << "\n=== Nested namespace ===\n";
    std::cout << "Math::Geometry::circle_area(5) = "
              << Math::Geometry::circle_area(5) << '\n';

    // ---- Inline namespace ----
    std::cout << "\n=== Inline namespace ===\n";
    Library::greet();  // calls v2 (inline)

    // ADL: using declaration for std::cout
    using std::cout;   // brings cout into scope
    cout << "Accessed via using std::cout\n";

    // ---- ADL in action ----
    std::cout << "\n=== ADL (Koenig lookup) ===\n";
    ADL::Point p{10, 20};
    print(p);  // ADL finds ADL::print because p is in ADL namespace

    // ---- Using directive (be careful) ----
    std::cout << "\n=== Using directive ===\n";
    using namespace std::literals;  // bring in 's' suffix for strings
    auto str = "C++14 string"s;     // std::string
    cout << "Using directive: " << str << '\n';

    // ---- Namespace alias ----
    std::cout << "\n=== Namespace alias ===\n";
    namespace Geo = Math::Geometry;
    cout << "Alias Geo::circle_area(3): " << Geo::circle_area(3) << '\n';

    // ---- Anonymous namespace ----
    std::cout << "\n=== Anonymous namespace ===\n";
    increment();
    increment();
    cout << "internal_counter after 2 increments: " << internal_counter << '\n';

    return 0;
}
