/*
THEORY: unique_ptr - Exclusive Ownership Smart Pointer

- std::unique_ptr<T>: owns a dynamically allocated T exclusively.
- Non-copyable (copy constructor/assignment deleted).
- Move-only: ownership transfers via std::move().
- std::make_unique<T>(args...): preferred way to create (C++14).
- Custom deleter: second template parameter (stateless or stateful).
- .get(): returns raw pointer. .release(): releases ownership. .reset(): delete and replace.
- Same size as raw pointer (zero overhead abstraction).
*/

#include <iostream>
#include <memory>
#include <string>

// --- Custom deleter ---
struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) {
            std::cout << "Closing file via custom deleter\n";
            fclose(f);
        }
    }
};

// --- Stateful custom deleter ---
struct LoggerDeleter {
    std::string msg;
    void operator()(int* p) const {
        std::cout << msg << " " << *p << '\n';
        delete p;
    }
};

int main() {
    // 1. Basic usage with make_unique
    std::cout << "=== make_unique ===\n";
    auto uptr{std::make_unique<int>(42)};
    std::cout << "*uptr = " << *uptr << '\n';

    // 2. Move-only semantics
    std::cout << "\n=== move semantics ===\n";
    auto uptr2{std::move(uptr)};
    // *uptr; // UB: uptr is null after move
    if (!uptr) std::cout << "uptr is null after move\n";
    std::cout << "*uptr2 = " << *uptr2 << '\n';

    // 3. Release / Reset
    std::cout << "\n=== release and reset ===\n";
    auto uptr3{std::make_unique<int>(100)};
    int* raw{uptr3.release()};  // uptr3 becomes null, doesn't delete
    std::cout << "Released raw = " << *raw << '\n';
    delete raw;                 // must manually delete

    uptr3.reset(new int(200));  // delete old, take new ownership
    std::cout << "*uptr3 after reset = " << *uptr3 << '\n';
    uptr3.reset();              // delete owned object, uptr3 becomes null
    std::cout << "After empty reset: " << (uptr3 ? "non-null" : "null") << '\n';

    // 4. Arrays
    std::cout << "\n=== unique_ptr with arrays ===\n";
    auto arr{std::make_unique<int[]>(5)};
    for (int i{0}; i < 5; ++i) arr[i] = i * 10;
    for (int i{0}; i < 5; ++i) std::cout << "arr[" << i << "] = " << arr[i] << '\n';

    // 5. Custom deleter (stateless function object)
    std::cout << "\n=== custom deleter with FILE* ===\n";
    const char* fname{"/tmp/test.txt"};
    // Create file so fopen succeeds
    FILE* tmp{fopen(fname, "w")};
    fputs("hello", tmp);
    fclose(tmp);

    std::unique_ptr<FILE, FileDeleter> file{fopen(fname, "r")};
    char buf[64]{};
    if (file && fgets(buf, sizeof(buf), file.get())) {
        std::cout << "Read: " << buf;
    }
    // file automatically closed when goes out of scope

    // 6. Custom deleter with state
    std::cout << "\n=== stateful custom deleter ===\n";
    std::unique_ptr<int, LoggerDeleter> logged(new int(999), LoggerDeleter{"Deleting value:"});
    // destructor will print "Deleting value: 999"

    // 7. Factory function returning unique_ptr
    auto makeWidget = [](int v) {
        return std::make_unique<int>(v);
    };
    auto w{makeWidget(77)};
    std::cout << "\nFactory: *w = " << *w << '\n';

    return 0;
}

/*
SAMPLE I/O:
=== make_unique ===
*uptr = 42

=== move semantics ===
uptr is null after move
*uptr2 = 42

=== release and reset ===
Released raw = 100
*uptr3 after reset = 200
After empty reset: null

=== custom deleter with FILE* ===
Read: hello
Closing file via custom deleter

=== stateful custom deleter ===
Deleting value: 999

Factory: *w = 77

COMPLEXITY:
- All operations: O(1)
- Custom deleter: no overhead if stateless (empty base optimization)

INTERVIEW QUESTIONS:
1. What is make_unique and why prefer it?
   Exception-safe (no gap between allocation and ownership), simpler syntax,
   avoids explicit new. Added in C++14.

2. Can unique_ptr be used with incomplete types?
   Yes, as long as the destructor is not called at the point of incomplete type.
   Useful in Pimpl idiom (destructor in .cpp).

3. What is the sizeof(unique_ptr<T>) compared to T*?
   Same: typically 8 bytes (on 64-bit). Zero overhead when using default deleter.

COMMON MISTAKES:
- Calling uptr.release() without deleting the raw pointer (memory leak)
- Trying to copy unique_ptr (must use std::move)
- Passing .get() to a function that deletes the pointer (double delete)
- Creating unique_ptr<T[]> with custom deleter and forgetting delete[]
- Using auto u = make_unique<Base>(args) when actual type is Derived (slicing)
*/
