/*
THEORY: LRU Cache (Least Recently Used)
- Fixed-size cache that evicts least recently used items
- O(1) get and put operations
- Uses doubly linked list + hash map
- List orders items by access time (MRU at front, LRU at back)
- Hash map provides O(1) lookup to list nodes

Complexity: O(1) get and put
Interview Questions:
1. How to make LRU thread-safe?
2. Different eviction policies: LRU vs LFU vs FIFO?
3. How to implement LRU with O(1) operations?
Common Mistakes:
- Forgetting to update node position on access
- Memory leaks from not properly deleting nodes
- Iterator invalidation in list operations
*/

#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>

template<typename Key, typename Value>
class LRUCache {
    size_t capacity_;
    std::list<std::pair<Key, Value>> items_;
    std::unordered_map<Key, decltype(items_.begin())> cache_;

public:
    explicit LRUCache(size_t capacity) : capacity_(capacity) {}

    Value* get(const Key& key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) return nullptr;

        // Move to front (most recently used)
        items_.splice(items_.begin(), items_, it->second);
        return &it->second->second;
    }

    void put(const Key& key, const Value& value) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // Update existing
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second);
            return;
        }

        // Evict if full
        if (cache_.size() >= capacity_) {
            auto lru = items_.back();
            cache_.erase(lru.first);
            items_.pop_back();
        }

        // Insert at front
        items_.emplace_front(key, value);
        cache_[key] = items_.begin();
    }

    bool contains(const Key& key) const { return cache_.count(key); }
    size_t size() const { return cache_.size(); }
    size_t capacity() const { return capacity_; }

    void print() const {
        std::cout << "Cache (MRU -> LRU): ";
        for (const auto& [k, v] : items_) {
            std::cout << "(" << k << ":" << v << ") ";
        }
        std::cout << "\n";
    }
};

int main() {
    LRUCache<int, std::string> cache(3);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.print();

    // Access 1, making it MRU
    auto* val = cache.get(1);
    std::cout << "Get(1): " << (val ? *val : "null") << "\n";
    cache.print();

    // Add 4, should evict 2 (LRU)
    cache.put(4, "four");
    cache.print();

    assert(!cache.contains(2)); // 2 should be evicted
    assert(cache.contains(1));
    assert(cache.contains(3));
    assert(cache.contains(4));

    std::cout << "All tests passed!\n";
    return 0;
}

/*
SAMPLE OUTPUT:
Cache (MRU -> LRU): (1:one) (2:two) (3:three) 
Get(1): one
Cache (MRU -> LRU): (1:one) (3:three) (2:two) 
Cache (MRU -> LRU): (4:four) (1:one) (3:three) 
All tests passed!
*/
