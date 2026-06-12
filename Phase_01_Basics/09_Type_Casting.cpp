/*
 * ====================================================================
 * Program 9: Type Casting
 *
 * THEORY:
 * C++ provides 4 named cast operators (preferred) + C-style cast:
 *
 *   1. static_cast<T>(expr)
 *      – Compile-time cast between related types.
 *      – e.g., int → double, void* → T*, downcast (no runtime check).
 *
 *   2. dynamic_cast<T>(expr)
 *      – Runtime cast for polymorphic class hierarchies.
 *      – Requires at least one virtual function.
 *      – Returns nullptr on failure (for pointers).
 *
 *   3. const_cast<T>(expr)
 *      – Removes or adds const/volatile qualification.
 *      – Only safe if the original object is not const.
 *
 *   4. reinterpret_cast<T>(expr)
 *      – Reinterprets bit pattern (low-level, dangerous).
 *      – e.g., int* → char*, pointer ↔ integer.
 *
 *   5. C-style cast: (T)expr  or  T(expr)
 *      – Tries const_cast, static_cast, reinterpret_cast in sequence.
 *      – Dangerous and hard to search for in code.
 *
 * IMPLICIT vs EXPLICIT:
 *   Implicit: double d = 3.14; int i = d;  (narrowing, warning)
 *   Explicit: int i = static_cast<int>(d);
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output demonstrates each cast type.
 *
 * TIME COMPLEXITY:
 *   static_cast / reinterpret_cast / const_cast: O(1)
 *   dynamic_cast: O(1) to O(n) in worst case (hierarchy walk)
 *
 * SPACE COMPLEXITY: O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: When should you use static_cast vs dynamic_cast?
 *   A: static_cast for upcasts and non-polymorphic conversions;
 *      dynamic_cast for polymorphic downcasts with runtime safety.
 *   Q: Why avoid C-style casts?
 *   A: They can perform unsafe reinterpret_cast silently, are hard
 *      to search for, and break const-correctness inadvertently.
 *   Q: Can const_cast be used to modify a truly const object?
 *   A: No — that's undefined behavior. Only use it for objects that
 *      are non-const but accessed through a const pointer/reference.
 *
 * COMMON MISTAKES:
 *   - Using reinterpret_cast when static_cast suffices.
 *   - Casting away const and modifying a genuinely const object.
 *   - Using dynamic_cast on non-polymorphic types.
 *   - Using C-style casts in C++ code.
 * ====================================================================
 */

#include <iostream>
#include <typeinfo>

class Base
{
public:
    virtual ~Base() = default;
    void speak() const { std::cout << "Base\n"; }
};

class Derived : public Base
{
public:
    void speak() const { std::cout << "Derived\n"; }
    void extra() const { std::cout << "Extra Derived method\n"; }
};

int main()
{
    std::cout << "=== static_cast ===\n";
    double pi = 3.14159;
    int int_pi = static_cast<int>(pi);
    std::cout << "static_cast<int>(pi) = " << int_pi << '\n';

    void* vptr = &pi;
    double* dptr = static_cast<double*>(vptr);
    std::cout << "static_cast<double*>(void*) = " << *dptr << '\n';

    // Upcast (safe, implicit works too)
    Derived d;
    Base& base_ref = static_cast<Base&>(d);
    base_ref.speak();

    // Downcast (no runtime check)
    Base* bp = &d;
    Derived* dp = static_cast<Derived*>(bp);
    dp->extra();

    // ---- dynamic_cast ----
    std::cout << "\n=== dynamic_cast ===\n";
    Base* b1 = new Derived();
    Base* b2 = new Base();

    Derived* d1 = dynamic_cast<Derived*>(b1);
    Derived* d2 = dynamic_cast<Derived*>(b2);

    std::cout << "dynamic_cast b1 (is Derived): "
              << (d1 ? "success" : "fail") << '\n';
    if (d1) d1->extra();

    std::cout << "dynamic_cast b2 (is Base): "
              << (d2 ? "success" : "fail") << '\n';

    delete b1;
    delete b2;

    // ---- const_cast ----
    std::cout << "\n=== const_cast ===\n";
    const int cx = 42;
    // int& rx = const_cast<int&>(cx); // WRONG: UB if modified
    // rx = 99;

    int mutable_val = 10;
    const int* cp = &mutable_val;
    int* p = const_cast<int*>(cp);
    *p = 20;  // OK: original object is not const
    std::cout << "Mutable val after const_cast write: " << mutable_val << '\n';

    // ---- reinterpret_cast ----
    std::cout << "\n=== reinterpret_cast ===\n";
    int raw = 0x00414243;  // little-endian: 'C', 'B', 'A', 0x00
    char* chars = reinterpret_cast<char*>(&raw);
    std::cout << "reinterpret int as char[4]: ";
    for (int i = 0; i < 4; ++i)
        std::cout << (chars[i] >= 32 ? chars[i] : '.');
    std::cout << '\n';

    // ---- C-style cast (dangerous) ----
    std::cout << "\n=== C-style cast (avoid) ===\n";
    double val = 9.99;
    int truncated = (int)val;  // same as static_cast here, but dangerous
    std::cout << "C-style cast: " << truncated << '\n';

    // ---- Implicit vs explicit ----
    std::cout << "\n=== Implicit vs explicit ===\n";
    double dbl = 3.14;
    int impl = dbl;                  // implicit — narrowing, may warn
    int expl = static_cast<int>(dbl); // explicit — intent clear
    std::cout << "Implicit: " << impl << ", Explicit: " << expl << '\n';

    return 0;
}
