/*
THEORY: Function Pointers, std::function, Member Function Pointers, Callbacks

1. Function pointer: pointer to a free function.
   Syntax: return_type (*ptr)(param_types...)
2. std::function (C++11): type-erased wrapper for callable objects.
   More flexible but has overhead.
3. Member function pointer: pointer to a class member function.
   Syntax: return_type (Class::*ptr)(params...) const
4. Callback: function passed as argument to be invoked later.
*/

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

// --- Free functions ---
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// --- Callback user ---
void process(const std::vector<int>& data, int (*op)(int, int), int init) {
    int result{init};
    for (int x : data) result = op(result, x);
    std::cout << "Result: " << result << '\n';
}

// --- Class with member functions ---
struct Calculator {
    int factor;

    explicit Calculator(int f) : factor{f} {}

    int scale(int x) const { return x * factor; }
    static int square(int x) { return x * x; }
};

int main() {
    // 1. Function pointers
    std::cout << "=== function pointers ===\n";
    int (*fptr)(int, int){add};           // pointer to add
    std::cout << "add via fptr: " << fptr(10, 20) << '\n';

    fptr = multiply;
    std::cout << "multiply via fptr: " << fptr(10, 20) << '\n';

    // 2. Function pointer as callback
    std::cout << "\n=== callback ===\n";
    std::vector<int> nums{1, 2, 3, 4, 5};
    process(nums, add, 0);
    process(nums, multiply, 1);

    // 3. std::function
    std::cout << "\n=== std::function ===\n";
    std::function<int(int, int)> func{add};
    std::cout << "std::function add: " << func(5, 3) << '\n';

    func = [](int a, int b) { return a * a + b * b; };
    std::cout << "std::function lambda: " << func(3, 4) << '\n';

    // 4. std::function in container
    std::vector<std::function<int(int, int)>> ops{add, multiply,
        [](int a, int b) { return a - b; }};
    std::cout << "\nOperations via vector:\n";
    for (auto& op : ops) {
        std::cout << op(10, 5) << ' ';
    }
    std::cout << '\n';

    // 5. Member function pointer
    std::cout << "\n=== member function pointer ===\n";
    Calculator calc{10};

    int (Calculator::*mptr)(int) const{&Calculator::scale};
    std::cout << "calc.*mptr(5): " << (calc.*mptr)(5) << '\n';

    Calculator* cptr{&calc};
    std::cout << "cptr->*mptr(5): " << (cptr->*mptr)(5) << '\n';

    // Static member function pointer (same as free function)
    int (*static_ptr)(int){&Calculator::square};
    std::cout << "static member: " << static_ptr(6) << '\n';

    // 6. std::function with member function
    std::function<int(const Calculator&, int)> memfunc{&Calculator::scale};
    std::cout << "std::function member: " << memfunc(calc, 7) << '\n';

    // 7. std::bind (C++11) vs lambda (prefer lambda)
    std::cout << "\n=== bind vs lambda ===\n";
    auto bound = std::bind(add, 100, std::placeholders::_1);
    std::cout << "bind result: " << bound(50) << '\n';

    auto lambdaBind = [](int x) { return add(100, x); };
    std::cout << "lambda result: " << lambdaBind(50) << '\n';

    return 0;
}

/*
SAMPLE I/O:
=== function pointers ===
add via fptr: 30
multiply via fptr: 200

=== callback ===
Result: 15
Result: 120

=== std::function ===
std::function add: 8
std::function lambda: 25

Operations via vector:
15 50 5

=== member function pointer ===
calc.*mptr(5): 50
cptr->*mptr(5): 50
static member: 36

std::function member: 70

=== bind vs lambda ===
bind result: 150
lambda result: 150

COMPLEXITY:
- Function pointer call: O(1), no overhead
- std::function call: O(1) but may have virtual dispatch overhead
- Member function pointer: O(1)

INTERVIEW QUESTIONS:
1. What is the sizeof a function pointer?
   Same as data pointer on most systems (8 bytes on 64-bit).
   Member function pointers may be larger (typically 16 bytes) to handle virtual functions.

2. std::function vs raw function pointer?
   std::function can store any callable (lambda, bind, functor). Has small overhead
   (heap alloc for lambdas with captures). Function pointer is minimal, only free functions.

3. When would you use std::function?
   When you need type erasure (callback registries, observer pattern, plugins).
   When the callable type is not known at compile time.

COMMON MISTAKES:
- Forgetting the outer parentheses: int* f() means function returning int*, not
  function pointer. Use int (*f)().
- Taking address of overloaded function without explicit cast
- Using std::function for high-frequency calls (performance overhead)
- Forgetting this object for member function pointer call
- Slicing with std::function when capturing large lambdas
*/
