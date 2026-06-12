/*
THEORY: Virtual Functions — Virtual Table, vptr, Dynamic Dispatch

Virtual function: member function whose behavior can be overridden in derived class.

Mechanism:
- Each class with virtual functions has a virtual table (vtable) — array of function pointers.
- Each object of such class has a hidden vptr pointer pointing to the class's vtable.
- Virtual call: object->vptr[index] resolves at runtime.
- Construction: vptr set during base construction, then updated for each derived.

Performance cost:
- Memory: vptr per object (typically 8 bytes).
- vtable: one per class (negligible, shared).
- Call overhead: vptr dereference + function pointer call (prevents inlining).

Virtual dispatch does NOT work during construction/destruction.
*/

#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base ctor — vptr points to Base vtable\n";
        virtualCall();  // calls Base::virtualCall (not derived!)
    }

    virtual ~Base() {
        std::cout << "Base dtor — vptr still points to Base vtable\n";
        virtualCall();  // calls Base::virtualCall
    }

    virtual void virtualCall() const {
        std::cout << "  Base::virtualCall()\n";
    }

    void nonVirtualCall() const {
        std::cout << "  Base::nonVirtualCall()\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived ctor — vptr now points to Derived vtable\n";
        virtualCall();  // calls Derived::virtualCall
    }

    ~Derived() override {
        std::cout << "Derived dtor — vptr still points to Derived vtable\n";
        virtualCall();  // calls Derived::virtualCall
    }

    void virtualCall() const override {
        std::cout << "  Derived::virtualCall()\n";
    }
};

// --- Manual vtable concept demo ---
using VirtualFunc = void(*)();

struct ManualVTable {
    VirtualFunc speak;
    VirtualFunc destroy;
};

void dogSpeak() { std::cout << "Woof!\n"; }
void dogDestroy() { std::cout << "Dog destroyed\n"; }
void catSpeak() { std::cout << "Meow!\n"; }
void catDestroy() { std::cout << "Cat destroyed\n"; }

const ManualVTable dogVTable{dogSpeak, dogDestroy};
const ManualVTable catVTable{catSpeak, catDestroy};

struct ManualAnimal {
    const ManualVTable* vptr;
};

int main() {
    // 1. Normal virtual dispatch
    std::cout << "=== Virtual Dispatch ===\n";
    Derived d;
    Base& b{d};

    std::cout << "\nCalling via Base reference:\n";
    b.virtualCall();        // Derived::virtualCall (dynamic dispatch)
    b.nonVirtualCall();     // Base::nonVirtualCall (static, resolved at compile time)

    // 2. Virtual behavior during construction/destruction
    std::cout << "\n=== Construction with virtual call ===\n";
    // Output shows virtual calls resolve to current class under construction

    // 3. Manual vtable (simulating compiler behavior)
    std::cout << "\n=== Manual VTable Simulation ===\n";
    ManualAnimal dog{&dogVTable};
    ManualAnimal cat{&catVTable};

    dog.vptr->speak();
    cat.vptr->speak();

    dog.vptr->destroy();
    cat.vptr->destroy();

    // 4. sizeof with vptr
    std::cout << "\n=== Memory Overhead ===\n";
    struct Empty {};
    struct WithVirtual {
        virtual ~WithVirtual() = default;
    };
    std::cout << "sizeof(Empty): " << sizeof(Empty) << '\n';
    std::cout << "sizeof(WithVirtual): " << sizeof(WithVirtual) << " (pointer added)\n";

    // 5. Trying to inline virtual functions
    std::cout << "\n=== Inlining & Virtual ===\n";
    std::cout << "Virtual calls cannot be inlined through base pointer/reference.\n";
    std::cout << "They CAN be inlined if the compiler knows the concrete type.\n";

    return 0;
}

/*
SAMPLE I/O:
=== Virtual Dispatch ===
Base ctor — vptr points to Base vtable
  Base::virtualCall()
Derived ctor — vptr now points to Derived vtable
  Derived::virtualCall()

Calling via Base reference:
  Derived::virtualCall()
  Base::nonVirtualCall()
Derived dtor — vptr still points to Derived vtable
  Derived::virtualCall()
Base dtor — vptr still points to Base vtable
  Base::virtualCall()

=== Construction with virtual call ===
(Base ctor -> Base::virtualCall; Derived ctor -> Derived::virtualCall)

=== Manual VTable Simulation ===
Woof!
Meow!
Dog destroyed
Cat destroyed

=== Memory Overhead ===
sizeof(Empty): 1
sizeof(WithVirtual): 8 (pointer added)

COMPLEXITY:
- Virtual call: 2 memory loads (vptr -> vtable -> function pointer) + call
- Non-virtual call: direct call (resolved at link/compile time)

INTERVIEW QUESTIONS:
1. How are virtual functions implemented?
   Compiler creates a vtable (array of function pointers) for each class with
   virtual functions. Each object has a vptr pointing to the class vtable.
   Virtual call: load vptr, index into vtable, call function pointer.

2. Why can't virtual functions be inlined (usually)?
   The concrete type is not known at compile time when called through base
   pointer/reference. If type is known (concrete object, devirtualization),
   compiler can inline.

3. What happens when you call a virtual function in constructor/destructor?
   The call dispatches to the function of the class currently being constructed
   (not the most derived). Because vptr is updated during construction chain.

COMMON MISTAKES:
- Calling virtual functions from constructor/destructor (unexpected behavior)
- Assuming virtual functions are always slow (usually negligible for typical use)
- Forgetting virtual destructor (UB when deleting derived via base pointer)
- Not understanding that virtual dispatch adds one indirection (affects cache)
- Expecting virtual function calls to be devirtualized (optimization, not guaranteed)
- Declaring too many virtual functions (vtable size, prevents inlining)
*/
