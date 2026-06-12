/*
 * ====================================================================
 * Program 11: References
 *
 * THEORY:
 * A reference is an alias — another name for an existing object.
 *   - Must be initialized when declared.
 *   - Cannot be reassigned to refer to a different object.
 *   - Cannot be null.
 *
 * TYPES:
 *   Lvalue reference:   Type&   — binds to lvalues (named objects)
 *   Const reference:    const Type& — can bind to temporaries too
 *   Rvalue reference:   Type&&  — binds to temporaries (C++11)
 *
 * REFERENCE vs POINTER:
 *   Reference                     Pointer
 *   Alias for object              Stores address
 *   Cannot be null                Can be null
 *   Cannot be reseated            Can be reseated
 *   Auto-dereferenced             Must explicit dereference (*)
 *   Safer, no pointer arithmetic  Supports pointer arithmetic
 *
 * REFERENCE TO ARRAY:
 *   int (&arr_ref)[5] = arr;
 *   Useful in templates to preserve array size.
 *
 * RANGE-FOR references:
 *   for (auto& elem : container) — avoid copying, allow mutation
 *   for (const auto& elem : container) — avoid copying, read-only
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output:
 *     x = 10, ref = 10
 *     After ref = 20: x = 20
 *     Array sum via ref: 15
 *     Range-for with auto& modified each element
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: Reference vs pointer — when to use which?
 *   A: Use reference for function parameters (avoid copies, no null),
 *      use pointer for nullable/optional/reseatable semantics.
 *   Q: Can a reference be null?
 *   A: No. A reference must always refer to a valid object.
 *   Q: What is a dangling reference?
 *   A: A reference to an object that has been destroyed (UB to use).
 *
 * COMMON MISTAKES:
 *   - Declaring a reference without initialization.
 *   - Trying to reassign a reference (it modifies the original).
 *   - Returning a reference to a local variable.
 *   - Confusing Type& with Type& in declarations: int& a, b;
 *     (b is int, not int& — the & applies only to a).
 * ====================================================================
 */

#include <iostream>
#include <vector>

void swap_by_ref(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

int main()
{
    // ---- Basic reference ----
    std::cout << "=== Basic references ===\n";
    int x = 10;
    int& ref = x;          // ref is an alias for x
    std::cout << "x = " << x << ", ref = " << ref << '\n';

    ref = 20;              // modifies x
    std::cout << "After ref = 20: x = " << x << '\n';

    // ---- Const reference ----
    std::cout << "\n=== Const reference ===\n";
    const int& cref = x;   // read-only view
    // cref = 30;          // Error: assignment of read-only reference
    std::cout << "cref = " << cref << '\n';

    // Can bind to temporaries
    const double& temp_ref = 3.14 + 2.86;
    std::cout << "Const ref to temp: " << temp_ref << '\n';

    // ---- Reference vs pointer ----
    std::cout << "\n=== Reference vs pointer ===\n";
    int  p_val = 42;
    int& r_val = p_val;    // reference — auto-dereferenced
    int* p_ptr = &p_val;   // pointer — must dereference

    std::cout << "Reference (auto-deref): " << r_val << '\n';
    std::cout << "Pointer  (*p_ptr):      " << *p_ptr << '\n';

    // ---- Swap via reference ----
    std::cout << "\n=== Swap via reference ===\n";
    int a = 1, b = 2;
    std::cout << "Before: a = " << a << ", b = " << b << '\n';
    swap_by_ref(a, b);
    std::cout << "After:  a = " << a << ", b = " << b << '\n';

    // ---- Reference to array ----
    std::cout << "\n=== Reference to array ===\n";
    int arr[3] = {10, 20, 30};
    int (&arr_ref)[3] = arr;   // reference to array of 3 ints

    int sum = 0;
    for (int i = 0; i < 3; ++i)
        sum += arr_ref[i];
    std::cout << "Sum via array reference: " << sum << '\n';

    // ---- Range-for with references ----
    std::cout << "\n=== Range-for with references ===\n";
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // auto& — modify in place
    for (auto& v : vec)
        v *= 2;

    // const auto& — read-only, no copy
    std::cout << "After doubling: ";
    for (const auto& v : vec)
        std::cout << v << ' ';
    std::cout << '\n';

    // ---- Common mistake: reassignment ----
    std::cout << "\n=== Common mistake (reassignment) ===\n";
    int    i1 = 10, i2 = 20;
    int&   ri = i1;
    std::cout << "i1 = " << i1 << ", i2 = " << i2 << ", ri = " << ri << '\n';
    ri = i2;   // This does NOT make ri refer to i2. It assigns i2's value to i1.
    std::cout << "After ri = i2: i1 = " << i1 << ", i2 = " << i2
              << ", ri = " << ri << '\n';
    std::cout << "(&ri == &i1) = " << (&ri == &i1) << " (still i1!)\n";

    return 0;
}
