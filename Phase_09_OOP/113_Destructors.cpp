/*
THEORY: Destructors

- Destructor: ~ClassName() — called when object goes out of scope or is deleted.
- No parameters, no return type, cannot be overloaded.
- Virtual destructor: needed when deleting derived object through base pointer.
- RAII (Resource Acquisition Is Initialization): acquire in constructor, release in destructor.
- Order: destructors run in reverse order of construction (members in reverse declaration order).

Stack unwinding: during exception propagation, all local objects are destroyed.
*/

#include <iostream>
#include <string>
#include <memory>

class Resource {
    std::string name_;
public:
    explicit Resource(const std::string& name) : name_{name} {
        std::cout << "  Acquire " << name_ << '\n';
    }
    ~Resource() {
        std::cout << "  Release " << name_ << '\n';
    }
};

class Base {
public:
    Base() { std::cout << "Base ctor\n"; }
    virtual ~Base() { std::cout << "Base dtor\n"; }  // virtual!
    virtual void speak() const { std::cout << "Base speaks\n"; }
};

class Derived : public Base {
    Resource res{"DerivedResource"};
public:
    Derived() { std::cout << "Derived ctor\n"; }
    ~Derived() override { std::cout << "Derived dtor\n"; }
    void speak() const override { std::cout << "Derived speaks\n"; }
};

class NonVirtualBase {
public:
    NonVirtualBase() { std::cout << "NonVirtualBase ctor\n"; }
    ~NonVirtualBase() { std::cout << "NonVirtualBase dtor\n"; }
};

class NonVirtualDerived : public NonVirtualBase {
    Resource res{"Leaked?"};
public:
    NonVirtualDerived() { std::cout << "NonVirtualDerived ctor\n"; }
    ~NonVirtualDerived() { std::cout << "NonVirtualDerived dtor\n"; }
};

int main() {
    // 1. Basic destructor
    std::cout << "=== RAII destructor ===\n";
    {
        Resource r{"FileHandle"};
        std::cout << "  Using resource...\n";
    } // r destroyed here

    // 2. Virtual destructor — proper cleanup
    std::cout << "\n=== Virtual destructor (correct) ===\n";
    Base* bp{new Derived()};
    bp->speak();
    delete bp;  // calls Derived dtor then Base dtor

    // 3. Non-virtual destructor — UB (partial cleanup)
    std::cout << "\n=== Non-virtual destructor (WRONG) ===\n";
    NonVirtualBase* nbp{new NonVirtualDerived()};
    delete nbp;  // Only NonVirtualBase dtor called! Resource not released.
    // This is UNDEFINED BEHAVIOR if NonVirtualDerived has non-trivial destructor.

    // 4. Order of destruction
    std::cout << "\n=== Destruction order ===\n";
    {
        Resource a{"A"};
        Resource b{"B"};
        std::cout << "  Inside inner scope\n";
    } // b released, then a (reverse order)

    // 5. Destructor and exceptions (should never throw)
    std::cout << "\n=== Destructor noexcept ===\n";
    struct Safe {
        ~Safe() noexcept {
            try {
                // cleanup that might fail
            } catch (...) {
                // destructors should swallow all exceptions
            }
        }
    };

    return 0;
}

/*
SAMPLE I/O:
=== RAII destructor ===
  Acquire FileHandle
  Using resource...
  Release FileHandle

=== Virtual destructor (correct) ===
Base ctor
  Acquire DerivedResource
Derived ctor
Derived speaks
Derived dtor
  Release DerivedResource
Base dtor

=== Non-virtual destructor (WRONG) ===
NonVirtualBase ctor
  Acquire Leaked?
NonVirtualDerived ctor
NonVirtualBase dtor
(Note: Resource 'Leaked?' was NOT released!)

=== Destruction order ===
  Acquire A
  Acquire B
  Inside inner scope
  Release B
  Release A

COMPLEXITY:
- Destructor: O(1) or O(N) depending on members
- Virtual destructor dispatch: O(1) via vtable

INTERVIEW QUESTIONS:
1. Why must the destructor be virtual in a base class?
   When deleting derived object via base pointer, ensures the correct
   destructor chain runs. Without virtual, only base destructor executes
   (UB for non-trivial derived members).

2. Can a destructor throw?
   Yes (before C++11), but it's extremely dangerous. If destructor throws
   during stack unwinding (another exception active), std::terminate is called.
   Destructors are noexcept by default in C++11+.

3. What is the order of destruction?
   Reverse of construction: derived destructor runs first, then base destructor.
   Members destroyed in reverse declaration order.

COMMON MISTAKES:
- Forgetting virtual destructor in polymorphic base classes (UB)
- Throwing exceptions from destructors (std::terminate if during stack unwinding)
- Calling virtual functions in destructors (static dispatch only — derived part already gone)
- Not deleting dynamically allocated members (memory leak)
- Double deletion (calling delete on the same pointer twice)
*/
