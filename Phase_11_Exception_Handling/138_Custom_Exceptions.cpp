/*
==================================================================
138. Custom Exception Hierarchy
==================================================================
Theory:
- Derive custom exceptions from std::runtime_error (or std::logic_error)
- Hierarchy allows catch-by-base (polymorphic catch)
- what() returns explanatory string
- Exception classes should have strong guarantee (copyable)
- Keep exception types small (no members that might throw)
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring> // strerror
#include <cerrno>

// ---- Custom Exception Hierarchy ----

// Base: MathError
class MathError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Derived: DivisionByZero
class DivisionByZero : public MathError {
public:
    explicit DivisionByZero(const std::string& detail = "")
        : MathError("Division by zero" +
            (detail.empty() ? "" : ": " + detail)) {}
};

// Derived: DomainError
class DomainError : public MathError {
public:
    explicit DomainError(const std::string& func, double val)
        : MathError("Domain error in " + func +
            ": invalid argument " + std::to_string(val)) {}
};

// Derived: OverflowError
class OverflowError : public MathError {
public:
    explicit OverflowError(const std::string& msg = "Overflow")
        : MathError(msg) {}
};

// System-level exception
class FileError : public std::runtime_error {
public:
    explicit FileError(const std::string& filename)
        : std::runtime_error("File error: " + filename +
            " (" + std::strerror(errno) + ")") {}
};

// --- Demonstration ---

double safeDivide(double a, double b) {
    if (b == 0)
        throw DivisionByZero("safeDivide(" +
            std::to_string(a) + ", " + std::to_string(b) + ")");
    return a / b;
}

double safeSqrt(double x) {
    if (x < 0)
        throw DomainError("safeSqrt", x);
    return std::sqrt(x);
}

void processFile(const std::string& name) {
    // Simulate file error
    errno = ENOENT; // "No such file or directory"
    throw FileError(name);
}

int main() {
    std::cout << "=== Custom Exception Hierarchy\n\n";

    // --- Catch by base class ---
    try {
        safeDivide(10, 0);
    } catch (const MathError& e) {
        std::cout << "Caught MathError: " << e.what() << "\n";
    }

    try {
        safeSqrt(-4.0);
    } catch (const MathError& e) {
        std::cout << "Caught MathError: " << e.what() << "\n";
    }

    // --- Specific catch (more derived first) ---
    try {
        safeDivide(5, 0);
    } catch (const DivisionByZero& e) {
        std::cout << "Caught DivisionByZero specifically: " << e.what() << "\n";
    } catch (const MathError& e) {
        std::cout << "Caught MathError: " << e.what() << "\n";
    }

    // --- Polymorphic container ---
    std::cout << "\nStoring exceptions in vector:\n";
    std::vector<MathError> excVec; // slicing! Better store pointers
    // We'll use pointers instead:
    std::vector<std::unique_ptr<MathError>> errors;
    errors.push_back(std::make_unique<DivisionByZero>("test1"));
    errors.push_back(std::make_unique<DomainError>("func", -1));
    errors.push_back(std::make_unique<OverflowError>("overflow in pow"));

    for (const auto& e : errors)
        std::cout << "  " << e->what() << "\n";

    // --- File error ---
    std::cout << "\nFile error:\n";
    try {
        processFile("nonexistent.txt");
    } catch (const std::runtime_error& e) {
        std::cout << "  " << e.what() << "\n";
    }

    return 0;
}

/*
==================================================================
Sample I/O:
Caught MathError: Division by zero: safeDivide(10.000000, 0.000000)
Caught MathError: Domain error in safeSqrt: invalid argument -4.000000
Caught DivisionByZero specifically: Division by zero: safeDivide(5.000000, 0.000000)

Storing exceptions in vector:
  Division by zero: test1
  Domain error in func: invalid argument -1.000000
  Overflow: overflow in pow

File error:
  File error: nonexistent.txt (No such file or directory)
==================================================================
Complexity:
- Construction: O(1) or O(len(msg)) for string copy
- Throwing/catching: O(1) amortized
==================================================================
Interview Questions:
1. Why derive from std::runtime_error vs std::exception?
   -> runtime_error carries a string message. Direct exception
      has no what() implementation.

2. Can an exception object be copied?
   -> Yes, copy is required for propagation. Keep members
      copyable and cheap.

3. Slicing problem with exceptions?
   -> catch (Base e) slices derived part. Always catch by
      reference: catch (const Base& e).
==================================================================
Common Mistakes:
- Deriving from std::exception directly and forgetting what()
- Catching by value (slicing)
- Making exception members that can throw (double fault)
- Missing virtual destructor (not needed if deriving from std::runtime_error)
- Using errno without setting/checking it properly
==================================================================
*/
