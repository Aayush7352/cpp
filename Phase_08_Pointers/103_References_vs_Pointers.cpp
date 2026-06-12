/*
THEORY: References vs Pointers

| Feature              | Pointer                    | Reference                |
|----------------------|----------------------------|--------------------------|
| Syntax               | int* p = &x;              | int& r = x;              |
| Nullable             | Yes (nullptr)              | No (must be bound)       |
| Reassignable         | Yes                        | No (binds once)          |
| Dereference          | Must use * or ->           | Automatic                |
| Array/pointer arith  | Yes                        | No                       |
| Rebind to new obj    | Yes                        | No                       |
| Memory address       | Holds address              | Alias (same address)     |
| Default param        | nullptr works              | Must be lvalue           |

Use reference when: you want an alias, no null, no rebind.
Use pointer when: null allowed, rebind needed, pointer arithmetic, dynamic memory.
*/

#include <iostream>
#include <string>
#include <cassert>

void by_pointer(int* p) {
    if (p) {
        *p *= 2;
    }
}

void by_reference(int& r) {
    r *= 2;     // no null check needed
}

int main() {
    int x{10};
    int y{20};

    // --- References ---
    int& ref{x};               // ref is alias for x
    std::cout << "ref = " << ref << " (alias for x)\n";
    ref = 30;                  // changes x
    std::cout << "x after ref = 30: " << x << '\n';

    // ref = y;                // DOES NOT rebind — assigns y's value to x
    // &ref is same as &x

    const int& cr{42};         // const ref can bind to rvalue (lifetime extended)
    std::cout << "cr = " << cr << '\n';

    // --- Pointers ---
    int* p{&x};
    std::cout << "\n*p = " << *p << '\n';
    p = &y;                    // rebind
    std::cout << "*p after rebind = " << *p << '\n';

    int* nullp{nullptr};       // nullable
    // *nullp;                 // UB — crash

    // --- const correctness ---
    int val{5};
    const int* cptr{&val};     // pointer to const int
    int* const ptrc{&val};     // const pointer to int
    const int* const cptrc{&val}; // const pointer to const int

    const int& cref{val};      // reference to const int

    // --- function params ---
    by_pointer(nullptr);       // OK, handles null
    by_reference(val);         // val must exist
    // by_reference(*(int*)nullptr); // UB

    // --- reassignment check ---
    int a{1}, b{2};
    int& alias{a};
    int* ptr{&a};

    alias = b;                 // a becomes 2 (no rebind)
    std::cout << "\nafter alias = b: a = " << a << ", b = " << b << '\n';

    ptr = &b;                  // rebind
    std::cout << "after ptr = &b: *ptr = " << *ptr << '\n';

    return 0;
}

/*
SAMPLE I/O:
ref = 10 (alias for x)
x after ref = 30: 30
cr = 42

*p = 30
*p after rebind = 20
null check prevents UB

after alias = b: a = 2, b = 2
after ptr = &b: *ptr = 2

COMPLEXITY: O(1) all operations

INTERVIEW QUESTIONS:
1. Can a reference be null?
   No. Undefined behavior to create a null reference. Some low-level code may
   deliberately create one via *nullptr, but this is hazardous.

2. When would you choose pointer over reference?
   When null is a valid input, you need to rebind, or you need pointer arithmetic.
   Also for dynamic memory (new returns a pointer).

3. What is a "dangling reference"?
   A reference to a temporary/out-of-scope object. Example: returning reference
   to local variable. Compilers usually warn.

COMMON MISTAKES:
- Thinking reference can be rebound: assigning to ref modifies the original
- Returning reference to local variable (dangling reference)
- Passing nullptr to a reference parameter (UB)
- Using int& r = &x; (wrong syntax; &x is address, not int)
- Forgetting that const int& can bind to rvalue (lifetime extension)
*/
