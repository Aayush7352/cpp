/*
THEORY: Multiple Inheritance

Multiple inheritance: derived class inherits from two or more base classes.

Syntax: class Derived : public Base1, public Base2, ... { };

Diamond Problem: Base <- (Derived1, Derived2) <- DiamondDerived.
- Base constructor runs twice without virtual inheritance.
- Virtual inheritance: Base constructed once (most derived class calls Base ctor).
- Requires: class Derived1 : virtual public Base { };

Virtual inheritance:
- Ensures only one copy of Base subobject.
- Most derived class is responsible for constructing the virtual base.
- Order: virtual bases first, then non-virtual bases, then members, then derived.
*/

#include <iostream>

// --- Diamond Problem ---
class Animal {
protected:
    std::string name_;
public:
    explicit Animal(const std::string& name = "Animal") : name_{name} {
        std::cout << "Animal ctor: " << name_ << '\n';
    }
    virtual void speak() const { std::cout << name_ << " speaks\n"; }
    virtual ~Animal() = default;
};

class Winged {
public:
    Winged() { std::cout << "Winged ctor\n"; }
    virtual void fly() const { std::cout << "Flying\n"; }
    virtual ~Winged() = default;
};

// --- Without virtual inheritance (problematic) ---
class Bat : public Animal, public Winged {
public:
    Bat() : Animal{"Bat"}, Winged() {
        std::cout << "Bat ctor\n";
    }
    void speak() const override { std::cout << "Bat screeches\n"; }
};

// --- Diamond problem with virtual inheritance ---
class Base {
protected:
    int value_;
public:
    explicit Base(int v = 0) : value_{v} {
        std::cout << "Base ctor (" << value_ << ")\n";
    }
    int getValue() const { return value_; }
    virtual ~Base() = default;
};

class Mid1 : virtual public Base {
public:
    Mid1() : Base{10} { std::cout << "Mid1 ctor\n"; }
};

class Mid2 : virtual public Base {
public:
    Mid2() : Base{20} { std::cout << "Mid2 ctor\n"; }
};

class Diamond : public Mid1, public Mid2 {
public:
    // Most derived class initializes virtual base
    Diamond() : Base{42}, Mid1{}, Mid2{} {
        std::cout << "Diamond ctor\n";
    }
    // Only one Base subobject, value is 42 (most derived wins)
    int getValue() const { return value_; }
};

// --- Interface combination ---
class IReader {
public:
    virtual ~IReader() = default;
    virtual std::string read() const = 0;
};

class IWriter {
public:
    virtual ~IWriter() = default;
    virtual void write(const std::string&) = 0;
};

class FileDevice : public IReader, public IWriter {
    std::string content_;
public:
    std::string read() const override { return content_; }
    void write(const std::string& data) override { content_ = data; }
};

int main() {
    // 1. Multiple unrelated bases
    std::cout << "=== Multiple bases (Bat) ===\n";
    Bat bat;
    bat.speak();
    bat.fly();

    // Casting
    Animal* aptr{&bat};
    Winged* wptr{&bat};
    std::cout << "Animal ptr: " << aptr << ", Winged ptr: " << wptr << '\n';

    // 2. Diamond with virtual inheritance
    std::cout << "\n=== Virtual inheritance (Diamond) ===\n";
    Diamond d;
    std::cout << "Diamond::getValue() = " << d.getValue() << '\n';
    // No ambiguity: only one Base, value = 42

    // 3. Interface combination
    std::cout << "\n=== Interface combination ===\n";
    FileDevice file;
    file.write("Hello, Multiple Inheritance!");
    std::cout << "Read: " << file.read() << '\n';

    // Upcast to both interfaces
    IReader& reader{file};
    IWriter& writer{file};
    writer.write("Updated via interface");
    std::cout << "Read via IReader: " << reader.read() << '\n';

    return 0;
}

/*
SAMPLE I/O:
=== Multiple bases (Bat) ===
Animal ctor: Bat
Winged ctor
Bat ctor
Bat screeches
Flying
Animal ptr: 0x16f82b518, Winged ptr: 0x16f82b520

=== Virtual inheritance (Diamond) ===
Base ctor (42)
Mid1 ctor
Mid2 ctor
Diamond ctor
Diamond::getValue() = 42

=== Interface combination ===
Read: Hello, Multiple Inheritance!
Read via IReader: Updated via interface

COMPLEXITY:
- Virtual inheritance: overhead of vtable pointer for virtual bases
- Multiple inheritance dispatch: O(1) per base

INTERVIEW QUESTIONS:
1. What is the diamond problem and how is it solved?
   Two classes derive from same base, then a class derives from both.
   Without virtual inheritance: two copies of base (ambiguity).
   Solution: virtual inheritance (shared base subobject).

2. When should you use multiple inheritance?
   - Combining interfaces (like Java interfaces).
   - Mixin classes (small, reusable behaviors).
   - Rare for production code — often replaced by composition.

3. What is the initialization order with virtual inheritance?
   Virtual bases first (deepest first, left-to-right in depth-first order),
   then non-virtual bases (left-to-right), then members, then derived body.

COMMON MISTAKES:
- Diamond without virtual inheritance (duplicate base, ambiguous members)
- Ambiguous name resolution (use Base::member to disambiguate)
- Not calling virtual base constructor from most derived class
- Assuming non-virtual and virtual bases are interchangeable
- Overusing multiple inheritance (prefer composition)
- Complex casting with multiple bases (pointer adjustments)
*/
