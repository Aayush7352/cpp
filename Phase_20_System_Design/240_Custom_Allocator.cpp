/*
THEORY: Custom STL Allocator
- Custom allocator using memory pool (arena allocator)
- Satisfies Allocator concept for use with STL containers
- Provides O(1) allocation within fixed buffer
- Uses std::allocator_traits for customization
- Demonstrates stack-based (arena) allocator pattern

Complexity: O(1) allocation/deallocation
Interview Questions:
1. What allocator requirements must be met? (Allocator concept)
2. How to make allocator stateful? (C++11+ allows stateful allocators)
3. When to use custom allocators? (real-time, embedded, game dev)
Common Mistakes:
- Not providing rebind template (for containers like list/map)
- Forgetting to propagate on container copy/assignment
- Incorrect equality comparison (all allocators from same pool must compare equal)
*/

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

template<typename T, size_t PoolSize = 1024>
class ArenaAllocator {
    alignas(T) char buffer_[PoolSize * sizeof(T)];
    size_t offset_{0};

public:
    using value_type = T;

    ArenaAllocator() noexcept {}

    template<typename U>
    ArenaAllocator(const ArenaAllocator<U, PoolSize>&) noexcept {}

    T* allocate(size_t n) {
        if (offset_ + n > PoolSize) {
            throw std::bad_alloc();
        }
        T* result = reinterpret_cast<T*>(buffer_) + offset_;
        offset_ += n;
        return result;
    }

    void deallocate(T*, size_t) noexcept {
        // Arena allocator doesn't support individual deallocation
    }

    template<typename U>
    struct rebind {
        using other = ArenaAllocator<U, PoolSize>;
    };
};

template<typename T, size_t P1, typename U, size_t P2>
bool operator==(const ArenaAllocator<T, P1>&, const ArenaAllocator<U, P2>&) {
    return P1 == P2;
}

template<typename T, size_t P1, typename U, size_t P2>
bool operator!=(const ArenaAllocator<T, P1>&, const ArenaAllocator<U, P2>&) {
    return P1 != P2;
}

int main() {
    ArenaAllocator<int> alloc;
    std::vector<int, ArenaAllocator<int>> vec(alloc);
    
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    
    std::cout << "Vector using arena allocator: ";
    for (auto x : vec) std::cout << x << " ";
    std::cout << "\nCapacity: " << vec.capacity() << "\n";
    
    ArenaAllocator<std::pair<const int, std::string>> mapAlloc;
    std::map<int, std::string, std::less<int>, 
             ArenaAllocator<std::pair<const int, std::string>>> myMap(mapAlloc);
    
    myMap[1] = "one";
    myMap[2] = "two";
    
    std::cout << "Map using arena allocator:\n";
    for (const auto& [k, v] : myMap) {
        std::cout << "  " << k << ": " << v << "\n";
    }
    
    return 0;
}

/*
SAMPLE OUTPUT:
Vector using arena allocator: 10 20 30 
Capacity: 4
Map using arena allocator:
  1: one
  2: two
*/
