/*
 * 62_Default_Arguments.cpp
 * ------------------------
 * Default parameters: provide default values for function parameters.
 *
 * Theory:
 * - Default arguments are specified in the declaration (not definition).
 * - They must be at the end of the parameter list (right-to-left).
 * - Virtual functions and default args: default args are based on
 *   static type, not dynamic type (caveat: avoid defaults in virtuals).
 * - Overloading with defaults can create ambiguity.
 *
 * Sample I/O:
 * 10 (no default rad)
 * 10 3.14159 (default rad)
 * Area with r=5 (default): 78.5397
 * Area with r=5, pi=3: 75
 *
 * Complexity: O(1) time, O(1) space
 * Interview Questions:
 * Q: Can default arguments be in both declaration and definition?
 * A: No, only in one (usually the declaration). Duplicate = compile error.
 * Q: Are default arguments evaluated at call time or compile time?
 * A: At call time (each call evaluates the default expression anew).
 *
 * Common Mistakes:
 * - Specifying default in both declaration and definition.
 * - Placing non-defaulted parameters after defaulted ones.
 * - Assuming virtual dispatch picks derived defaults (it doesn't).
 */

#include <iostream>

double area(double r, double pi = 3.1415926535) {
    return pi * r * r;
}

int main() {
    double r;
    std::cout << "Enter radius: ";
    std::cin >> r;

    std::cout << "Area (default pi): " << area(r) << '\n';
    std::cout << "Area (pi=3.0): " << area(r, 3.0) << '\n';

    return 0;
}
