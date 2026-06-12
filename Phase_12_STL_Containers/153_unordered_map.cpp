/*
==================================================================
153. std::unordered_map
==================================================================
Theory:
- Hash-based key-value store: average O(1) lookup
- operator[]: inserts default if key missing
- Bucket interface, load factor, rehash
- Custom hash function for user-defined keys
- No ordering; iteration order is unspecified
*/

#include <iostream>
#include <unordered_map>
#include <string>
#include <functional>

struct City {
    std::string name;
    std::string state;

    bool operator==(const City& o) const {
        return name == o.name && state == o.state;
    }
};

struct CityHash {
    std::size_t operator()(const City& c) const noexcept {
        return std::hash<std::string>{}(c.name) ^
               (std::hash<std::string>{}(c.state) << 1);
    }
};

int main() {
    std::cout << "=== std::unordered_map\n\n";

    // --- Basic ---
    std::cout << "1. Basic operations:\n";
    std::unordered_map<std::string, double> prices;
    prices["apple"] = 1.99;
    prices["banana"] = 0.59;
    prices["cherry"] = 3.49;

    for (const auto& [item, price] : prices)
        std::cout << "   " << item << ": $" << price << "\n";
    std::cout << "\n";

    // --- Insert ---
    std::cout << "2. Insert:\n";
    auto [it, success] = prices.insert({"apple", 2.99});
    std::cout << "   Insert apple:2.99 -> " << (success ? "success" : "already exists")
              << " (value: " << it->second << ")\n";
    prices.insert({{"date", 2.49}, {"elderberry", 5.99}});
    std::cout << "   Size: " << prices.size() << "\n\n";

    // --- operator[] ---
    std::cout << "3. operator[]:\n";
    std::cout << "   Price of apple: $" << prices["apple"] << "\n";
    std::cout << "   Price of fig (default): $" << prices["fig"] << " (inserted 0)\n\n";

    // --- Find ---
    std::cout << "4. Find:\n";
    it = prices.find("banana");
    if (it != prices.end())
        std::cout << "   Found: " << it->first << " @ $" << it->second << "\n";
    it = prices.find("grape");
    std::cout << "   Find grape: " << (it == prices.end() ? "not found" : "found") << "\n\n";

    // --- Emplace ---
    std::cout << "5. Emplace:\n";
    prices.emplace("grape", 2.89);
    std::cout << "   Emplaced grape:2.89\n";
    std::cout << "   Size: " << prices.size() << "\n\n";

    // --- Bucket interface ---
    std::cout << "6. Bucket interface:\n";
    std::cout << "   Bucket count: " << prices.bucket_count() << "\n";
    std::cout << "   Load factor: " << prices.load_factor() << "\n\n";

    // --- Custom hash ---
    std::cout << "7. Custom key (City):\n";
    std::unordered_map<City, int, CityHash> populations;
    populations[{"New York", "NY"}] = 8336817;
    populations[{"Los Angeles", "CA"}] = 3979576;
    populations[{"Chicago", "IL"}] = 2693976;

    for (const auto& [city, pop] : populations)
        std::cout << "   " << city.name << ", " << city.state
                  << ": " << pop << "\n\n";

    // --- Word frequency ---
    std::cout << "8. Practical: Word frequency:\n";
    std::string text = "the quick brown fox jumps over the lazy dog the fox";
    std::unordered_map<std::string, int> freq;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word)
        ++freq[word];  // auto-initialized to 0, then increment
    for (const auto& [w, cnt] : freq)
        std::cout << "   " << w << ": " << cnt << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic operations:
   cherry: $3.49
   banana: $0.59
   apple: $1.99

2. Insert:
   Insert apple:2.99 -> already exists (value: 1.99)
   Size: 5

3. operator[]:
   Price of apple: $1.99
   Price of fig (default): $0 (inserted 0)

4. Find:
   Found: banana @ $0.59
   Find grape: not found

5. Emplace:
   Emplaced grape:2.89
   Size: 7

6. Bucket interface:
   Bucket count: 13
   Load factor: 0.538462

7. Custom key (City):
   Chicago, IL: 2693976
   Los Angeles, CA: 3979576
   New York, NY: 8336817

8. Practical: Word frequency:
   the: 3
   jumps: 1
   over: 1
   brown: 1
   lazy: 1
   quick: 1
   fox: 2
   dog: 1
==================================================================
Complexity:
- Average: O(1) insert/find/erase
- Worst-case: O(n) (hash collisions)
==================================================================
Interview Questions:
1. unordered_map vs map?
   -> unordered_map: O(1) avg, no order. map: O(log n), ordered.

2. When would unordered_map degrade to O(n)?
   -> When all keys hash to same bucket (bad hash function).

3. How to provide custom hash?
   -> Define functor with operator()(const T&) returning size_t,
      or specialize std::hash<T>.
==================================================================
Common Mistakes:
- Using operator[] in const contexts (mutates!)
- Not providing operator== for custom key types
- Assuming iteration order is stable (it's not)
- Bad hash function causing performance degradation
- Forgetting to include <functional> for std::hash
==================================================================
*/
