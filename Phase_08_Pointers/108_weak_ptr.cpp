/*
THEORY: weak_ptr - Non-owning Observer

- std::weak_ptr<T>: holds a "weak reference" to an object managed by shared_ptr.
- Does NOT increase the reference count.
- lock(): returns a shared_ptr (or null if object was destroyed).
- expired(): checks if object has been deleted.
- Used to break circular references in shared_ptr ownership.

Common pattern:
- Parent owns children via shared_ptr.
- Children refer back to parent via weak_ptr (avoid cycle).
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Employee {
    std::string name;
    std::vector<std::shared_ptr<Employee>> subordinates;
    std::weak_ptr<Employee> manager;   // weak back-reference

    explicit Employee(std::string n) : name{n} {
        std::cout << "Employee(" << name << ") created\n";
    }
    ~Employee() { std::cout << "Employee(" << name << ") destroyed\n"; }
};

int main() {
    // 1. Basic weak_ptr usage
    std::cout << "=== basic weak_ptr ===\n";
    std::weak_ptr<int> wptr;

    {
        auto sptr{std::make_shared<int>(42)};
        wptr = sptr;                     // assign — does NOT increase ref count
        std::cout << "use_count = " << wptr.use_count() << '\n';

        if (auto locked = wptr.lock()) {  // get shared_ptr
            std::cout << "Locked value: " << *locked << '\n';
        }
    } // sptr destroyed, object freed

    std::cout << "use_count after scope = " << wptr.use_count() << '\n';
    if (wptr.expired()) {
        std::cout << "weak_ptr expired (object destroyed)\n";
    }

    auto locked = wptr.lock();
    if (!locked) {
        std::cout << "lock() returned nullptr\n";
    }

    // 2. Breaking circular reference with weak_ptr
    std::cout << "\n=== breaking circular ref ===\n";
    {
        auto ceo{std::make_shared<Employee>("Alice")};
        auto dev{std::make_shared<Employee>("Bob")};

        dev->manager = ceo;              // weak_ptr — no cycle
        ceo->subordinates.push_back(dev); // shared_ptr

        std::cout << "CEO use_count: " << ceo.use_count() << '\n';
        std::cout << "Dev use_count: " << dev.use_count() << '\n';

        // Access manager via weak_ptr
        if (auto mgr = dev->manager.lock()) {
            std::cout << "Bob's manager: " << mgr->name << '\n';
        }
    } // Both correctly destroyed (no cycle)
    std::cout << "Both destroyed successfully\n";

    // 3. Weak_ptr as cache
    std::cout << "\n=== weak_ptr cache pattern ===\n";
    auto create = [](int key) {
        return std::make_shared<std::string>("Value_" + std::to_string(key));
    };

    std::weak_ptr<std::string> cache;
    {
        auto res{create(1)};
        cache = res;
        if (auto cached = cache.lock()) {
            std::cout << "Cache hit: " << *cached << '\n';
        }
    } // resource released

    if (cache.expired()) {
        std::cout << "Cache miss (object freed)\n";
    }

    return 0;
}

/*
SAMPLE I/O:
=== basic weak_ptr ===
use_count = 1
Locked value: 42
use_count after scope = 0
weak_ptr expired (object destroyed)
lock() returned nullptr

=== breaking circular ref ===
Employee(Alice) created
Employee(Bob) created
CEO use_count: 2
Dev use_count: 1
Bob's manager: Alice
Employee(Bob) destroyed
Employee(Alice) destroyed
Both destroyed successfully

=== weak_ptr cache pattern ===
Cache hit: Value_1
Cache miss (object freed)

COMPLEXITY:
- lock(): O(1) — atomically checks and creates shared_ptr if still alive
- expired(), use_count(): O(1)

INTERVIEW QUESTIONS:
1. Can weak_ptr be dereferenced directly?
   No. Must call lock() to get a shared_ptr first. Direct dereference would
   be unsafe as the object may have been destroyed.

2. What happens if you extend lock()'s result across threads?
   The shared_ptr returned by lock() keeps the object alive for the duration
   of the lock, making it safe to use across threads as long as you hold it.

3. Use_count() on an expired weak_ptr returns 0. True or false?
   True. Once the managed object is destroyed, use_count() returns 0.

COMMON MISTAKES:
- Using weak_ptr without locking (calling -> or * on weak_ptr directly)
- Holding lock() result too long (defeats purpose of weak reference)
- Forgetting to check expired() or lock() result for null
- Using weak_ptr where unique_ptr is appropriate (unnecessary overhead)
- Circular dependency not fully broken (one shared_ptr still cycles)
*/
