/*
 * 63_Function_Overloading.cpp
 * ---------------------------
 * Overloading: multiple functions with same name but different parameters.
 *
 * Theory:
 * - Overload by: different number of parameters, different types,
 *   different const/volatile qualification (for member functions),
 *   reference vs value (distinguishable by overload resolution).
 * - Overload resolution: 1) exact match, 2) promotion, 3) standard
 *   conversion, 4) user-defined conversion, 5) ellipsis.
 * - Return type is NOT part of the signature.
 * - Cannot overload by: only return type, only static/non-static.
 *
 * Sample I/O:
 * Enter int: 5
 * Enter double: 3.14
 * Enter string: hello
 * square(int) = 25
 * square(double) = 9.8596
 * print(string) = hello
 *
 * Complexity: O(1) per call, resolution at compile-time
 * Interview Questions:
 * Q: What is the difference between overloading and overriding?
 * A: Overloading = same name, different params (compile-time, same scope).
 *    Overriding = virtual function redefinition in derived class (runtime).
 * Q: Can you overload by const (for non-member functions)?
 * A: No, const applies to the object (this) only for member functions.
 *
 * Common Mistakes:
 * - Ambiguous overloads (e.g., f(double) and f(float) called with int).
 * - Expecting different return types to disambiguate.
 * - Forgetting that char* and const char* are distinct in overloads.
 */

#include <iostream>
#include <string>

int square(int x) {
    std::cout << "square(int) = ";
    return x * x;
}

double square(double x) {
    std::cout << "square(double) = ";
    return x * x;
}

void print(const std::string &s) {
    std::cout << "print(string) = " << s << '\n';
}

void print(const char *s) {
    std::cout << "print(char*) = " << s << '\n';
}

int main() {
    int a = 5;
    double b = 3.14;

    std::cout << square(a) << '\n';
    std::cout << square(b) << '\n';
    print("hello");
    print(std::string("world"));

    return 0;
}
