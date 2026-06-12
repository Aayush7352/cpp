/*
THEORY: shared_ptr - Shared Ownership Smart Pointer

- std::shared_ptr<T>: reference-counted smart pointer.
- Multiple shared_ptr can own the same object.
- Control block stores: ref count, weak count, deleter, allocator.
- std::make_shared<T>(args...): single allocation (object + control block), preferred.
- Aliasing constructor: shared_ptr that points to a different object but shares ownership.
- Custom deleter: type-erased in control block (not part of template parameter).
- Circular references: two objects owning each other creates a memory leak (use weak_ptr).
*/

#include <iostream>
#include <memory>
#include <vector>

struct Node {
    int value;
    std::shared_ptr<Node> next;
    explicit Node(int v) : value{v} { std::cout << "Node(" << v << ") created\n"; }
    ~Node() { std::cout << "Node(" << value << ") destroyed\n"; }
};

struct Employee {
    std::string name;
    std::shared_ptr<Employee> manages;  // circular if not weak_ptr
    Employee(std::string n) : name{n} {}
    ~Employee() { std::cout << "~Employee(" << name << ")\n"; }
};

int main() {
    // 1. Basic usage — make_shared
    std::cout << "=== make_shared ===\n";
    auto sptr{std::make_shared<int>(42)};
    std::cout << "*sptr = " << *sptr << ", use_count = " << sptr.use_count() << '\n';

    // 2. Copying increases ref count
    std::cout << "\n=== shared ownership ===\n";
    auto sptr2{sptr};
    std::cout << "After copy: use_count = " << sptr.use_count() << '\n';

    auto sptr3{sptr2};
    std::cout << "After second copy: use_count = " << sptr.use_count() << '\n';

    sptr2.reset();
    std::cout << "After reset one: use_count = " << sptr.use_count() << '\n';

    sptr3.reset();
    std::cout << "After reset two: use_count = " << sptr.use_count() << '\n';
    // last one destroys

    // 3. Custom deleter
    std::cout << "\n=== custom deleter ===\n";
    auto deleter = [](int* p) {
        std::cout << "Custom deleter called for " << *p << '\n';
        delete p;
    };
    std::shared_ptr<int> custom{new int{77}, deleter};
    std::cout << "*custom = " << *custom << '\n';

    // 4. Aliasing constructor
    std::cout << "\n=== aliasing constructor ===\n";
    struct Pair { int a; int b; };
    auto spair{std::make_shared<Pair>(1, 2)};
    std::shared_ptr<int> alias{spair, &spair->b};  // shares ownership, points to b
    std::cout << "*alias = " << *alias << '\n';
    std::cout << "use_count from alias = " << alias.use_count() << '\n';

    // 5. Circular reference (problem)
    std::cout << "\n=== circular reference (leak!) ===\n";
    {
        auto alice{std::make_shared<Employee>("Alice")};
        auto bob{std::make_shared<Employee>("Bob")};
        alice->manages = bob;
        bob->manages = alice;
        // Destructors never called — ref count never reaches 0
        std::cout << "Alice use_count: " << alice.use_count() << '\n';
        std::cout << "Bob use_count: " << bob.use_count() << '\n';
        std::cout << "Memory leak: ~Employee not called!\n";
    }

    // 6. enable_shared_from_this
    std::cout << "\n=== shared_from_this ===\n";
    struct SelfShared : std::enable_shared_from_this<SelfShared> {
        int id;
        explicit SelfShared(int i) : id{i} {}
        std::shared_ptr<SelfShared> getptr() { return shared_from_this(); }
    };

    auto ss{std::make_shared<SelfShared>(5)};
    auto ss2{ss->getptr()};
    std::cout << "use_count = " << ss.use_count() << '\n';

    return 0;
}

/*
SAMPLE I/O:
=== make_shared ===
*sptr = 42, use_count = 1

=== shared ownership ===
After copy: use_count = 2
After second copy: use_count = 3
After reset one: use_count = 2
After reset two: use_count = 1

=== custom deleter ===
Custom deleter called for 77

=== aliasing constructor ===
*alias = 2
use_count from alias = 2

=== circular reference (leak!) ===
Alice use_count: 2
Bob use_count: 2
Memory leak: ~Employee not called!

=== shared_from_this ===
use_count = 2

COMPLEXITY:
- Copy/Assign: O(1) atomic ref count increment/decrement
- make_shared: single allocation (object + control block)
- Destructor: O(1) atomic decrement, O(N) if owning array

INTERVIEW QUESTIONS:
1. Difference between make_shared and shared_ptr(new T)?
   make_shared allocates object and control block together (1 alloc vs 2).
   make_shared is exception-safe. However, the memory stays alive as long as
   any weak_ptr exists (control block persists).

2. Why does shared_ptr have no array specialization?
   Before C++17, no. C++17 added shared_ptr<T[]> but it's uncommon.
   Prefer vector or unique_ptr<T[]> for arrays.

3. What is aliasing constructor?
   Allows shared_ptr to point to one object while sharing ownership of another.
   Useful for pointing to a member while keeping the parent alive.

COMMON MISTAKES:
- Creating circular references (memory leak)
- Using shared_ptr when unique_ptr suffices (performance overhead)
- Forgetting enable_shared_from_this when returning shared_ptr from member
- Passing raw pointer to two shared_ptr constructors (double free)
- Using .get() when the shared_ptr may be destroyed first
*/
