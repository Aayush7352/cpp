/*
==================================================================
170. Custom Comparators
==================================================================
Theory:
- Three ways to provide ordering: lambda, function object,
  function pointer
- Strict weak ordering: must be transitive, irreflexive,
  and asymmetric
- Used by: std::sort, std::set, std::map, std::priority_queue,
  lower_bound, etc.
- Lambda: inline, captures, C++11+
- Function object: stateful, reusable
- Function pointer: traditional, no capture
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <functional>

// ---- 1. Function object (functor) ----
struct ByLength {
    bool operator()(const std::string& a, const std::string& b) const {
        if (a.length() != b.length())
            return a.length() < b.length();
        return a < b; // tie-breaker
    }
};

// ---- 2. Function pointer ----
bool descInt(int a, int b) {
    return a > b;
}

// ---- 3. Complex comparator with state ----
struct ByNthCharacter {
    int n;
    explicit ByNthCharacter(int pos) : n(pos) {}
    bool operator()(const std::string& a, const std::string& b) const {
        char ca = (n < static_cast<int>(a.length())) ? a[n] : 0;
        char cb = (n < static_cast<int>(b.length())) ? b[n] : 0;
        return ca < cb;
    }
};

struct Person {
    std::string name;
    int age;
};

int main() {
    std::cout << "=== Custom Comparators\n\n";

    // --- Lambda comparator for sort ---
    std::cout << "1. Lambda comparator (sort by string length):\n";
    std::vector<std::string> words{"banana", "apple", "kiwi", "strawberry", "fig"};
    std::sort(words.begin(), words.end(),
              [](const std::string& a, const std::string& b) {
                  return a.length() < b.length();
              });
    std::cout << "   ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n\n";

    // --- Function object comparator ---
    std::cout << "2. Function object comparator (ByLength):\n";
    std::set<std::string, ByLength> byLengthSet;
    byLengthSet.insert("banana");
    byLengthSet.insert("apple");
    byLengthSet.insert("kiwi");
    byLengthSet.insert("fig");
    byLengthSet.insert("strawberry");
    for (const auto& w : byLengthSet)
        std::cout << "   " << w << " (len=" << w.length() << ")\n";
    std::cout << "\n";

    // --- Function pointer comparator ---
    std::cout << "3. Function pointer (descending):\n";
    std::vector<int> nums{5, 2, 8, 1, 9};
    std::sort(nums.begin(), nums.end(), descInt);
    std::cout << "   ";
    for (int x : nums) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Stateful comparator (ByNthCharacter) ---
    std::cout << "4. Stateful comparator (by 2nd character):\n";
    std::vector<std::string> names{"Alice", "Bob", "Charlie", "David", "Eve"};
    std::sort(names.begin(), names.end(), ByNthCharacter(1));
    for (const auto& n : names)
        std::cout << "   " << n << " (2nd char: '" << n[1] << "')\n";
    std::cout << "\n";

    // --- Priority queue with custom comparator ---
    std::cout << "5. Priority queue with custom lambda (min-heap by age):\n";
    auto cmp = [](const Person& a, const Person& b) {
        return a.age > b.age; // min-heap
    };
    std::priority_queue<Person, std::vector<Person>, decltype(cmp)> pq(cmp);
    pq.push({"Alice", 30});
    pq.push({"Bob", 25});
    pq.push({"Charlie", 35});
    while (!pq.empty()) {
        const auto& p = pq.top();
        std::cout << "   " << p.name << " (" << p.age << ")\n";
        pq.pop();
    }
    std::cout << "\n";

    // --- Map with custom comparator ---
    std::cout << "6. Map with case-insensitive key:\n";
    auto ciLess = [](const std::string& a, const std::string& b) {
        return std::lexicographical_compare(
            a.begin(), a.end(), b.begin(), b.end(),
            [](char ca, char cb) { return std::tolower(ca) < std::tolower(cb); });
    };
    std::map<std::string, int, decltype(ciLess)> caseInsensitiveMap(ciLess);
    caseInsensitiveMap["Apple"] = 1;
    caseInsensitiveMap["apple"] = 2; // overwrites because keys are equal
    caseInsensitiveMap["BANANA"] = 3;
    for (const auto& [k, v] : caseInsensitiveMap)
        std::cout << "   " << k << " -> " << v << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Lambda comparator (sort by string length):
   fig kiwi apple banana strawberry

2. Function object comparator (ByLength):
   fig (len=3)
   kiwi (len=4)
   apple (len=5)
   banana (len=6)
   strawberry (len=10)

3. Function pointer (descending):
   9 8 5 2 1

4. Stateful comparator (by 2nd character):
   David (2nd char: 'a')
   Charlie (2nd char: 'h')
   Alice (2nd char: 'l')
   Bob (2nd char: 'o')
   Eve (2nd char: 'v')

5. Priority queue with custom lambda (min-heap by age):
   Bob (25)
   Alice (30)
   Charlie (35)

6. Map with case-insensitive key:
   Apple -> 2
   BANANA -> 3
==================================================================
Complexity:
- Custom comparators don't change asymptotic complexity
- Lambda: same as function call (often inlined)
- Function object with state: O(1) per comparison
==================================================================
Interview Questions:
1. Lambda vs function object vs function pointer?
   -> Lambda: concise, captures. Function object: reusable,
      stateful. Function pointer: traditional, no capture,
      can't be inlined easily.

2. What is strict weak ordering?
   -> Irreflexive: !comp(a,a). Transitive: comp(a,b) &&
      comp(b,c) => comp(a,c). Equivalence: !comp(a,b) &&
      !comp(b,a) means a and b are equivalent.

3. Can a lambda be used as comparator for set/map?
   -> Yes, but the type must be provided as template parameter,
      and the lambda instance passed to constructor (C++20 CTAD
      helps, but for set<T, decltype(lambda)> you need instance).
==================================================================
Common Mistakes:
- Comparator not providing strict weak ordering (crashes sort)
- Lambda comparator for associative containers forgetting
  the type parameter in template
- Mutating state in comparator (undefined behavior for set/map)
- Using operator< that's not transitive
- Default capture [&] in lambda comparator (dangling reference)
==================================================================
*/
