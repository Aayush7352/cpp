/*
 * 69_Lambda_Functions.cpp
 * -----------------------
 * Full lambda guide: capture by value/ref, mutable, IIFE,
 * generic lambdas, capture with init, template lambdas (C++20).
 *
 * Theory:
 * - Lambda syntax: [captures](params) specifiers -> return_type { body }
 * - Capture by value [=], by reference [&], mixed [x, &y].
 * - mutable: allows modification of value-captured variables inside body.
 * - IIFE (Immediately Invoked Function Expression): define and call at once.
 * - Generic lambda (C++14): [](auto x) { ... }
 * - Template lambda (C++20): []<typename T>(T x) { ... }
 * - Capture with init (C++14): [x = expr](...) { ... }
 *
 * Sample I/O:
 * sum = 15
 * count > 2: 3
 * IIFE result: 10
 * generic: 6, 3.14
 * template: 7, 7.77
 *
 * Complexity: O(1) lambda overhead (no more than function object)
 * Interview Questions:
 * Q: How is a lambda implemented internally?
 * A: The compiler generates an unnamed functor (struct with operator()).
 * Q: What is the default capture mode best practice?
 * A: Prefer explicit captures [&x, y] over [=] or [&] to avoid dangling.
 * Q: Can a lambda be default-constructed?
 * A: No, unless it's a stateless lambda (no captures) which is
 *    convertible to function pointer.
 *
 * Common Mistakes:
 * - Capturing a reference to a temporary (dangling reference).
 * - Modifying a value-captured variable without mutable.
 * - Using default capture [=] in a member function captures this
 *   (by value, but this is a pointer -- subtle).
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

int main() {
    // Basic lambda
    auto add = [](int a, int b) { return a + b; };
    std::cout << "sum = " << add(5, 10) << '\n';

    // Capture by value and reference
    int threshold = 2;
    std::vector<int> v{1, 2, 3, 4, 5};
    int count = 0;
    std::for_each(v.begin(), v.end(), [threshold, &count](int x) {
        if (x > threshold) ++count;
    });
    std::cout << "count > 2: " << count << '\n';

    // Mutable lambda (value capture can be modified inside)
    int x = 5;
    auto inc = [x]() mutable { return ++x; };
    std::cout << "inc: " << inc() << ", original x: " << x << '\n';

    // IIFE
    int iife = []() { return 7 + 3; }();
    std::cout << "IIFE result: " << iife << '\n';

    // Generic lambda (C++14)
    auto generic = [](auto a, auto b) { return a + b; };
    std::cout << "generic: " << generic(2, 4) << ", " << generic(1.1, 2.04) << '\n';

    // Template lambda (C++20) -- can refer to the type T
    auto templated = []<typename T>(T a, T b) { return a + b; };
    std::cout << "template: " << templated(3, 4) << ", " << templated(3.33, 4.44) << '\n';

    // Capture with init (C++14)
    auto initCapture = [y = 10](int x) { return x * y; };
    std::cout << "init capture: " << initCapture(5) << '\n';

    return 0;
}
