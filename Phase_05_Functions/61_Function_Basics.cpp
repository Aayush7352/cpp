/*
 * 61_Function_Basics.cpp
 * ----------------------
 * Function declaration, definition, call, pass by value/reference/pointer.
 *
 * Theory:
 * - Declaration (prototype): tells compiler about function name, return type,
 *   and parameters. Definition: contains the body.
 * - Pass by value: copy of argument; original unchanged.
 * - Pass by reference: alias to original; modifications affect original.
 * - Pass by pointer: address passed; use * to dereference.
 * - Return by value: returns a copy; return by reference: returns an alias.
 *
 * Sample I/O:
 * Enter two numbers: 5 3
 * By value sum: 8
 * By ref swap: a=3, b=5
 * By pointer sum: 8
 *
 * Complexity: O(1) time, O(1) space
 * Interview Questions:
 * Q: What is the difference between pass by reference and pass by pointer?
 * A: References cannot be null, cannot be reassigned, use cleaner syntax.
 *    Pointers can be null, can be reassigned, need explicit dereference.
 * Q: Can a function return a local variable by reference?
 * A: No, that's a dangling reference (undefined behavior).
 *
 * Common Mistakes:
 * - Returning reference to local variable.
 * - Forgetting to dereference pointer parameter.
 * - Passing large objects by value (expensive copy; use const reference).
 */

#include <iostream>

int sumByValue(int a, int b) {
    return a + b;
}

void swapByRef(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int sumByPointer(const int *a, const int *b) {
    return *a + *b;
}

int main() {
    int x, y;
    std::cout << "Enter two numbers: ";
    std::cin >> x >> y;

    std::cout << "By value sum: " << sumByValue(x, y) << '\n';

    swapByRef(x, y);
    std::cout << "By ref swap: x=" << x << ", y=" << y << '\n';

    std::cout << "By pointer sum: " << sumByPointer(&x, &y) << '\n';

    return 0;
}
