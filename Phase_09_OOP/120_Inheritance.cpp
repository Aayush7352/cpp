/*
THEORY: Inheritance Basics

Inheritance: deriving a new class from an existing one (reuse, extension, polymorphism).

Syntax: class Derived : access_specifier Base { ... };

Access Control:
| Base member access | public inheritance | protected inheritance | private inheritance |
|---------------------|--------------------|----------------------|---------------------|
| public              | public             | protected            | private             |
| protected           | protected          | protected            | private             |
| private             | inaccessible       | inaccessible         | inaccessible        |

- public inheritance: "is-a" relationship (most common).
- protected inheritance: "implemented-in-terms-of" (rare).
- private inheritance: "implemented-in-terms-of" (composition preferred).

Constructors: base class constructor called first (in initializer list).
Destructors: reverse order (derived destroyed first).
*/

#include <iostream>
#include <string>

// Base class
class Animal {
protected:
    std::string name_;

public:
    Animal(const std::string& name) : name_{name} {
        std::cout << "Animal ctor: " << name_ << '\n';
    }

    virtual ~Animal() { std::cout << "Animal dtor: " << name_ << '\n'; }

    virtual void speak() const {
        std::cout << name_ << " makes a sound\n";
    }

    std::string getName() const { return name_; }
};

// Public inheritance — "is-a"
class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal{name} {
        std::cout << "Dog ctor: " << name_ << '\n';
    }

    ~Dog() override { std::cout << "Dog dtor: " << name_ << '\n'; }

    void speak() const override {
        std::cout << name_ << " barks: Woof!\n";
    }

    void fetch() const {
        std::cout << name_ << " fetches the stick\n";
    }
};

// Protected inheritance
class ProtectedBase {
public:
    int pub{1};
protected:
    int prot{2};
private:
    int priv{3};
};

class ProtectedDerived : protected ProtectedBase {
    // pub becomes protected, prot stays protected, priv inaccessible
public:
    int getPub() const { return pub; }
    int getProt() const { return prot; }
    // int getPriv() const { return priv; } // ERROR
};

// Private inheritance
class PrivateDerived : private ProtectedBase {
    // pub and prot become private, priv inaccessible
public:
    int getPub() const { return pub; }
    int getProt() const { return prot; }
};

int main() {
    // 1. Public inheritance
    std::cout << "=== Public Inheritance ===\n";
    Dog dog{"Buddy"};
    dog.speak();           // Dog::speak
    dog.fetch();           // Dog specific

    // Animal pointer can point to Dog
    Animal* animalPtr{&dog};
    animalPtr->speak();    // polymorphic: Dog::speak

    // 2. Access control
    std::cout << "\n=== Access Control ===\n";
    ProtectedDerived pd;
    std::cout << "ProtectedDerived::getPub() = " << pd.getPub() << '\n';
    // std::cout << pd.pub;  // ERROR: protected in derived

    PrivateDerived privd;
    std::cout << "PrivateDerived::getPub() = " << privd.getPub() << '\n';

    // 3. Slicing problem
    std::cout << "\n=== Object Slicing ===\n";
    Dog terrier{"Max"};
    Animal animal{terrier};    // slicing! only Animal part copied
    animal.speak();            // Animal::speak, not Dog::speak

    // 4. Upcasting is safe, downcasting is not
    std::cout << "\n=== Upcasting / Downcasting ===\n";
    Dog* dogPtr{&dog};
    Animal* basePtr{dogPtr};   // upcast: implicit, safe
    // Dog* back{dynamic_cast<Dog*>(basePtr)}; // downcast: needs virtual base

    return 0;
}

/*
SAMPLE I/O:
=== Public Inheritance ===
Animal ctor: Buddy
Dog ctor: Buddy
Buddy barks: Woof!
Buddy fetches the stick
Buddy barks: Woof!
Dog dtor: Buddy
Animal dtor: Buddy

=== Access Control ===
ProtectedDerived::getPub() = 1
PrivateDerived::getPub() = 1

=== Object Slicing ===
Animal ctor: Max
Animal dtor: Max
Max makes a sound
Animal dtor: Max

=== Upcasting / Downcasting ===

COMPLEXITY:
- Constructor/Destructor: O(1) for each level
- Virtual dispatch: O(1) vtable lookup

INTERVIEW QUESTIONS:
1. What is public vs private inheritance?
   Public: "is-a" — derived IS a base. (Animal -> Dog)
   Private: "implemented-in-terms-of" — hides inheritance. Prefer composition.

2. What is object slicing?
   When a derived object is assigned to a base object by value. The derived part
   is "sliced off". Use pointers/references to avoid.

3. Can a derived class access private members of base?
   No. Private members are never accessible outside the class. Use protected
   for derived access or provide protected getters/setters.

COMMON MISTAKES:
- Using private/protected inheritance when public is intended
- Object slicing (passing by value instead of pointer/ref)
- Not calling base class constructor in initializer list (default ctor must exist)
- Forgetting virtual destructor in polymorphic base classes
- Downcasting without dynamic_cast (UB if wrong type)
- Making inheritance hierarchy too deep (fragile base class problem)
*/
