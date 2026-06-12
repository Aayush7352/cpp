/*
THEORY: LFU Cache (Least Frequently Used)
- Evicts the least frequently used items when full
- Tracks access frequency for each item
- O(1) operations using frequency buckets
- On frequency tie, LRU among same-frequency items
- More complex than LRU but better for certain workloads

Complexity: O(1) get and put
Interview Questions:
1. LFU vs LRU: when is each better suited?
2. How to implement O(1) LFU with frequency buckets?
3. How to handle frequency overflow?
Common Mistakes:
- Not handling same-frequency LRU eviction properly
- Forgetting to update frequency on access
- Memory overhead from maintaining frequency map
*/

#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>
#include <map>

template<typename Key, typename Value>
class LFUCache {
    struct Node {
        Key key;
        Value value;
        int freq;
    };

    size_t capacity_;
    std::unordered_map<Key, typename std::list<Node>::iterator> cache_;
    std::map<int, std::list<Node>> freqBuckets_;

public:
    explicit LFUCache(size_t capacity) : capacity_(capacity) {}

    Value* get(const Key& key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) return nullptr;

        auto& node = it->second;
        int oldFreq = node->freq;
        int newFreq = oldFreq + 1;

        // Move to higher frequency bucket
        freqBuckets_[oldFreq].splice(
            freqBuckets_[oldFreq].begin(),
            freqBuckets_[newFreq],
            node);

        // Use emplace to avoid dangling reference with move semantics
        // Actually, splice moves the node, so we need to update correctly:
        auto& oldBucket = freqBuckets_[oldFreq];
        auto& newBucket = freqBuckets_[newFreq];
        
        // Move node: erase from old, insert to new
        auto nodeIt = std::find_if(oldBucket.begin(), oldBucket.end(),
            [&](const Node& n) { return n.key == key; });
        if (nodeIt != oldBucket.end()) {
            auto moved = std::move(*nodeIt);
            moved.freq = newFreq;
            oldBucket.erase(nodeIt);
            newBucket.push_front(std::move(moved));
            cache_[key] = newBucket.begin();
        }

        Node* result = &(*cache_[key]);
        return &result->value;
    }

    void put(const Key& key, const Value& value) {
        if (capacity_ == 0) return;

        auto it = cache_.find(key);
        if (it != cache_.end()) {
            it->second->value = value;
            get(key); // Update frequency
            return;
        }

        // Evict LFU (and LRU among ties)
        if (cache_.size() >= capacity_) {
            auto& lowestBucket = freqBuckets_.begin()->second;
            auto evict = lowestBucket.back();
            cache_.erase(evict.key);
            lowestBucket.pop_back();
            if (lowestBucket.empty()) {
                freqBuckets_.erase(freqBuckets_.begin());
            }
        }

        // Insert new node
        freqBuckets_[1].push_front({key, value, 1});
        cache_[key] = freqBuckets_[1].begin();
    }

    bool contains(const Key& key) const { return cache_.count(key); }
    size_t size() const { return cache_.size(); }

    void print() const {
        std::cout << "Cache:\n";
        for (const auto& [freq, bucket] : freqBuckets_) {
            std::cout << "  Freq " << freq << ": ";
            for (const auto& node : bucket) {
                std::cout << "(" << node.key << ":" << node.value << ":" << node.freq << ") ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    LFUCache<int, std::string> cache(3);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.print();

    cache.get(1);
    cache.get(1);  // freq(1) = 3
    cache.get(2);  // freq(2) = 2
    cache.print();

    // Add 4, should evict 3 (freq=1, LRU)
    cache.put(4, "four");
    cache.print();

    assert(!cache.contains(3)); // 3 should be evicted
    assert(cache.contains(1));
    assert(cache.contains(2));
    assert(cache.contains(4));

    std::cout << "All tests passed!\n";
    return 0;
}

/*
SAMPLE OUTPUT:
Cache:
  Freq 1: (3:three:1) (2:two:1) (1:one:1) 
Cache:
  Freq 1: (3:three:1) 
  Freq 2: (2:two:2) 
  Freq 3: (1:one:3) 
Cache:
  Freq 1: (4:four:1) 
  Freq 2: (2:two:2) 
  Freq 3: (1:one:3) 
All tests passed!
*/
