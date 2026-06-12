/*
THEORY: Pointer Basics

Pointer: A variable that stores the memory address of another variable.
- Address-of (&): Gets the memory address of a variable.
- Dereference (*): Accesses the value stored at the address held by pointer.
- Null pointer: Points to nothing (address 0); dereferencing crashes.
- void*: Generic pointer; can hold any address but cannot be dereferenced without casting.
- nullptr (C++11): Type-safe null pointer literal (preferred over NULL or 0).

Declaration: type* ptr;  or  type *ptr;  (style choice, * binds to variable name)
*/

#include <iostream>
#include <type_traits>

int main() {
    // 1. Address-of and Dereference
    int x{42};
    int* ptr{&x};                          // ptr holds address of x

    std::cout << "x  = " << x << '\n';
    std::cout << "&x = " << &x << '\n';
    std::cout << "ptr = " << ptr << '\n';
    std::cout << "*ptr = " << *ptr << '\n'; // dereference: read value at ptr

    *ptr = 100;                            // modify x through ptr
    std::cout << "x after *ptr = 100: " << x << '\n';

    // 2. Null pointer
    int* np{nullptr};
    if (np) {
        std::cout << "Non-null\n";
    } else {
        std::cout << "np is null, cannot dereference\n";
    }

    // 3. void* pointer
    void* vp{&x};
    // *vp; // ERROR: cannot dereference void*
    std::cout << "void* holds address: " << vp << '\n';
    std::cout << "Cast void* to int* and deref: " << *static_cast<int*>(vp) << '\n';

    // 4. nullptr_t type
    std::nullptr_t n{nullptr};
    int* p2{n}; // converts to int*
    // int i = n; // ERROR: no conversion to non-pointer
    std::cout << "std::nullptr_t variable is null: " << (p2 == nullptr ? "true" : "false") << '\n';

    // 5. Pointer to pointer
    int** pp{&ptr};
    std::cout << "**pp = " << **pp << '\n';

    // 6. const and pointers
    const int ci{10};
    const int* cptr{&ci}; // pointer to const int (can't modify through cptr)
    // *cptr = 20; // ERROR

    int mutable_val{5};
    int* const ptr_c{mutable_val}; // const pointer (can't change address)
    *ptr_c = 10; // OK: can modify value
    // ptr_c = &x; // ERROR

    return 0;
}

/*
SAMPLE I/O:
x  = 42
&x = 0x16f82b528
ptr = 0x16f82b528
*ptr = 42
x after *ptr = 100: 100
np is null, cannot dereference
void* holds address: 0x16f82b528
Cast void* to int* and deref: 100
std::nullptr_t variable is null: true
**pp = 100

COMPLEXITY:
- Time: O(1) all operations
- Space: O(1)

INTERVIEW QUESTIONS:
1. Difference between NULL, 0, and nullptr?
   nullptr is type-safe (std::nullptr_t), avoids ambiguity in overloaded functions.
   NULL is preprocessor macro (usually 0), 0 is int, can cause overload resolution issues.

2. What happens if you dereference a null pointer?
   Undefined behavior — typically a crash (segmentation fault).

3. What is a dangling pointer?
   A pointer that points to memory that has been freed. Dereferencing it is UB.

COMMON MISTAKES:
- Forgetting to initialize a pointer (uninitialized = wild pointer)
- Dereferencing nullptr or dangling pointer
- Confusing (int*) vs int (*) in declarations
- Using & on a register variable (not allowed)
- Mixing * and & in complex declarations like int*& (reference to pointer)
*/
