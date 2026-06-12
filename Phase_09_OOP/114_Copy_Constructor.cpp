/*
THEORY: Copy Constructor

Copy Constructor: MyClass(const MyClass& other);
- Creates a new object as a copy of an existing object.
- Shallow copy: member-wise copy (default). For pointer members, both point to same memory.
- Deep copy: allocates new memory and copies the content. Required for owning raw pointers.
- Copy elision (NRVO/NRVO, RVO): compiler may omit copy/move operations.
- Rule of Three: If you need custom destructor, copy constructor, or copy assignment,
  you likely need all three.
- Rule of Five: same + move constructor + move assignment (C++11).
*/

#include <iostream>
#include <cstring>
#include <algorithm>

class StringBuffer {
    char* data_;
    size_t size_;

public:
    // Constructor
    explicit StringBuffer(const char* str = "")
        : size_{std::strlen(str)}, data_{new char[size_ + 1]} {
        std::strcpy(data_, str);
        std::cout << "Ctor: \"" << data_ << "\"\n";
    }

    // --- Shallow copy (default) — PROBLEMATIC ---
    // StringBuffer(const StringBuffer& other)
    //     : data_{other.data_}, size_{other.size_} { }  // double delete!

    // Deep copy constructor
    StringBuffer(const StringBuffer& other)
        : size_{other.size_}, data_{new char[size_ + 1]} {
        std::strcpy(data_, other.data_);
        std::cout << "Copy ctor: \"" << data_ << "\"\n";
    }

    // Destructor
    ~StringBuffer() {
        delete[] data_;
        std::cout << "Dtor\n";
    }

    // Copy assignment (see 116_Copy_Assignment.cpp)
    StringBuffer& operator=(const StringBuffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        }
        std::cout << "Copy assign\n";
        return *this;
    }

    const char* c_str() const { return data_; }
};

// --- Copy elision demo ---
struct ElisionDemo {
    int id;
    ElisionDemo(int i) : id{i} { std::cout << "Ctor(" << i << ")\n"; }
    ElisionDemo(const ElisionDemo&) { std::cout << "Copy ctor\n"; }
    ElisionDemo(ElisionDemo&&) noexcept { std::cout << "Move ctor\n"; }
    ~ElisionDemo() { std::cout << "Dtor(" << id << ")\n"; }
};

ElisionDemo makeElision(int i) {
    ElisionDemo local{i};        // Ctor
    return local;                // NRVO — may elide copy/move
}

int main() {
    // 1. Deep copy
    std::cout << "=== Deep copy ===\n";
    StringBuffer s1{"Hello"};
    StringBuffer s2{s1};          // deep copy
    std::cout << "s1: " << s1.c_str() << "\ns2: " << s2.c_str() << '\n';

    // 2. Copy elision / NRVO (compiler dependent)
    std::cout << "\n=== Copy elision (NRVO) ===\n";
    auto obj = makeElision(42);   // May see 0 or 1 copies
    std::cout << "obj.id = " << obj.id << '\n';

    // 3. RVO (return by value)
    std::cout << "\n=== RVO ===\n";
    auto obj2 = []() {
        return ElisionDemo{99};
    }();                           // Guaranteed copy elision since C++17
    std::cout << "obj2.id = " << obj2.id << '\n';

    // 4. Preventing copies
    std::cout << "\n=== Deleted copy ===\n";
    struct NoCopy {
        int val;
        NoCopy(int v) : val{v} {}
        NoCopy(const NoCopy&) = delete;
        NoCopy& operator=(const NoCopy&) = delete;
    };
    NoCopy nc1{5};
    // NoCopy nc2{nc1}; // ERROR: copy deleted

    return 0;
}

/*
SAMPLE I/O:
=== Deep copy ===
Ctor: "Hello"
Copy ctor: "Hello"
s1: Hello
s2: Hello
Dtor
Dtor

=== Copy elision (NRVO) ===
Ctor(42)
obj.id = 42
Dtor(42)
(Note: no Copy ctor called — elided)

=== RVO ===
Ctor(99)
obj2.id = 99
Dtor(99)

=== Deleted copy ===
(Ctor only — no error)

COMPLEXITY:
- Deep copy: O(N) time and space (N = string length)
- Shallow copy: O(1) but dangerous

INTERVIEW QUESTIONS:
1. Shallow vs deep copy — when is each appropriate?
   Shallow: POD types, shared ownership (no resource cleanup).
   Deep: owning raw pointers (heap memory, file handles, etc.).
   Prefer smart pointers or RAII wrappers to avoid manual deep copy.

2. What is copy elision and NRVO?
   Compiler optimization to avoid unnecessary copies.
   Named Return Value Optimization: eliding copy of local variable returned by value.
   Guaranteed since C++17 in certain cases.

3. What is the Rule of Three?
   If you need a custom destructor (to release resources), you likely also need
   custom copy constructor and copy assignment operator to correctly manage
   shared/owned resources.

COMMON MISTAKES:
- Shallow copy when deep copy needed (double delete, use-after-free)
- Forgetting to allocate +1 for null terminator in C strings
- Not handling self-assignment in copy assignment
- Relying on copy elision for correctness (program must work without elision too)
- Passing by value unnecessarily (causes copies)
*/
