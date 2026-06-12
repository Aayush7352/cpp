/*
==================================================================
154. std::pair
==================================================================
Theory:
- Template struct holding two heterogeneous values
- make_pair (C++03) / pair CTAD (C++17)
- Structured binding (C++17): auto [a, b] = pair
- Comparison: lexicographic (first, then second)
- Used as element type for map, unordered_map
- pair<T1,T2> is copyable if T1,T2 are copyable
*/

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

int main() {
    std::cout << "=== std::pair\n\n";

    // --- Construction ---
    std::cout << "1. Construction:\n";
    std::pair<int, std::string> p1(1, "one");
    auto p2 = std::make_pair(2, "two");  // C++03
    std::pair p3{3, "three"};             // CTAD (C++17)
    std::pair p4 = p1;                    // copy

    std::cout << "   p1: (" << p1.first << ", " << p1.second << ")\n";
    std::cout << "   p2: (" << p2.first << ", " << p2.second << ")\n";
    std::cout << "   p3: (" << p3.first << ", " << p3.second << ")\n\n";

    // --- Structured binding ---
    std::cout << "2. Structured binding (C++17):\n";
    auto [id, name] = p1;
    std::cout << "   id=" << id << ", name=" << name << "\n\n";

    // --- make_pair with type deduction ---
    std::cout << "3. make_pair usage:\n";
    auto pr = std::make_pair(3.14, 42);
    std::cout << "   make_pair(3.14, 42): (" << pr.first << ", "
              << pr.second << ")\n\n";

    // --- Comparison ---
    std::cout << "4. Comparison (lexicographic):\n";
    std::pair<int,int> a{1, 2};
    std::pair<int,int> b{1, 3};
    std::pair<int,int> c{2, 1};
    std::cout << "   (1,2) == (1,3): " << (a == b) << "\n";
    std::cout << "   (1,2) <  (1,3): " << (a < b) << "\n";
    std::cout << "   (1,2) <  (2,1): " << (a < c) << "\n";
    std::cout << "   (1,3) >  (1,2): " << (b > a) << "\n\n";

    // --- In containers ---
    std::cout << "5. Vector of pairs:\n";
    std::vector<std::pair<std::string, int>> vec{
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}
    };
    std::sort(vec.begin(), vec.end());
    std::cout << "   Sorted by name then age:\n";
    for (const auto& [n, a] : vec)
        std::cout << "   " << n << ": " << a << "\n";

    std::cout << "\n6. Map iteration with pair:\n";
    std::map<int, std::string> m{{1, "a"}, {2, "b"}};
    for (const auto& kv : m)  // kv is pair<const int, string>
        std::cout << "   " << kv.first << ": " << kv.second << "\n";

    // --- Returning multiple values ---
    std::cout << "\n7. Return multiple values:\n";
    auto minMax = [](const std::vector<int>& v) {
        auto it = std::minmax_element(v.begin(), v.end());
        return std::pair(*it.first, *it.second);
    };
    std::vector<int> data{5, 2, 8, 1, 9};
    auto [min, max] = minMax(data);
    std::cout << "   Data: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n   Min: " << min << ", Max: " << max << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Construction:
   p1: (1, one)
   p2: (2, two)
   p3: (3, three)

2. Structured binding (C++17):
   id=1, name=one

3. make_pair usage:
   make_pair(3.14, 42): (3.14, 42)

4. Comparison (lexicographic):
   (1,2) == (1,3): 0
   (1,2) <  (1,3): 1
   (1,2) <  (2,1): 1
   (1,3) >  (1,2): 1

5. Vector of pairs:
   Sorted by name then age:
   Alice: 30
   Bob: 25
   Charlie: 35

6. Map iteration with pair:
   1: a
   2: b

7. Return multiple values:
   Data: 5 2 8 1 9
   Min: 1, Max: 9
==================================================================
Complexity:
- pair operations: O(1)
- Comparison: O(1) for fundamental types, O(k) for strings
==================================================================
Interview Questions:
1. structured binding vs std::tie?
   -> Structured binding: auto [a,b] = p; (C++17, preferred).
      tie: int a; int b; std::tie(a,b) = p; (C++11, requires
      variables already declared).

2. When to use pair vs tuple?
   -> pair: exactly two values. tuple: two or more.

3. Why is pair comparison lexicographic?
   -> Useful for ordering in sorted containers; first element
      primary key.
==================================================================
Common Mistakes:
- Forgetting .first/.second are public members, not methods
- Using pair when tuple or custom struct is more readable
- Assuming pair<int,int>{1,2} == {1,2} (needs explicit type)
- Modifying pair key in map (first is const)
==================================================================
*/
