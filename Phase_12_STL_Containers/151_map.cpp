/*
==================================================================
151. std::map
==================================================================
Theory:
- Ordered key-value pairs (Red-Black Tree)
- operator[]: inserts if key missing (value-initializes)
- insert: returns pair<iterator,bool> (true if new)
- emplace: constructs in-place, avoids copy
- find: O(log n)
- Used for: dictionary, phone book, symbol table
*/

#include <iostream>
#include <map>
#include <string>
#include <algorithm>

int main() {
    std::cout << "=== std::map\n\n";

    // --- Construction ---
    std::map<std::string, int> ages;
    std::map<int, std::string> ids{{1, "Alice"}, {2, "Bob"}, {3, "Charlie"}};

    std::cout << "1. Initial map:\n";
    for (const auto& [id, name] : ids)
        std::cout << "   " << id << ": " << name << "\n";
    std::cout << "\n";

    // --- Insert ---
    std::cout << "2. Insert:\n";
    auto [it, success] = ages.insert({"Alice", 30});
    std::cout << "   Insert Alice:30 -> " << (success ? "success" : "already exists") << "\n";
    std::tie(it, success) = ages.insert({"Alice", 35});
    std::cout << "   Insert Alice:35 -> " << (success ? "success" : "already exists")
              << " (value remains " << it->second << ")\n";

    ages.insert({{"Bob", 25}, {"Charlie", 35}});
    std::cout << "   Map now:\n";
    for (const auto& [name, age] : ages)
        std::cout << "   " << name << ": " << age << "\n";
    std::cout << "\n";

    // --- operator[] ---
    std::cout << "3. operator[]:\n";
    ages["David"] = 40;         // inserts
    ages["Alice"] = 31;         // modifies existing
    std::cout << "   Alice: " << ages["Alice"] << "\n";
    std::cout << "   Eve (default): " << ages["Eve"] << " (inserted with 0)\n\n";

    // --- Emplace ---
    std::cout << "4. emplace:\n";
    ages.emplace("Frank", 28);
    std::cout << "   Emplace Frank:28\n";
    std::cout << "   Map size: " << ages.size() << "\n\n";

    // --- Find ---
    std::cout << "5. Find:\n";
    it = ages.find("Charlie");
    if (it != ages.end())
        std::cout << "   Found: " << it->first << " -> " << it->second << "\n";
    it = ages.find("Zoo");
    std::cout << "   Find Zoo: " << (it == ages.end() ? "not found" : "found") << "\n\n";

    // --- Erase ---
    std::cout << "6. Erase:\n";
    ages.erase("Eve");
    std::cout << "   Erase Eve. Size: " << ages.size() << "\n\n";

    // --- lower_bound / upper_bound ---
    std::cout << "7. lower_bound/upper_bound:\n";
    std::map<int, std::string> m{{1, "a"}, {3, "c"}, {5, "e"}, {7, "g"}};
    auto lb = m.lower_bound(3);  // first >= 3
    auto ub = m.upper_bound(3);  // first > 3
    std::cout << "   lower_bound(3): " << lb->first << "," << lb->second << "\n";
    std::cout << "   upper_bound(3): " << (ub != m.end() ?
        std::to_string(ub->first) + "," + ub->second : "end") << "\n";

    // --- Iteration in order ---
    std::cout << "\n8. Ordered iteration:\n";
    for (const auto& [key, val] : m)
        std::cout << "   " << key << " -> " << val << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Initial map:
   1: Alice
   2: Bob
   3: Charlie

2. Insert:
   Insert Alice:30 -> success
   Insert Alice:35 -> already exists (value remains 30)
   Map now:
   Alice: 30
   Bob: 25
   Charlie: 35

3. operator[]:
   Alice: 31
   Eve (default): 0 (inserted with 0)

4. emplace:
   Emplace Frank:28
   Map size: 5

5. Find:
   Found: Charlie -> 35
   Find Zoo: not found

6. Erase:
   Erase Eve. Size: 5

7. lower_bound/upper_bound:
   lower_bound(3): 3,c
   upper_bound(3): 5,e

8. Ordered iteration:
   1 -> a
   3 -> c
   5 -> e
   7 -> g
==================================================================
Complexity:
- Insert/Find/Erase: O(log n)
- operator[]: O(log n)
- Iteration: O(n)
==================================================================
Interview Questions:
1. What does operator[] do if key doesn't exist?
   -> Inserts a new element with value-initialized mapped value
      (e.g., 0 for int, empty string) and returns reference to it.

2. Difference between map::insert and operator[]?
   -> insert doesn't overwrite existing; operator[] overwrites.

3. When to use map vs unordered_map?
   -> map: ordered iteration, O(log n). unordered_map:
      average O(1), no order.
==================================================================
Common Mistakes:
- operator[] inadvertently inserting default values
- Using operator[] inside const member functions (not const)
- Forgetting that insert returns pair with bool, not iterator
- Iterating and modifying map (invalidates iterators)
- Using map when hash table would be faster
==================================================================
*/
