/*
THEORY: Move Assignment Operator

Move Assignment: MyClass& operator=(MyClass&& other) noexcept;
- Transfers resources from 'other' to *this.
- Releases old resources held by *this.
- Leaves 'other' in valid but unspecified state.
- Should be noexcept for optimal container operations.

Resource Transfer pattern:
1. Check self-assignment (optional but good practice).
2. Release current resources.
3. Steal pointers/handles from 'other'.
4. Set 'other' to null/empty.

noexcept: essential for containers. std::vector tries to use move during
reallocation; if move assignment is not noexcept, it falls back to copy.
*/

#include <iostream>
#include <vector>
#include <cstring>

class Buffer {
    char* data_;
    size_t size_;

public:
    Buffer() : data_{nullptr}, size_{0} {}
    explicit Buffer(size_t sz) : data_{new char[sz]{}}, size_{sz} {
        std::cout << "Ctor(" << sz << ")\n";
    }

    ~Buffer() {
        delete[] data_;
        std::cout << "Dtor\n";
    }

    Buffer(const Buffer& other) : data_{new char[other.size_]}, size_{other.size_} {
        std::memcpy(data_, other.data_, size_);
        std::cout << "Copy ctor\n";
    }

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

    // --- Move constructor ---
    Buffer(Buffer&& other) noexcept
        : data_{other.data_}, size_{other.size_} {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Move ctor\n";
    }

    // --- Move assignment operator ---
    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "Move assign";
        if (data_) std::cout << " (releasing " << size_ << " bytes)";
        std::cout << '\n';

        if (this != &other) {
            delete[] data_;                 // release old resource
            data_ = other.data_;            // steal pointer
            size_ = other.size_;            // steal size
            other.data_ = nullptr;           // other: null
            other.size_ = 0;
        }
        return *this;
    }

    // Swap-based move assignment (alternative, simpler)
    // Buffer& operator=(Buffer&& other) noexcept {
    //     swap(*this, other);
    //     return *this;
    // }

    size_t size() const { return size_; }
    bool empty() const { return data_ == nullptr; }
};

// Move-only type
class MoveOnly {
    Buffer buf_;
public:
    MoveOnly() : buf_{100} {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;
};

int main() {
    // 1. Move assignment from temporary
    std::cout << "=== Move assignment ===\n";
    Buffer b1{10};
    b1 = Buffer{100};               // move assign (temporary on RHS)
    std::cout << "b1 size after move assign: " << b1.size() << '\n';

    // 2. Explicit move with std::move
    std::cout << "\n=== std::move ===\n";
    Buffer b2{200};
    Buffer b3{5};
    b3 = std::move(b2);              // move assign
    std::cout << "b2 empty: " << b2.empty() << ", b3 size: " << b3.size() << '\n';

    // 3. Self-move assignment (rare, but should be safe)
    std::cout << "\n=== Self-move assignment ===\n";
    Buffer b4{50};
    b4 = std::move(b4);              // self-move: should be safe no-op
    std::cout << "b4 size after self-move: " << b4.size() << '\n';

    // 4. noexcept enables vector optimization
    std::cout << "\n=== noexcept and vector growth ===\n";
    std::vector<Buffer> vecv;
    vecv.reserve(1);
    vecv.emplace_back(1000);         // first element

    // This triggers reallocation, will use move ctor (noexcept) instead of copy
    vecv.emplace_back(2000);
    std::cout << "Vector size: " << vecv.size() << '\n';

    // 5. Comparing move vs copy performance
    std::cout << "\n=== Move vs Copy performance ===\n";
    Buffer src{1000000};             // large buffer
    Buffer dst{1};
    // dst = src;                     // copy: O(N)
    dst = std::move(src);             // move: O(1)
    std::cout << "Move: O(1) pointer swap, no deep copy\n";

    return 0;
}

/*
SAMPLE I/O:
=== Move assignment ===
Ctor(10)
Ctor(100)
Move assign (releasing 10 bytes)
Dtor
b1 size after move assign: 100
Dtor

=== std::move ===
Ctor(200)
Ctor(5)
Move assign (releasing 5 bytes)
b2 empty: 1, b3 size: 200
Dtor
Dtor

=== Self-move assignment ===
Ctor(50)
Move assign (releasing 50 bytes)
b4 size after self-move: 50
Dtor

=== noexcept and vector growth ===
Ctor(1000)
Ctor(2000)
Move ctor
Dtor
Vector size: 2
Dtor
Dtor

COMPLEXITY:
- Move assignment: O(1) — just pointer swap
- Copy assignment: O(N) — must duplicate data

INTERVIEW QUESTIONS:
1. Why should move assignment be noexcept?
   Containers like std::vector use std::move_if_noexcept during reallocation.
   If move operations throw, they fall back to copy (or only copy for strong guarantee).
   noexcept ensures optimal performance (move during reallocation).

2. What's the difference between move and copy performance?
   Move is O(1) (pointer/handle swap). Copy is O(N) (deep duplication).
   For large resources, difference is significant.

3. Self-move assignment — safe or not?
   It should be safe (no-op). But real code should avoid it.
   The standard requires that &x = std::move(x) leaves x in valid state.

COMMON MISTAKES:
- Not checking self-assignment (can cause use-after-free)
- Making move assignment not noexcept (performance penalty with containers)
- Leaving moved-from object in an invalid state (should be valid but unspecified)
- Not releasing old resources before stealing new ones (memory leak)
- Using std::move when a temporary already works (redundant)
*/
