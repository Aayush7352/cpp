/*
==================================================================
137. try-catch-throw Basics
==================================================================
Theory:
- try block: encloses code that may throw exceptions
- catch block: catches exceptions of specific type
- Multiple catch blocks: order matters (most derived first)
- Catch-all: catch(...) must be last
- Rethrow: throw; (without expression) rethrows current exception
- noexcept specifier: function won't throw (terminate if it does)
- Stack unwinding: destructors of local objects are called
*/

#include <iostream>
#include <stdexcept>
#include <string>

void mightThrow(int choice) {
    switch (choice) {
        case 1: throw 42;                           // int
        case 2: throw std::runtime_error("Oops!");  // exception
        case 3: throw "raw string literal";         // const char*
        case 4: throw std::string("C++ string");    // std::string
        default: return;                            // no throw
    }
}

void rethrowDemo() {
    try {
        mightThrow(2);
    } catch (const std::runtime_error& e) {
        std::cout << "Caught in rethrowDemo: " << e.what() << "\n";
        throw; // rethrow
    }
}

class DestructorTester {
public:
    std::string m_name;
    DestructorTester(std::string n) : m_name(std::move(n)) {
        std::cout << "  Construct " << m_name << "\n";
    }
    ~DestructorTester() {
        std::cout << "  Destruct " << m_name << "\n";
    }
};

void stackUnwindDemo() {
    DestructorTester a("a");
    DestructorTester b("b");
    std::cout << "  About to throw...\n";
    throw std::runtime_error("unwind!");
}

// noexcept guarantees
void noThrowFunc() noexcept {
    std::cout << "  noexcept function: safe\n";
}

int main() {
    std::cout << "=== Try-Catch-Throw Basics\n\n";

    // --- Multiple catch blocks ---
    for (int i = 1; i <= 4; ++i) {
        std::cout << "Testing throw choice " << i << ":\n";
        try {
            mightThrow(i);
        } catch (int e) {
            std::cout << "  Caught int: " << e << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "  Caught runtime_error: " << e.what() << "\n";
        } catch (const char* e) {
            std::cout << "  Caught const char*: " << e << "\n";
        } catch (const std::string& e) {
            std::cout << "  Caught string: " << e << "\n";
        } catch (...) {
            std::cout << "  Catch-all (unknown type)\n";
        }
    }

    // --- Rethrow ---
    std::cout << "\nRethrow demo:\n";
    try {
        rethrowDemo();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught at outer level: " << e.what() << "\n";
    }

    // --- Stack unwinding ---
    std::cout << "\nStack unwinding demo:\n";
    try {
        stackUnwindDemo();
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught: " << e.what() << "\n";
    }

    // --- noexcept ---
    std::cout << "\nnoexcept function:\n";
    noThrowFunc();

    // --- Catch-all ---
    std::cout << "\nCatch-all test:\n";
    try {
        throw 3.14;
    } catch (...) {
        std::cout << "  Caught something (double)\n";
    }

    return 0;
}

/*
==================================================================
Sample I/O:
Testing throw choice 1:
  Caught int: 42
Testing throw choice 2:
  Caught runtime_error: Oops!
Testing throw choice 3:
  Caught const char*: raw string literal
Testing throw choice 4:
  Caught string: C++ string

Rethrow demo:
Caught in rethrowDemo: Oops!
Caught at outer level: Oops!

Stack unwinding demo:
  Construct a
  Construct b
  About to throw...
  Destruct b
  Destruct a
  Caught: unwind!

noexcept function:
  safe

Catch-all test:
  Caught something (double)
==================================================================
Complexity:
- Throwing/catching: O(1) amortized (but may involve allocation)
- Stack unwinding: O(n) where n = number of local objects with dtors
==================================================================
Interview Questions:
1. What is stack unwinding?
   -> Process of destroying local objects as exception propagates
      up the call stack. Guaranteed if dtor doesn't throw.

2. Can a destructor throw?
   -> Technically yes, but dangerous. std::terminate may be called
      if another exception is active (stack unwinding).

3. Difference between throw and throw; ?
   -> throw expr: throws a new exception. throw; (in catch):
      rethrows the current exception preserving its type.

4. What happens if a noexcept function throws?
   -> std::terminate is called immediately (no stack unwinding
      guaranteed).
==================================================================
Common Mistakes:
- Catching by value instead of reference (slicing)
- Catch order: base class before derived (derived never caught)
- Placing catch(...) before specific catches
- Throwing from destructor during stack unwinding
- Using throw; outside a catch handler (calls std::terminate)
==================================================================
*/
