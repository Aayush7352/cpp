/*
==================================================================
139. Nested Exceptions
==================================================================
Theory:
- std::nested_exception: captures and stores current exception
- std::throw_with_nested: throws a new exception with nested info
- std::exception_ptr: handle to exception for transport across threads
- std::rethrow_exception: rethrows from exception_ptr
- std::current_exception: captures current exception into exception_ptr
- Useful for adding context without losing original error info
*/

#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>
#include <sstream>

// ---- Nested Exceptions ----

void innerFunc() {
    try {
        throw std::runtime_error("Original error: disk full");
    } catch (...) {
        // Wrap current exception with additional context
        std::throw_with_nested(
            std::runtime_error("innerFunc failed"));
    }
}

void middleFunc() {
    try {
        innerFunc();
    } catch (...) {
        std::throw_with_nested(
            std::runtime_error("middleFunc failed"));
    }
}

void outerFunc() {
    try {
        middleFunc();
    } catch (...) {
        std::throw_with_nested(
            std::runtime_error("outerFunc failed"));
    }
}

// Recursive printer for nested exceptions
void printNested(const std::exception& e, int level = 0) {
    std::cerr << std::string(level * 2, ' ') << "Level " << level
              << ": " << e.what() << "\n";
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& nested) {
        printNested(nested, level + 1);
    } catch (...) {
        std::cerr << std::string((level + 1) * 2, ' ')
                  << "Non-standard exception\n";
    }
}

// ---- exception_ptr demo (thread-safe exception transport) ----
#include <thread>
#include <vector>

std::exception_ptr globalExceptionPtr = nullptr;

void threadWorker(int id) {
    try {
        if (id == 2)
            throw std::logic_error("Thread " + std::to_string(id) + " error");
        std::cout << "  Thread " << id << " OK\n";
    } catch (...) {
        globalExceptionPtr = std::current_exception();
    }
}

int main() {
    std::cout << "=== Nested Exceptions\n\n";

    // --- Nested exception chain ---
    std::cout << "Nested exception chain:\n";
    try {
        outerFunc();
    } catch (...) {
        // Print with nested details
        try {
            throw; // rethrow to examine
        } catch (const std::exception& e) {
            printNested(e);
        }
    }

    // --- throw_with_nested + custom exception ---
    std::cout << "\nCustom exception with nested:\n";
    try {
        try {
            throw std::invalid_argument("invalid input");
        } catch (...) {
            std::throw_with_nested(
                std::runtime_error("validation failed"));
        }
    } catch (const std::exception& e) {
        printNested(e);
    }

    // --- exception_ptr for cross-thread ---
    std::cout << "\nexception_ptr demo (cross-thread):\n";
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i)
        threads.emplace_back(threadWorker, i);
    for (auto& t : threads)
        t.join();

    if (globalExceptionPtr) {
        try {
            std::rethrow_exception(globalExceptionPtr);
        } catch (const std::exception& e) {
            std::cerr << "  Caught from thread: " << e.what() << "\n";
        }
    } else {
        std::cout << "  All threads completed OK\n";
    }

    return 0;
}

/*
==================================================================
Sample I/O:
Nested exception chain:
Level 0: outerFunc failed
Level 1: middleFunc failed
Level 2: innerFunc failed
Level 3: Original error: disk full

Custom exception with nested:
Level 0: validation failed
Level 1: invalid input

exception_ptr demo (cross-thread):
  Thread 0 OK
  Thread 1 OK
  Thread 3 OK
  Caught from thread: Thread 2 error
==================================================================
Complexity:
- throw_with_nested: O(1) for capture, O(n) for string concat
- rethrow_if_nested: O(d) recursion depth
==================================================================
Interview Questions:
1. How to print full nested exception chain?
   -> Write recursive function calling std::rethrow_if_nested
      inside a catch block.

2. Difference between nested_exception and exception_ptr?
   -> nested_exception: stored inside another exception for
      propagation context. exception_ptr: handle to any exception
      for storage/retransmission (thread-safe).

3. Can we nest a nested exception?
   -> Yes, nesting can be arbitrarily deep.
==================================================================
Common Mistakes:
- Forgetting to rethrow inside catch for std::throw_with_nested
- Not using std::rethrow_if_nested (just printing what() misses nested)
- Catching by value when rethrowing nested (slicing)
- Using exception_ptr without checking for nullptr
- Trying to nested_exception with a non-standard exception (catch(...))
==================================================================
*/
