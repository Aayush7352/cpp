/*
==================================================================
148. std::set
==================================================================
Theory:
- Ordered set (usually Red-Black Tree)
- Unique keys in sorted order (default <)
- Insert, find, erase: O(log n)
- lower_bound: first element >= value
- upper_bound: first element > value
- equal_range: pair of lower_bound, upper_bound
- Iteration is in-order (ascending)
*/

#include <iostream>
#include <set>
#include <string>
#include <algorithm>

int main() {
    std::cout << "=== std::set\n\n";

    // --- Construction ---
    std::set<int> empty;                            // default
    std::set<int> s{5, 2, 8, 1, 9, 2, 5};          // duplicates ignored
    std::cout << "1. Set content (sorted, unique): ";
    for (int x : s) std::cout << x << " ";
    std::cout << " (size: " << s.size() << ")\n\n";

    // --- Insert ---
    std::cout << "2. Insert:\n";
    auto [it, inserted] = s.insert(3);
    std::cout << "   Insert 3: " << (inserted ? "success" : "already exists") << "\n";
    std::tie(it, inserted) = s.insert(3);  // second attempt
    std::cout << "   Insert 3 again: " << (inserted ? "success" : "already exists") << "\n";

    s.insert({10, 0, 7}); // initializer list
    std::cout << "   After insert {10,0,7}: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Find ---
    std::cout << "3. Find:\n";
    it = s.find(5);
    if (it != s.end())
        std::cout << "   Found: " << *it << "\n";
    else
        std::cout << "   5 not found\n";

    it = s.find(99);
    std::cout << "   Find 99: " << (it == s.end() ? "not found" : "found") << "\n\n";

    // --- lower_bound / upper_bound ---
    std::cout << "4. lower_bound / upper_bound:\n";
    auto lb = s.lower_bound(5);  // first >= 5
    auto ub = s.upper_bound(5);  // first > 5
    std::cout << "   Set: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n   lower_bound(5): " << *lb;
    std::cout << "\n   upper_bound(5): " << *ub << "\n\n";

    // --- equal_range ---
    std::cout << "5. equal_range:\n";
    auto [lo, hi] = s.equal_range(5);
    std::cout << "   equal_range(5): [";
    for (auto i = lo; i != hi; ++i) std::cout << *i << " ";
    std::cout << ")\n\n";

    // --- Erase ---
    std::cout << "6. Erase:\n";
    s.erase(5);
    std::cout << "   Erase 5: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";

    it = s.find(8);
    s.erase(it); // erase by iterator
    std::cout << "   Erase via iterator to 8: ";
    for (int x : s) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Custom ordering (descending) ---
    std::cout << "7. Descending set:\n";
    std::set<int, std::greater<int>> desc{3, 1, 4, 1, 5, 9};
    for (int x : desc) std::cout << x << " ";
    std::cout << " (largest first)\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Set content (sorted, unique): 1 2 5 8 9  (size: 5)

2. Insert:
   Insert 3: success
   Insert 3 again: already exists
   After insert {10,0,7}: 0 1 2 3 5 7 8 9 10

3. Find:
   Found: 5
   Find 99: not found

4. lower_bound / upper_bound:
   Set: 0 1 2 3 5 7 8 9 10
   lower_bound(5): 5
   upper_bound(5): 7

5. equal_range(5): [5 )

6. Erase:
   Erase 5: 0 1 2 3 7 8 9 10
   Erase via iterator to 8: 0 1 2 3 7 9 10

7. Descending set:
   9 5 4 3 1
==================================================================
Complexity:
- Insert: O(log n)
- Find: O(log n)
- Erase: O(log n)
- lower_bound/upper_bound: O(log n)
- Iteration: O(n)
==================================================================
Interview Questions:
1. Underlying data structure of std::set?
   -> Typically Red-Black Tree (balanced BST). Guarantees
      O(log n) for insert/find/erase.

2. Difference between set and unordered_set?
   -> set: ordered, O(log n), RB tree.
      unordered_set: hash-based, average O(1), no order.

3. When does a set insertion return false?
   -> When the key already exists. Returns pair<iterator, bool>.
==================================================================
Common Mistakes:
- Inserting duplicate — second insertion is silently ignored
- Dereferencing end() iterator from find
- Assuming set elements can be modified (they are const)
- Using set when hash table (unordered_set) would be faster
==================================================================
*/
