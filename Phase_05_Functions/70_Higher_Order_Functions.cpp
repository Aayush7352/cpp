/*
 * 70_Higher_Order_Functions.cpp
 * -----------------------------
 * Functions returning functions, std::function, function composition,
 * map/filter patterns.
 *
 * Theory:
 * - Higher-order function: takes or returns another function.
 * - std::function: type-erased wrapper for callables (lambdas, functors).
 * - Function composition: combine two functions f and g into f(g(x)).
 * - Map: apply a function to each element of a container.
 * - Filter: select elements satisfying a predicate.
 *
 * Sample I/O:
 * add5(10) = 15
 * mul2(10) = 20
 * compose(add5, mul2)(10) = 25  (add5(mul2(10)))
 * map: 2 4 6 8 10
 * filter: 10 30 50
 *
 * Complexity: O(n) for map/filter, O(1) for composition
 * Interview Questions:
 * Q: What is the performance cost of std::function?
 * A: It has small overhead (type erasure, heap allocation for large
 *    callables). Prefer auto or templates in performance-critical code.
 * Q: How does function composition relate to functional programming?
 * A: Composition is a core concept; it enables point-free style and
 *    building complex logic from simple functions.
 *
 * Common Mistakes:
 * - std::function overhead for simple callbacks (prefer template).
 * - Returning a lambda that captures a local reference (dangling).
 * - Forgetting std::invoke or proper forwarding with std::function.
 */

#include <iostream>
#include <vector>
#include <functional>

std::function<int(int)> compose(
    std::function<int(int)> f,
    std::function<int(int)> g) {
    return [f, g](int x) { return f(g(x)); };
}

void map(const std::vector<int> &v, std::function<int(int)> f) {
    for (auto x : v) std::cout << f(x) << ' ';
    std::cout << '\n';
}

std::vector<int> filter(const std::vector<int> &v, std::function<bool(int)> pred) {
    std::vector<int> result;
    for (auto x : v)
        if (pred(x)) result.push_back(x);
    return result;
}

int main() {
    auto add5 = [](int x) { return x + 5; };
    auto mul2 = [](int x) { return x * 2; };

    std::cout << "add5(10) = " << add5(10) << '\n';
    std::cout << "mul2(10) = " << mul2(10) << '\n';

    auto composed = compose(add5, mul2);
    std::cout << "compose(add5, mul2)(10) = " << composed(10) << '\n';

    std::vector<int> v{1, 2, 3, 4, 5};
    std::cout << "map (x*2): ";
    map(v, mul2);

    std::vector<int> v2{5, 10, 25, 30, 50};
    auto evens = filter(v2, [](int x) { return x % 2 == 0; });
    std::cout << "filter evens: ";
    for (auto x : evens) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
