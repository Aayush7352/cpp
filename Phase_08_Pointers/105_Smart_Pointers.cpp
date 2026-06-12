/*
THEORY: Smart Pointers Overview

C++11 introduced smart pointers in <memory> to automate resource management (RAII).

| Pointer       | Ownership     | Copyable | Ref count | When to use                     |
|---------------|-------------|----------|-----------|----------------------------------|
| auto_ptr      | Exclusive   | No (broken copy) | No | Deprecated in C++11, removed in C++17 |
| unique_ptr    | Exclusive   | No (move only) | No | Default smart pointer            |
| shared_ptr    | Shared      | Yes      | Yes       | Multiple owners                  |
| weak_ptr      | None (observer) | Yes   | Yes (no count) | Break shared_ptr cycles      |

auto_ptr (C++98): DEPRECATED. Has problematic copy semantics (transfers ownership silently).
Never use in modern C++ — replaced by unique_ptr.
*/

#include <iostream>
#include <memory>

struct Resource {
    int id;
    explicit Resource(int n) : id{n} { std::cout << "Acquire Resource " << id << '\n'; }
    ~Resource() { std::cout << "Release Resource " << id << '\n'; }
    void greet() const { std::cout << "Hello from Resource " << id << '\n'; }
};

int main() {
    std::cout << "=== unique_ptr (exclusive ownership) ===\n";
    {
        std::unique_ptr<Resource> uptr{std::make_unique<Resource>(1)};
        uptr->greet();
        // auto uptr2 = uptr; // ERROR: copy deleted
        auto uptr2 = std::move(uptr);  // transfer ownership
        // uptr is now null
        if (!uptr) std::cout << "uptr is null after move\n";
        uptr2->greet();
    } // Resource 1 released here

    std::cout << "\n=== shared_ptr (shared ownership) ===\n";
    {
        std::shared_ptr<Resource> sptr{std::make_shared<Resource>(2)};
        std::cout << "Use count: " << sptr.use_count() << '\n';
        {
            std::shared_ptr<Resource> sptr2{sptr};
            std::cout << "Use count (inner scope): " << sptr.use_count() << '\n';
        }
        std::cout << "Use count (outer scope): " << sptr.use_count() << '\n';
    } // Resource 2 released when last shared_ptr dies

    std::cout << "\n=== weak_ptr (observer, no ownership) ===\n";
    {
        std::shared_ptr<Resource> sptr{std::make_shared<Resource>(3)};
        std::weak_ptr<Resource> wptr{sptr};

        if (auto locked = wptr.lock()) {  // returns shared_ptr
            locked->greet();
            std::cout << "Use count during lock: " << sptr.use_count() << '\n';
        }

        sptr.reset();  // release ownership
        if (wptr.expired()) {
            std::cout << "weak_ptr expired after reset\n";
        }
    } // no double deletion

    std::cout << "\n=== auto_ptr (deprecated, DO NOT USE) ===\n";
#if __cplusplus < 201703L
    std::auto_ptr<Resource> apt(new Resource(4));
    std::auto_ptr<Resource> apt2(apt);  // ownership transferred silently!
    // apt is now null — dangerous!
    apt2->greet();
#endif

    return 0;
}

/*
SAMPLE I/O:
=== unique_ptr (exclusive ownership) ===
Acquire Resource 1
Hello from Resource 1
uptr is null after move
Hello from Resource 1
Release Resource 1

=== shared_ptr (shared ownership) ===
Acquire Resource 2
Use count: 1
Use count (inner scope): 2
Use count (outer scope): 1
Release Resource 2

=== weak_ptr (observer, no ownership) ===
Acquire Resource 3
Hello from Resource 3
Use count during lock: 2
weak_ptr expired after reset

COMPLEXITY:
- All operations: O(1)
- shared_ptr copy/assign: atomic increment/decrement of ref count

INTERVIEW QUESTIONS:
1. Why is auto_ptr deprecated?
   Copy semantics silently transfer ownership, making it easy to accidentally
   create null pointers or double-delete. unique_ptr enforces move-only semantics.

2. What is the overhead of shared_ptr vs unique_ptr?
   shared_ptr has a control block (ref count, weak count, deleter) — 2x heap alloc
   unless make_shared is used. unique_ptr has zero overhead (same size as raw pointer).

3. When would you use weak_ptr?
   To break circular references with shared_ptr (e.g., parent-child graphs).
   Observer pattern where you need to check if object still exists.

COMMON MISTAKES:
- Using auto_ptr in C++11 or later
- Creating circular shared_ptr references (memory leak)
- Passing raw pointer to two shared_ptr constructors (double delete)
- Using .get() after the pointer is released
- Forgetting that make_shared prevents separate control block allocation
*/
