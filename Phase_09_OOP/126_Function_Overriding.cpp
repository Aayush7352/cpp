/*
THEORY: Function Overriding

Function overriding: derived class redefines a virtual function from base class.

Key rules:
- Base function must be virtual.
- Derived function signature must match exactly (same name, params, constness, return type).
- override specifier (C++11): compiler checks that it actually overrides a base virtual.
- final specifier (C++11): prevents further overriding in derived classes.

Without override: accidentally creating a new function (different signature) instead
of overriding. Compiler won't warn (name hiding instead of overriding).

final on class: prevents inheritance entirely.
final on function: prevents further overriding in any derived class.
*/

#include <iostream>
#include <string>

class Base {
public:
    virtual ~Base() = default;

    virtual void show() const {
        std::cout << "Base::show()\n";
    }

    virtual void display(int x) const {
        std::cout << "Base::display(" << x << ")\n";
    }

    // Not virtual — can't be overridden
    void nonVirtual() const {
        std::cout << "Base::nonVirtual()\n";
    }

    virtual void interface() const = 0;
};

// --- Correct overriding with override ---
class Derived : public Base {
public:
    // Correct override
    void show() const override {
        std::cout << "Derived::show()\n";
    }

    // Compiler error if signature doesn't match:
    // void display(double x) const override;  // ERROR: doesn't override
    void display(int x) const override {
        std::cout << "Derived::display(" << x << ")\n";
    }

    // Incorrect: missing const — this HIDES Base::show(), doesn't override
    // void show() { }  // different signature (no const)

    // Missing override — might accidentally hide instead of override
    // void display(int x) { }  // different constness — warning with -Wsuggest-override

    void interface() const override {
        std::cout << "Derived::interface() implementation\n";
    }
};

// --- final function ---
class BaseWithFinal {
public:
    virtual void normal() const { std::cout << "BaseWithFinal::normal\n"; }
    virtual void locked() const final { std::cout << "BaseWithFinal::locked (final)\n"; }
};

class Child final : public BaseWithFinal {
public:
    void normal() const override { std::cout << "Child::normal\n"; }
    // void locked() const override { }  // ERROR: locked is final
};

// class GrandChild : public Child { };  // ERROR: Child is final

// --- Covariant return types ---
class Shape {
public:
    virtual ~Shape() = default;
    virtual Shape* clone() const {
        return new Shape(*this);
    }
};

class Circle : public Shape {
public:
    Circle* clone() const override {  // covariant return type (Circle* instead of Shape*)
        return new Circle(*this);
    }
};

int main() {
    // 1. Correct overriding
    std::cout << "=== Correct Override ===\n";
    Derived d;
    Base& b{d};
    b.show();
    b.display(42);
    b.interface();

    // 2. Non-virtual function is not overridden
    std::cout << "\n=== Non-virtual (no override) ===\n";
    b.nonVirtual();        // Base::nonVirtual — no dynamic dispatch
    d.nonVirtual();        // Base::nonVirtual (inherited)

    // 3. final specifier
    std::cout << "\n=== final function ===\n";
    Child c;
    BaseWithFinal& bf{c};
    bf.normal();
    bf.locked();            // final function called via base

    // 4. Without override (accidental hiding)
    std::cout << "\n=== Accidental Hiding ===\n";
    struct DerivedBad : Base {
        // int show() const { return 1; }   // different return type — hiding, not override
        void show(int x) const { }          // different params — hiding
    };

    // 5. Covariant return type
    std::cout << "\n=== Covariant Return ===\n";
    Circle circ;
    Circle* cloned{circ.clone()};
    std::cout << "Cloned Circle (covariant return type)\n";
    delete cloned;

    return 0;
}

/*
SAMPLE I/O:
=== Correct Override ===
Derived::show()
Derived::display(42)
Derived::interface() implementation

=== Non-virtual (no override) ===
Base::nonVirtual()
Base::nonVirtual()

=== final function ===
Child::normal
BaseWithFinal::locked (final)

=== Accidental Hiding ===
(Compiles but DerivedBad::show hides Base::show without overriding)

=== Covariant Return ===
Cloned Circle (covariant return type)

COMPLEXITY:
- Virtual dispatch: O(1) vtable lookup
- Override resolution: compile-time

INTERVIEW QUESTIONS:
1. What does the 'override' specifier do?
   Explicitly marks a function as overriding a base virtual. Compiler
   checks the signature matches. Prevents accidental hiding due to
   signature mismatch.

2. What is the 'final' specifier?
   On function: prevents further overriding in derived classes.
   On class: prevents the class from being inherited.

3. What are covariant return types?
   Override can return a more specific type (Derived* instead of Base*)
   if the types are related by inheritance. Useful for clone(), factory methods.

COMMON MISTAKES:
- Missing const in derived override (different signature = hiding, not overriding)
- Different parameter types (overloading in base vs overriding in derived)
- Forgetting override specifier (silent name hiding instead of override)
- Trying to override a non-virtual function (it's hiding, not overriding)
- Overriding a final function (compile error)
- Different return type that is not covariant (compile error)
*/
