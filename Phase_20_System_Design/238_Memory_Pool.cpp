/*
THEORY: Memory Pool
- Fixed-size block allocation for high-performance scenarios
- Pre-allocates contiguous memory, maintains free list
- O(1) allocation and deallocation
- Reduces fragmentation and allocation overhead
- Ideal for real-time systems and game engines

Complexity: O(1) alloc/free
Interview Questions:
1. How to handle variable-size allocations? (multiple pool sizes)
2. Thread safety in pool allocator? (thread-local pools)
3. How to align memory for specific types? (std::align, alignas)
Common Mistakes:
- Memory leaks from not returning blocks to pool
- Using pool for objects with different sizes
- Not checking for pool exhaustion (out of memory)
*/

#include <iostream>
#include <vector>
#include <cstddef>
#include <cassert>

class MemoryPool {
    struct Block {
        Block* next;
    };

    Block* freeList_;
    void* pool_;
    size_t blockSize_;
    size_t poolSize_;
    size_t used_;

public:
    MemoryPool(size_t blockSize, size_t numBlocks)
        : blockSize_(blockSize), poolSize_(numBlocks), used_(0) {
        pool_ = ::operator new(blockSize_ * numBlocks);
        freeList_ = static_cast<Block*>(pool_);
        
        Block* current = freeList_;
        for (size_t i = 0; i < numBlocks - 1; ++i) {
            current->next = reinterpret_cast<Block*>(
                reinterpret_cast<char*>(current) + blockSize_);
            current = current->next;
        }
        current->next = nullptr;
    }

    ~MemoryPool() {
        ::operator delete(pool_);
    }

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    void* allocate() {
        if (!freeList_) {
            throw std::bad_alloc();
        }
        void* ptr = freeList_;
        freeList_ = freeList_->next;
        ++used_;
        return ptr;
    }

    void deallocate(void* ptr) {
        if (!ptr) return;
        Block* block = static_cast<Block*>(ptr);
        block->next = freeList_;
        freeList_ = block;
        --used_;
    }

    size_t available() const {
        return poolSize_ - used_;
    }

    size_t used() const { return used_; }

    template<typename T, typename... Args>
    T* construct(Args&&... args) {
        void* ptr = allocate();
        return new (ptr) T(std::forward<Args>(args)...);
    }

    template<typename T>
    void destroy(T* ptr) {
        if (ptr) {
            ptr->~T();
            deallocate(ptr);
        }
    }
};

struct Point {
    int x, y;
    Point(int a, int b) : x(a), y(b) {}
};

int main() {
    MemoryPool pool(sizeof(Point), 10);
    std::cout << "Pool created with 10 blocks of size " << sizeof(Point) << "\n";

    auto p1 = pool.construct<Point>(1, 2);
    auto p2 = pool.construct<Point>(3, 4);
    std::cout << "Allocated Point(" << p1->x << "," << p1->y << ")\n";
    std::cout << "Allocated Point(" << p2->x << "," << p2->y << ")\n";
    std::cout << "Used: " << pool.used() << ", Available: " << pool.available() << "\n";

    pool.destroy(p1);
    pool.destroy(p2);
    std::cout << "After deallocation - Used: " << pool.used() << "\n";
    return 0;
}

/*
SAMPLE OUTPUT:
Pool created with 10 blocks of size 8
Allocated Point(1,2)
Allocated Point(3,4)
Used: 2, Available: 8
After deallocation - Used: 0
*/
