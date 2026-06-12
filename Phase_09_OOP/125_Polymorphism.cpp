/*
THEORY: Polymorphism — Static vs Dynamic

Polymorphism: ability to present the same interface for different data types.

1. Static (compile-time) polymorphism:
   - Function overloading (same name, different params).
   - Template (compile-time duck typing).
   - Operator overloading.
   - No runtime overhead; resolved at compile time.

2. Dynamic (runtime) polymorphism:
   - Virtual functions and inheritance.
   - Virtual table (vtable) dispatch at runtime.
   - Requires pointer/reference to base class.
   - Runtime overhead (vtable lookup, cannot inline).

| Feature              | Static Polymorphism | Dynamic Polymorphism |
|----------------------|-------------------|---------------------|
| Resolution time      | Compile time      | Runtime             |
| Mechanism            | Overloading/templates | Virtual functions   |
| Performance          | Zero overhead     | Vtable lookup (~2 instructions) |
| Flexibility          | Types known at compile time | Runtime type flexibility |
| Code size            | Larger (template bloat) | Smaller             |
*/

#include <iostream>
#include <vector>
#include <memory>

// ===== Static Polymorphism =====

// 1. Function overloading
void print(int i) { std::cout << "int: " << i << '\n'; }
void print(double d) { std::cout << "double: " << d << '\n'; }
void print(const std::string& s) { std::cout << "string: " << s << '\n'; }

// 2. Template (compile-time duck typing)
template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

// 3. CRTP (Curiously Recurring Template Pattern) — static polymorphism
template <typename Derived>
class ShapeBase {
public:
    double area() const {
        return static_cast<const Derived*>(this)->area_impl();
    }
};

class Square : public ShapeBase<Square> {
    double side_;
public:
    explicit Square(double s) : side_{s} {}
    double area_impl() const { return side_ * side_; }
};

class Triangle : public ShapeBase<Triangle> {
    double base_, height_;
public:
    Triangle(double b, double h) : base_{b}, height_{h} {}
    double area_impl() const { return 0.5 * base_ * height_; }
};

// ===== Dynamic Polymorphism =====

class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const = 0;
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Meow!\n"; }
};

void makeSound(const Animal& a) {
    a.speak();  // dynamic dispatch
}

int main() {
    // 1. Static: function overloading
    std::cout << "=== Function Overloading ===\n";
    print(42);
    print(3.14);
    print("Hello");

    // 2. Static: templates
    std::cout << "\n=== Templates ===\n";
    std::cout << "max(3, 7): " << max(3, 7) << '\n';
    std::cout << "max(3.14, 2.71): " << max(3.14, 2.71) << '\n';

    // 3. Static: CRTP
    std::cout << "\n=== CRTP (Static Polymorphism) ===\n";
    Square sq{5.0};
    Triangle tri{4.0, 3.0};
    std::cout << "Square area: " << sq.area() << '\n';
    std::cout << "Triangle area: " << tri.area() << '\n';

    // 4. Dynamic: virtual functions
    std::cout << "\n=== Dynamic Polymorphism ===\n";
    Dog dog;
    Cat cat;
    makeSound(dog);
    makeSound(cat);

    // 5. Dynamic: polymorphic container
    std::cout << "\n=== Polymorphic Container ===\n";
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());
    animals.push_back(std::make_unique<Dog>());

    for (const auto& a : animals) {
        a->speak();
    }

    // 6. Comparison: performance implications
    std::cout << "\n=== Comparison ===\n";
    std::cout << "Static: no vtable, inlinable, faster\n";
    std::cout << "Dynamic: vtable dispatch (~2 ptr lookups), not inlineable\n";

    return 0;
}

/*
SAMPLE I/O:
=== Function Overloading ===
int: 42
double: 3.14
string: Hello

=== Templates ===
max(3, 7): 7
max(3.14, 2.71): 3.14

=== CRTP (Static Polymorphism) ===
Square area: 25
Triangle area: 6

=== Dynamic Polymorphism ===
Woof!
Meow!

=== Polymorphic Container ===
Woof!
Meow!
Woof!

=== Comparison ===
Static: no vtable, inlinable, faster
Dynamic: vtable dispatch (~2 ptr lookups), not inlineable

COMPLEXITY:
- Static: resolved at compile time, zero runtime overhead
- Dynamic: O(1) vtable lookup per call (cannot inline)

INTERVIEW QUESTIONS:
1. Static vs dynamic polymorphism — when to use each?
   Static: performance-critical, types known at compile time, compile-time
   polymorphism needed (CRTP, templates).
   Dynamic: runtime type selection, plugin architectures, heterogeneous containers.

2. What is CRTP and why use it?
   Curiously Recurring Template Pattern: class Derived : Base<Derived>.
   Achieves static polymorphism without virtual function overhead.
   Used in ATL, WTL, Boost.Iterator, etc.

3. Can templates be used for runtime polymorphism?
   No — templates are compile-time only. For runtime polymorphism,
   use virtual functions or std::variant + std::visit (type erasure).

COMMON MISTAKES:
- Using dynamic polymorphism when static suffices (unnecessary overhead)
- Forgetting virtual destructor in base class (UB)
- Object slicing when passing by value instead of pointer/reference
- Assuming templates are always faster (code bloat, instruction cache pressure)
- Mixing static and dynamic incorrectly (e.g., calling virtual in template)
- Not using override specifier (typos in virtual function signatures)
*/
