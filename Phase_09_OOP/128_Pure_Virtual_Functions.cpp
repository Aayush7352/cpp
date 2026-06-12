/*
THEORY: Pure Virtual Functions

Pure virtual function: virtual function with no implementation (= 0).
Syntax: virtual returnType functionName(params) = 0;

Abstract Base Class (ABC): class with at least one pure virtual function.
- Cannot be instantiated.
- Derived classes MUST override all pure virtuals (or be abstract too).
- Can have regular member functions and data members.

Pure virtual destructor: MUST provide a body (even though it's pure).
- Reason: derived destructors will call base destructor (which needs a body).
- Makes class abstract without forcing any other pure virtual.

= 0 can appear on any virtual function, including the destructor.
*/

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// --- Abstract Base Class ---
class IShape {
public:
    // Pure virtual functions
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // Regular member function
    virtual std::string description() const {
        return "Shape with area " + std::to_string(area());
    }

    // Pure virtual destructor — MUST have body
    virtual ~IShape() = 0;
};

IShape::~IShape() {
    std::cout << "IShape dtor\n";
}

class Circle final : public IShape {
    double radius_;
public:
    explicit Circle(double r) : radius_{r} {}
    double area() const override { return 3.14159 * radius_ * radius_; }
    double perimeter() const override { return 2.0 * 3.14159 * radius_; }
    std::string description() const override {
        return "Circle(r=" + std::to_string(radius_) + ") " + IShape::description();
    }
    ~Circle() override { std::cout << "Circle dtor\n"; }
};

class Rectangle final : public IShape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_{w}, h_{h} {}
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2.0 * (w_ + h_); }
    std::string description() const override {
        return "Rectangle(" + std::to_string(w_) + "x" + std::to_string(h_) + ") "
               + IShape::description();
    }
    ~Rectangle() override { std::cout << "Rectangle dtor\n"; }
};

// --- Pure virtual via destructor only ---
class ILoggable {
public:
    virtual ~ILoggable() = 0;   // makes class abstract
    virtual std::string log() const { return "Log entry"; }
};
ILoggable::~ILoggable() = default;

class Logger : public ILoggable {
public:
    ~Logger() override = default;
    std::string log() const override { return "Logger entry"; }
};

int main() {
    // 1. Cannot instantiate abstract class
    // IShape s;                     // ERROR: abstract class

    // 2. Derived concrete classes
    std::cout << "=== Concrete Shapes ===\n";
    Circle c{5.0};
    Rectangle r{4.0, 6.0};

    std::cout << c.description() << '\n';
    std::cout << "  Area: " << c.area() << ", Perim: " << c.perimeter() << '\n';

    std::cout << r.description() << '\n';
    std::cout << "  Area: " << r.area() << ", Perim: " << r.perimeter() << '\n';

    // 3. Polymorphic container
    std::cout << "\n=== Container of IShape* ===\n";
    std::vector<std::unique_ptr<IShape>> shapes;
    shapes.push_back(std::make_unique<Circle>(3.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 4.0));

    for (const auto& s : shapes) {
        std::cout << s->description() << '\n';
    }

    // 4. Pure virtual destructor
    std::cout << "\n=== Pure Virtual Dtor ===\n";
    Logger log;
    ILoggable& ilog{log};
    std::cout << ilog.log() << '\n';

    // 5. Forcing implementation
    std::cout << "\n=== Partial implementation ===\n";
    struct Abstractish : IShape {
        // Only overrides area, not perimeter — still abstract
        double area() const override { return 0; }
        // Can't instantiate this
    };

    return 0;
}

/*
SAMPLE I/O:
=== Concrete Shapes ===
Circle(r=5.000000) Shape with area 78.539750
  Area: 78.5397, Perim: 31.4159
Rectangle(4.000000x6.000000) Shape with area 24.000000
  Area: 24, Perim: 20

=== Container of IShape* ===
Circle(r=3.000000) Shape with area 28.274310
Rectangle(2.000000x4.000000) Shape with area 8.000000

=== Pure Virtual Dtor ===
Logger entry

=== Partial implementation ===
(Compiles, but Abstractish still abstract — can't instantiate)

COMPLEXITY:
- Same as virtual functions: O(1) vtable dispatch
- Abstract class: no overhead beyond virtual functions

INTERVIEW QUESTIONS:
1. What is an abstract class?
   A class with at least one pure virtual function. Cannot be instantiated.
   Serves as an interface or base contract for derived classes.

2. Why does a pure virtual destructor need a body?
   Derived class destructor always calls base destructor. Even though the
   destructor is pure virtual, it must have an implementation for the chain.
   It makes the class abstract without requiring other pure virtual functions.

3. Can you call a pure virtual function from constructor/destructor?
   It's undefined behavior. The function may not have an implementation, or
   vptr may not be pointing to the right vtable.

COMMON MISTAKES:
- Forgetting to implement a pure virtual function in derived (class stays abstract)
- Not providing a body for pure virtual destructor (linker error)
- Trying to instantiate an abstract class (compiler error)
- Defining pure virtual function with = 0 but also providing a body (possible,
  but rarely useful — only for Base::func() calls from Derived)
- Using abstract class where concrete class would suffice (unnecessary complexity)
- Not making destructor virtual in abstract class (UB)
*/
