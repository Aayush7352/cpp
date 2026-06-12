// ============================================================================
// Phase 15 — Templates
// Topic 186: CRTP – Curiously Recurring Template Pattern
// ============================================================================
//
// THEORY:
// --------
// CRTP: A class Derived inherits from a base template instantiated with
// Derived itself: class Derived : public Base<Derived> { ... };
//
// This enables static polymorphism (compile-time virtual dispatch) —
// the base class can call derived class methods through the template
// parameter without virtual functions (zero overhead).
//
// CRTP is used for:
//   - Static polymorphism (compile-time virtual functions)
//   - Mixins (adding functionality via inheritance)
//   - Object counters
//   - Singleton pattern
//   - Barton–Nackman trick (interface injection)
//   - std::enable_shared_from_this
//
// ============================================================================

#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>

// ==================== 1. Static Polymorphism ====================

template <typename Derived>
struct Shape
{
    // Static dispatch: downcast to Derived and call area_impl
    double area() const
    {
        return static_cast<const Derived&>(*this).area_impl();
    }

    double perimeter() const
    {
        return static_cast<const Derived&>(*this).perimeter_impl();
    }

    void describe() const
    {
        std::cout << "Area: " << area()
                  << ", Perimeter: " << perimeter() << "\n";
    }
};

struct Circle : Shape<Circle>
{
    double radius;

    Circle(double r) : radius{r} {}

    double area_impl() const
    {
        return 3.14159 * radius * radius;
    }

    double perimeter_impl() const
    {
        return 2.0 * 3.14159 * radius;
    }
};

struct Rectangle : Shape<Rectangle>
{
    double width, height;

    Rectangle(double w, double h) : width{w}, height{h} {}

    double area_impl() const
    {
        return width * height;
    }

    double perimeter_impl() const
    {
        return 2.0 * (width + height);
    }
};

// ==================== 2. CRTP Object Counter ====================

template <typename T>
struct ObjectCounter
{
    static int objects_created;
    static int objects_alive;

    ObjectCounter()  { ++objects_created; ++objects_alive; }
    ~ObjectCounter() { --objects_alive; }

    ObjectCounter(const ObjectCounter&) { ++objects_created; ++objects_alive; }
    ObjectCounter(ObjectCounter&&) noexcept
    {
        ++objects_created;
        ++objects_alive;
    }

    static int created() { return objects_created; }
    static int alive()   { return objects_alive; }
};

template <typename T>
int ObjectCounter<T>::objects_created = 0;

template <typename T>
int ObjectCounter<T>::objects_alive = 0;

struct Widget : ObjectCounter<Widget>
{
    int id;
    explicit Widget(int i) : id{i} {}
};

struct Gadget : ObjectCounter<Gadget>
{
    std::string name;
    explicit Gadget(std::string n) : name{std::move(n)} {}
};

// ==================== 3. CRTP Mixin (adding operator<) ====================

template <typename Derived>
struct Comparable
{
    friend bool operator==(const Derived& a, const Derived& b)
    {
        return static_cast<const Derived&>(a).compare_to(b) == 0;
    }
    friend bool operator!=(const Derived& a, const Derived& b)
    {
        return !(a == b);
    }
    friend bool operator<(const Derived& a, const Derived& b)
    {
        return static_cast<const Derived&>(a).compare_to(b) < 0;
    }
    friend bool operator>(const Derived& a, const Derived& b)
    {
        return b < a;
    }
    friend bool operator<=(const Derived& a, const Derived& b)
    {
        return !(b < a);
    }
    friend bool operator>=(const Derived& a, const Derived& b)
    {
        return !(a < b);
    }
};

struct Point : Comparable<Point>
{
    int x, y;

    Point(int x, int y) : x{x}, y{y} {}

    int compare_to(const Point& other) const
    {
        if (x != other.x) return x - other.x;
        return y - other.y;
    }
};

// ==================== 4. CRTP with enable_shared_from_this (standard) ====
class MyShared : public std::enable_shared_from_this<MyShared>
{
public:
    std::shared_ptr<MyShared> get_shared()
    {
        return shared_from_this();
    }
    void greet() const { std::cout << "Hello from MyShared\n"; }
};

// ============================================================================
// SAMPLE I/O:
// -----------
// Circle (r=5): Area: 78.5397, Perimeter: 31.4159
// Rectangle (3x4): Area: 12, Perimeter: 14
// Widget: created=3, alive=1
// Gadget: created=2, alive=2
// Point(1,2) < Point(3,4): true
// Point(1,2) == Point(1,2): true
// Point(1,2) > Point(1,1): true
// ============================================================================

int main()
{
    // Static polymorphism
    Circle c{5.0};
    Rectangle r{3.0, 4.0};

    std::cout << "Circle (r=5): ";
    c.describe();
    std::cout << "Rectangle (3x4): ";
    r.describe();

    // Object counter
    {
        Widget w1{1}, w2{2}, w3{3};
        Gadget g1{"gadget1"}, g2{"gadget2"};

        std::cout << "Widget: created="  << Widget::created()
                  << ", alive=" << Widget::alive() << "\n";
        std::cout << "Gadget: created=" << Gadget::created()
                  << ", alive=" << Gadget::alive() << "\n";
    }
    std::cout << "Widget after scope: alive=" << Widget::alive() << "\n";

    // Comparable mixin
    Point p1{1, 2}, p2{3, 4}, p3{1, 2};

    std::cout << "Point(1,2) < Point(3,4): "
              << std::boolalpha << (p1 < p2) << "\n";
    std::cout << "Point(1,2) == Point(1,2): "
              << (p1 == p3) << "\n";
    std::cout << "Point(1,2) > Point(1,1): "
              << (p1 > Point{1, 1}) << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// Static polymorphism: zero runtime overhead (no vtable)
// Object counter: O(1) construction/destruction
// Comparable mixin: O(1) comparison
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What problem does CRTP solve?
// A1: It enables static (compile-time) polymorphism, avoiding vtable
//     overhead (no virtual dispatch), enabling inlining, and providing
//     mixin-style code reuse without runtime costs.
//
// Q2: How does the base class call derived class methods?
// A2: Through static_cast<Derived&>(*this) — at compile time, the
//     base knows the exact derived type from the template parameter.
//     No virtual function table lookup needed.
//
// Q3: What are CRTP mixins?
// A3: Mixins are CRTP base classes that add functionality (e.g.,
//     comparison operators, serialization, cloning) to derived classes
//     with zero overhead. The derived class provides a few primitives;
//     the mixin provides the rest.
//
// COMMON MISTAKES:
// ----------------
// - Forgetting the template parameter when inheriting (class Circle : Shape
//   instead of Shape<Circle>).
// - Calling derived methods from the base destructor (UB — derived already
//   destroyed).
// - Not marking the base destructor as protected (to prevent slicing).
// - Using CRTP when virtual dispatch (runtime polymorphism) is actually
//   needed (e.g., heterogeneous containers of base pointers).
// ============================================================================
