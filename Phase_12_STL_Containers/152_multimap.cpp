/*
==================================================================
152. std::multimap
==================================================================
Theory:
- Ordered key-value pairs allowing duplicate keys
- No operator[] (ambiguous with duplicate keys)
- equal_range: get all values for a key
- Insert always succeeds
- Used for: one-to-many relationships, grouping
*/

#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>

int main() {
    std::cout << "=== std::multimap\n\n";

    // --- Construction ---
    std::multimap<std::string, int> scores;
    std::multimap<int, std::string> students{{3, "Alice"}, {1, "Bob"},
                                             {3, "Charlie"}, {2, "David"},
                                             {3, "Eve"}};

    std::cout << "1. Initial multimap:\n";
    for (const auto& [grade, name] : students)
        std::cout << "   Grade " << grade << ": " << name << "\n";
    std::cout << "\n";

    // --- Insert (always succeeds) ---
    std::cout << "2. Insert:\n";
    scores.insert({"Alice", 95});
    scores.insert({"Bob", 87});
    scores.insert({"Alice", 92});  // duplicate key
    scores.insert({"Alice", 98});
    scores.insert({"Bob", 90});
    std::cout << "   scores size: " << scores.size() << "\n";
    for (const auto& [name, score] : scores)
        std::cout << "   " << name << ": " << score << "\n";
    std::cout << "\n";

    // --- Count ---
    std::cout << "3. Count:\n";
    std::cout << "   Alice has " << scores.count("Alice") << " scores\n";
    std::cout << "   Bob has " << scores.count("Bob") << " scores\n";
    std::cout << "   Eve has " << scores.count("Eve") << " scores\n\n";

    // --- Find (first occurrence) ---
    std::cout << "4. Find:\n";
    auto it = scores.find("Alice");
    if (it != scores.end())
        std::cout << "   First Alice entry: " << it->first << " -> " << it->second << "\n\n";

    // --- equal_range ---
    std::cout << "5. equal_range (all scores for Alice):\n";
    auto [lo, hi] = scores.equal_range("Alice");
    int sum = 0, cnt = 0;
    for (auto i = lo; i != hi; ++i) {
        std::cout << "   " << i->second << "\n";
        sum += i->second;
        ++cnt;
    }
    std::cout << "   Avg: " << (cnt > 0 ? sum / cnt : 0.0) << "\n\n";

    // --- Erase ---
    std::cout << "6. Erase:\n";
    scores.erase("Bob"); // removes all Bob
    std::cout << "   After erasing Bob: size = " << scores.size() << "\n";
    // Erase single element
    it = scores.find("Alice");
    if (it != scores.end()) {
        scores.erase(it);
        std::cout << "   Erased one Alice entry. Size: " << scores.size() << "\n";
    }
    for (const auto& [name, score] : scores)
        std::cout << "   " << name << ": " << score << "\n";
    std::cout << "\n";

    // --- Practical: Group by ---
    std::cout << "7. Grouping students by grade:\n";
    std::multimap<int, std::string> gradeMap;
    std::vector<std::pair<std::string, int>> roster{
        {"Alice", 3}, {"Bob", 1}, {"Charlie", 2},
        {"David", 3}, {"Eve", 1}, {"Frank", 2}
    };
    for (const auto& [name, g] : roster)
        gradeMap.insert({g, name});

    for (int g = 1; g <= 3; ++g) {
        auto [start, end] = gradeMap.equal_range(g);
        std::cout << "   Grade " << g << ": ";
        for (auto i = start; i != end; ++i)
            std::cout << i->second << " ";
        std::cout << "\n";
    }

    return 0;
}

/*
==================================================================
Sample I/O:
1. Initial multimap:
   Grade 1: Bob
   Grade 2: David
   Grade 3: Alice
   Grade 3: Charlie
   Grade 3: Eve

2. Insert:
   scores size: 5
   Alice: 95
   Alice: 92
   Alice: 98
   Bob: 87
   Bob: 90

3. Count:
   Alice has 3 scores
   Bob has 2 scores
   Eve has 0 scores

4. Find:
   First Alice entry: Alice -> 95

5. equal_range (all scores for Alice):
   95
   92
   98
   Avg: 95

6. Erase:
   After erasing Bob: size = 3
   Erased one Alice entry. Size: 2
   Alice: 92
   Alice: 98

7. Grouping students by grade:
   Grade 1: Bob Eve
   Grade 2: Charlie Frank
   Grade 3: Alice David
==================================================================
Complexity:
- Insert: O(log n)
- Find: O(log n)
- equal_range: O(log n)
- Erase(key): O(k + log n)
- Erase(iterator): O(1) amortized
==================================================================
Interview Questions:
1. Why no operator[] in multimap?
   -> Ambiguous — which value to return when multiple keys exist?

2. How to insert/emplace in multimap?
   -> insert({key, value}) or emplace(key, value). No pair<bool>
      returned (always succeeds).

3. How to iterate over all values for a key?
   -> Use equal_range() or lower_bound() + upper_bound().
==================================================================
Common Mistakes:
- Trying to use operator[] on multimap
- Assuming insert returns pair<iterator,bool>
- Erase(key) removing all entries unintentionally
- Forgetting equal_range returns pair of iterators
==================================================================
*/
