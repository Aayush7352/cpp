/*
THEORY: Memory Management — RAII, Rule of Five, Leak Detection

RAII (Resource Acquisition Is Initialization):
- Resource management tied to object lifetime.
- Constructor acquires, destructor releases.
- Exceptions safe: stack unwinding calls destructors.

Rule of Five (C++11):
If you need to define any of these, define all:
1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

Rule of Three (C++98): 1-3 only (before move semantics).

Memory leak detection:
- Valgrind (Linux), AddressSanitizer (ASan), LeakSanitizer
- Heaptrack, Dr. Memory
- Overriding operator new/delete for tracking
*/

#include <iostream>
#include <cstring>
#include <vector>

// --- RAII Wrapper for raw memory ---
class Buffer {
    char* data_;
    size_t size_;

public:
    explicit Buffer(size_t sz) : data_{new char[sz]{}}, size_{sz} {
        std::cout << "Buffer(" << size_ << ") acquired\n";
    }

    // Destructor
    ~Buffer() {
        delete[] data_;
        std::cout << "~Buffer() released\n";
    }

    // Copy constructor — deep copy
    Buffer(const Buffer& other)
        : data_{new char[other.size_]}, size_{other.size_} {
        std::memcpy(data_, other.data_, size_);
        std::cout << "Buffer copy ctor\n";
    }

    // Copy assignment — copy-and-swap idiom
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            Buffer tmp{other};          // deep copy
            std::swap(data_, tmp.data_);
            std::swap(size_, tmp.size_);
        }
        std::cout << "Buffer copy assign\n";
        return *this;
    }

    // Move constructor
    Buffer(Buffer&& other) noexcept
        : data_{other.data_}, size_{other.size_} {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "Buffer move ctor\n";
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
        std::cout << "Buffer move assign\n";
        return *this;
    }

    size_t size() const { return size_; }
    char* data() { return data_; }
};

// --- Simple memory leak tracker ---
class LeakTracker {
    static inline int alloc_count{0};

public:
    static void* track_alloc(size_t sz) {
        ++alloc_count;
        std::cout << "Allocation #" << alloc_count << " (" << sz << " bytes)\n";
        return std::malloc(sz);
    }

    static void track_free(void* p) {
        --alloc_count;
        std::cout << "Free. Remaining allocations: " << alloc_count << '\n';
        std::free(p);
    }

    static int current_allocs() { return alloc_count; }
};

int main() {
    // 1. RAII with Buffer
    std::cout << "=== RAII Buffer ===\n";
    {
        Buffer buf{32};                  // acquire
        // use buffer ...
    }                                    // automatically released

    // 2. Copy and move
    std::cout << "\n=== Copy and Move ===\n";
    Buffer a{10};
    Buffer b{a};                         // copy
    Buffer c{std::move(a)};              // move
    // a is now empty (null)

    // 3. Vector of Buffer (move-only aware)
    std::cout << "\n=== vector of Buffer ===\n";
    std::vector<Buffer> vec;
    vec.reserve(3);
    vec.push_back(Buffer{8});            // temporary — move
    vec.emplace_back(16);                // in-place construction

    // 4. Rule of Five check
    std::cout << "\n=== Rule of Five testing ===\n";
    Buffer x{4};
    Buffer y{8};
    x = y;                               // copy assignment
    x = std::move(y);                    // move assignment

    // 5. Memory leak detection (manual tracking)
    std::cout << "\n=== leak detection ===\n";
    void* mem1{LeakTracker::track_alloc(100)};
    void* mem2{LeakTracker::track_alloc(200)};
    LeakTracker::track_free(mem1);
    // mem2 not freed — would be reported as leak
    LeakTracker::track_free(mem2);

    // 6. AddressSanitizer example (compile with -fsanitize=address)
    // int* leak = new int(42);  // ASan would report this as leak
    // int* oob = new int[5]; oob[5] = 0;  // ASan: heap-buffer-overflow

    return 0;
}

/*
SAMPLE I/O:
=== RAII Buffer ===
Buffer(32) acquired
~Buffer() released

=== Copy and Move ===
Buffer(10) acquired
Buffer copy ctor
Buffer move ctor

=== vector of Buffer ===
Buffer(8) acquired
Buffer move ctor
~Buffer() released
Buffer(16) acquired

=== Rule of Five testing ===
Buffer(4) acquired
Buffer(8) acquired
Buffer copy ctor
Buffer copy assign
~Buffer() released
Buffer move assign

=== leak detection ===
Allocation #1 (100 bytes)
Allocation #2 (200 bytes)
Free. Remaining allocations: 1
Free. Remaining allocations: 0

COMPLEXITY:
- Buffer operations: O(N) for copy, O(1) for move
- LeakTracker: O(1) per alloc/free

INTERVIEW QUESTIONS:
1. What is RAII and why is it important?
   Ties resource lifecycle to object lifetime. Ensures exception-safe resource
   cleanup. Core concept of C++ resource management.

2. What is the Rule of Five / Three?
   If you need custom destructor (managing raw resource), you likely also need
   custom copy/move operations to correctly handle ownership semantics.

3. How does AddressSanitizer detect memory leaks?
   Compiler instruments code, tracks allocations at runtime, reports unreleased
   memory on program exit. Detection at compile-time (-fsanitize=address).

COMMON MISTAKES:
- Violating Rule of Five: custom dtor but no copy/move ops (shallow copy, double delete)
- Throwing from destructor (should never throw)
- Not making move operations noexcept (prevents optimal container usage)
- Forgetting to delete pointer members in destructor
- Manually calling destructor on stack objects (double destruction)
- Using raw pointers where RAII would work (new/delete outside constructor/destructor)
*/
