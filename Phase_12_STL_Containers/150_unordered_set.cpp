/*
==================================================================
150. std::unordered_set
==================================================================
Theory:
- Hash-based container: average O(1) insert/find/erase
- Bucket interface: bucket_count, bucket_size, bucket(key)
- Custom hash function: can specialize std::hash or provide
  functor
- No ordering of elements
- Load factor: avg elements per bucket; rehash when exceeded
- reserve: pre-allocate buckets
*/

#include <iostream>
#include <unordered_set>
#include <string>
#include <functional>

// Custom hash for std::pair<int,int>
struct PairHash {
    std::size_t operator()(const std::pair<int,int>& p) const noexcept {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1); // combine
    }
};

struct Person {
    std::string name;
    int age;

    bool operator==(const Person& o) const {
        return name == o.name && age == o.age;
    }
};

struct PersonHash {
    std::size_t operator()(const Person& p) const noexcept {
        auto h1 = std::hash<std::string>{}(p.name);
        auto h2 = std::hash<int>{}(p.age);
        return h1 ^ (h2 << 1);
    }
};

int main() {
    std::cout << "=== std::unordered_set\n\n";

    // --- Basic ---
    std::cout << "1. Basic operations:\n";
    std::unordered_set<int> us{5, 2, 8, 1, 9, 2, 5};
    std::cout << "   Content (no order): ";
    for (int x : us) std::cout << x << " ";
    std::cout << "\n   Size: " << us.size() << "\n\n";

    // --- Insert / Find / Erase ---
    std::cout << "2. Insert/Find/Erase:\n";
    us.insert(3);
    std::cout << "   Insert 3\n";
    auto it = us.find(5);
    if (it != us.end())
        std::cout << "   Found: " << *it << "\n";
    us.erase(5);
    std::cout << "   Erase 5. Now: ";
    for (int x : us) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Bucket interface ---
    std::cout << "3. Bucket interface:\n";
    std::unordered_set<int> bucketDemo;
    for (int i = 0; i < 20; ++i)
        bucketDemo.insert(i * 7);
    std::cout << "   Bucket count: " << bucketDemo.bucket_count() << "\n";
    std::cout << "   Load factor: " << bucketDemo.load_factor() << "\n";
    std::cout << "   Max load factor: " << bucketDemo.max_load_factor() << "\n";
    for (size_t i = 0; i < bucketDemo.bucket_count(); ++i) {
        if (bucketDemo.bucket_size(i) > 0)
            std::cout << "   Bucket " << i << ": size " << bucketDemo.bucket_size(i)
                      << " (key " << bucketDemo.begin(i) << ")\n";
    }
    std::cout << "\n";

    // --- Custom hash (pair) ---
    std::cout << "4. Custom hash for pair<int,int>:\n";
    std::unordered_set<std::pair<int,int>, PairHash> pairSet;
    pairSet.insert({1, 2});
    pairSet.insert({3, 4});
    pairSet.insert({1, 2}); // duplicate
    std::cout << "   pairSet size: " << pairSet.size() << " (1 dup ignored)\n";
    for (const auto& p : pairSet)
        std::cout << "   (" << p.first << "," << p.second << ")\n";
    std::cout << "\n";

    // --- Custom class ---
    std::cout << "5. Custom class with hash:\n";
    std::unordered_set<Person, PersonHash> people;
    people.insert({"Alice", 30});
    people.insert({"Bob", 25});
    people.insert({"Alice", 30}); // dup
    std::cout << "   people size: " << people.size() << "\n";
    for (const auto& p : people)
        std::cout << "   " << p.name << ", " << p.age << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic operations:
   Content (no order): 9 1 8 2 5
   Size: 5

2. Insert/Find/Erase:
   Insert 3
   Found: 5
   Erase 5. Now: 3 9 1 8 2

3. Bucket interface:
   Bucket count: 41
   Load factor: 0.487805
   Max load factor: 1
   Bucket 2: size 1 (key 14)
   Bucket 7: size 1 (key 7)
   ...

4. Custom hash for pair<int,int>:
   pairSet size: 2
   (1,2)
   (3,4)

5. Custom class with hash:
   people size: 2
   Alice, 30
   Bob, 25
==================================================================
Complexity:
- Average: O(1) for insert/find/erase
- Worst-case: O(n) (hash collisions)
- Bucket operations: O(bucket_count)
==================================================================
Interview Questions:
1. When to use unordered_set vs set?
   -> unordered_set: average O(1), no ordering.
      set: O(log n), ordered iteration.

2. How to provide custom hash for a class?
   -> Specialize std::hash or define hash functor + operator==.

3. What is load factor?
   -> avg = size / bucket_count. When exceeded, rehash doubles
      buckets.
==================================================================
Common Mistakes:
- Forgetting operator== for custom types
- Using bad hash function (too many collisions)
- Assuming order is deterministic (it's not)
- Modifying key in-place (undefined behavior)
- Not calling reserve() when many inserts expected
==================================================================
*/
