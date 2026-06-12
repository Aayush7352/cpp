/*
THEORY: Factory Pattern
- Creational design pattern for object creation
- Factory Method: virtual creator in base class
- Abstract Factory: factory of factories
- Registration-based: register types by name for runtime creation
- Decouples client code from concrete classes

Complexity: O(1) creation via registry
Interview Questions:
1. Factory vs Builder pattern difference?
2. When to use Abstract Factory vs Factory Method?
3. How to handle complex object graphs?
Common Mistakes:
- Over-engineering with factories for simple cases
- Forgetting to register types
- Not handling factory errors gracefully
*/

#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

// Product hierarchy
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual double area() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r = 1.0) : radius_(r) {}
    void draw() const override { std::cout << "Circle(r=" << radius_ << ")\n"; }
    double area() const override { return 3.14159 * radius_ * radius_; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w = 1, double h = 1) : w_(w), h_(h) {}
    void draw() const override { std::cout << "Rectangle(w=" << w_ << ",h=" << h_ << ")\n"; }
    double area() const override { return w_ * h_; }
};

// Registration-based factory
class ShapeFactory {
    using Creator = std::function<std::unique_ptr<Shape>()>;
    std::unordered_map<std::string, Creator> registry_;

    ShapeFactory() = default;
public:
    static ShapeFactory& instance() {
        static ShapeFactory factory;
        return factory;
    }

    template<typename T>
    void registerType(const std::string& name) {
        registry_[name] = []() { return std::make_unique<T>(); };
    }

    void registerType(const std::string& name, Creator creator) {
        registry_[name] = std::move(creator);
    }

    std::unique_ptr<Shape> create(const std::string& name) const {
        auto it = registry_.find(name);
        if (it == registry_.end()) return nullptr;
        return it->second();
    }

    std::vector<std::string> availableTypes() const {
        std::vector<std::string> types;
        for (const auto& [name, _] : registry_) types.push_back(name);
        return types;
    }
};

// Auto-registration helper
template<typename T>
struct Registrar {
    explicit Registrar(const std::string& name) {
        ShapeFactory::instance().registerType<T>(name);
    }
};

static Registrar<Circle> circleReg("circle");
static Registrar<Rectangle> rectReg("rectangle");

int main() {
    auto& factory = ShapeFactory::instance();
    factory.registerType("big_circle", []() { 
        return std::make_unique<Circle>(10.0); 
    });

    std::cout << "Available shapes:\n";
    for (const auto& type : factory.availableTypes()) {
        std::cout << "  - " << type << "\n";
    }

    for (const auto& name : {"circle", "rectangle", "big_circle", "triangle"}) {
        auto shape = factory.create(name);
        if (shape) {
            std::cout << name << ": ";
            shape->draw();
            std::cout << "  Area: " << shape->area() << "\n";
        } else {
            std::cout << name << ": not registered\n";
        }
    }
    return 0;
}

/*
SAMPLE OUTPUT:
Available shapes:
  - circle
  - rectangle
  - big_circle
circle: Circle(r=1)
  Area: 3.14159
rectangle: Rectangle(w=1,h=1)
  Area: 1
big_circle: Circle(r=10)
  Area: 314.159
triangle: not registered
*/
