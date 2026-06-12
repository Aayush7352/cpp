/*
THEORY: Move Constructor

Move Constructor: MyClass(MyClass&& other) noexcept;
- Transfers resources from 'other' to new object (no deep copy).
- 'other' is left in valid but unspecified state (typically null).
- Rvalue references (&&): bind to temporaries and objects marked with std::move().
- std::move(): cast to rvalue reference (doesn't actually move anything).
- noexcept: move operations should be noexcept for optimal container performance.
  (std::vector uses move instead of copy only if move is noexcept.)

Compiler-generated move constructor: member-wise move. Deleted if any member
cannot be moved (e.g., const or reference members).
*/

#include <iostream>
#include <vector>
#include <cstring>

class Buffer {
    char* data_;
    size_t size_;

public:
    // Constructor
    explicit Buffer(size_t sz) : data_{new char[sz]{}}, size_{sz} {
        std::cout << "Ctor(" << sz << ")\n";
    }

    // Destructor
    ~Buffer() {
        delete[] data_;
        std::cout << "Dtor\n";
    }

    // Copy constructor
    Buffer(const Buffer& other)
        : data_{new char[other.size_]}, size_{other.size_} {
        std::memcpy(data_, other.data_, size_);
        std::cout << "Copy ctor (" << size_ << ")\n";
    }

    // --- Move constructor ---
    Buffer(Buffer&& other) noexcept
        : data_{other.data_}, size_{other.size_} {
        other.data_ = nullptr;      // leave 'other' in empty state
        other.size_ = 0;
        std::cout << "Move ctor (" << size_ << ")\n";
    }

    // Copy assignment
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_];
            std::memcpy(data_, other.data_, size_);
        }
        std::cout << "Copy assign\n";
        return *this;
    }

    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        std::cout << "Move assign\n";
        return *this;
    }

    size_t size() const { return size_; }
};

struct MoveTest {
    Buffer buf;
    MoveTest() : buf{10} {}
    MoveTest(MoveTest&& other) noexcept = default;  // default member-wise move
};

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = default;
};

int main() {
    // 1. Move constructor from temporary
    std::cout << "=== Move from temporary ===\n";
    Buffer b1{Buffer{100}};  // Move ctor (or elided)

    std::cout << "\n=== Explicit move ===\n";
    Buffer b2{1024};
    Buffer b3{std::move(b2)};   // move constructor
    std::cout << "b2 size after move: " << b2.size() << '\n';  // 0

    // 2. Move into container
    std::cout << "\n=== Vector push_back with move ===\n";
    std::vector<Buffer> vec;
    vec.reserve(3);

    Buffer buf{64};
    vec.push_back(std::move(buf));   // move
    vec.emplace_back(128);           // in-place construction, no move
    vec.push_back(Buffer{256});      // temporary — move

    // 3. noexcept matters for vector growth
    std::cout << "\n=== noexcept and vector ===\n";
    struct MayThrow {
        MayThrow() = default;
        MayThrow(MayThrow&&) noexcept(false) {}
    };
    // vector will use copy instead of move for growth if move isn't noexcept

    // 4. Defaulted move
    std::cout << "\n=== Defaulted move ===\n";
    MoveTest mt1;
    MoveTest mt2{std::move(mt1)};   // calls default member-wise move

    // 5. std::move doesn't actually move
    std::cout << "\n=== std::move just casts ===\n";
    Buffer b4{50};
    // std::move(b4) — just returns Buffer&&, no bytes are moved
    Buffer b5{std::move(b4)};       // move constructor actually does the work

    return 0;
}

/*
SAMPLE I/O:
=== Move from temporary ===
Ctor(100)
Move ctor (100)
Dtor
Dtor

=== Explicit move ===
Ctor(1024)
Move ctor (1024)
b2 size after move: 0
Dtor
Dtor

=== Vector push_back with move ===
Ctor(64)
Move ctor (64)
Ctor(128)
Ctor(256)
Move ctor (256)
Dtor
Dtor
Dtor
Dtor
Dtor
(Note: destructors for vector elements + temporaries)

COMPLEXITY:
- Move: O(1) — just copies pointers
- std::move: O(1) — just a cast

INTERVIEW QUESTIONS:
1. What does std::move do?
   It's a cast to rvalue reference (T&&). It doesn't move anything by itself.
   It enables the move constructor/assignment to be called.

2. Why should move operations be noexcept?
   For optimal container reallocation. std::vector uses move instead of copy
   during growth only if the move operation is noexcept.

3. What is the "moved-from" state?
   A valid but unspecified state. The moved-from object can be destroyed or
   assigned to, but its value should not be assumed. Typically set to empty/null.

COMMON MISTAKES:
- Using moved-from object after move (assuming it still has data)
- Not marking move operations noexcept (performance penalty with containers)
- Marking members as const (prevents move — must use const_cast or redesign)
- Forgetting to actually transfer resources (just copying in move ctor)
- Overusing std::move — can inhibit copy elision
*/
