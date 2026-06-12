/*
==================================================================
159. std::lower_bound
==================================================================
Theory:
- Returns iterator to first element >= value
- Binary search on SORTED range
- O(log n) time complexity
- Uses operator< (or custom comparator)
- If all elements < value, returns end()
- Used for: insertion position, range queries
- Foundation for std::set::lower_bound, map::lower_bound
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

int main() {
    std::cout << "=== std::lower_bound\n\n";

    // --- Basic ---
    std::cout << "1. Basic lower_bound:\n";
    std::vector<int> v{1, 3, 5, 7, 9, 11, 13};
    std::cout << "   Data: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    auto it = std::lower_bound(v.begin(), v.end(), 6);
    std::cout << "   lower_bound(6): " << *it
              << " (at index " << (it - v.begin()) << ")\n";

    it = std::lower_bound(v.begin(), v.end(), 7);
    std::cout << "   lower_bound(7): " << *it
              << " (at index " << (it - v.begin()) << ")\n";

    it = std::lower_bound(v.begin(), v.end(), 0);
    std::cout << "   lower_bound(0): " << *it
              << " (at index " << (it - v.begin()) << ")\n";

    it = std::lower_bound(v.begin(), v.end(), 99);
    std::cout << "   lower_bound(99): "
              << (it == v.end() ? "end()" : std::to_string(*it)) << "\n\n";

    // --- With duplicates ---
    std::cout << "2. lower_bound with duplicates:\n";
    std::vector<int> dup{1, 2, 2, 2, 3, 4, 5};
    std::cout << "   Data: ";
    for (int x : dup) std::cout << x << " ";
    std::cout << "\n";

    auto lb = std::lower_bound(dup.begin(), dup.end(), 2);
    auto ub = std::upper_bound(dup.begin(), dup.end(), 2);
    std::cout << "   lower_bound(2): index " << (lb - dup.begin()) << "\n";
    std::cout << "   upper_bound(2): index " << (ub - dup.begin()) << "\n";
    std::cout << "   Range of 2: [" << (lb - dup.begin()) << ", "
              << (ub - dup.begin()) << ")\n";
    std::cout << "   Elements equal to 2: ";
    for (auto i = lb; i != ub; ++i) std::cout << *i << " ";
    std::cout << "\n\n";

    // --- Custom comparator ---
    std::cout << "3. lower_bound with custom comparator:\n";
    std::vector<int> desc{13, 11, 9, 7, 5, 3, 1};
    it = std::lower_bound(desc.begin(), desc.end(), 7, std::greater<int>());
    std::cout << "   Descending data: ";
    for (int x : desc) std::cout << x << " ";
    std::cout << "\n   lower_bound(7) with greater: " << *it
              << " (index " << (it - desc.begin()) << ")\n\n";

    // --- Practical: Insertion position ---
    std::cout << "4. Practical: maintain sorted insertion:\n";
    std::vector<int> sorted{10, 20, 30, 40};
    int newVal = 25;
    auto pos = std::lower_bound(sorted.begin(), sorted.end(), newVal);
    sorted.insert(pos, newVal);
    std::cout << "   Insert " << newVal << ": ";
    for (int x : sorted) std::cout << x << " ";
    std::cout << "\n\n";

    // --- With strings ---
    std::cout << "5. lower_bound with strings:\n";
    std::vector<std::string> words{"apple", "banana", "grape", "orange"};
    auto sit = std::lower_bound(words.begin(), words.end(), "cherry");
    if (sit != words.end())
        std::cout << "   lower_bound(\"cherry\"): \"" << *sit << "\"\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic lower_bound:
   Data: 1 3 5 7 9 11 13
   lower_bound(6): 7 (at index 3)
   lower_bound(7): 7 (at index 3)
   lower_bound(0): 1 (at index 0)
   lower_bound(99): end()

2. lower_bound with duplicates:
   Data: 1 2 2 2 3 4 5
   lower_bound(2): index 1
   upper_bound(2): index 4
   Range of 2: [1, 4)
   Elements equal to 2: 2 2 2

3. lower_bound with custom comparator:
   Descending data: 13 11 9 7 5 3 1
   lower_bound(7) with greater: 7 (index 3)

4. Practical: maintain sorted insertion:
   Insert 25: 10 20 25 30 40

5. lower_bound with strings:
   lower_bound("cherry"): "grape"
==================================================================
Complexity:
- Time: O(log n)
- Space: O(1)
==================================================================
Interview Questions:
1. lower_bound vs binary_search?
   -> lower_bound returns iterator (position), binary_search
      returns bool. Use lower_bound when you need the position.

2. lower_bound vs upper_bound?
   -> lower_bound: first >= val. upper_bound: first > val.
      Together they define the range of elements == val.

3. What if element not found?
   -> Returns iterator to first element greater than val,
      or end() if all elements < val.
==================================================================
Common Mistakes:
- Using lower_bound on unsorted range
- Forgetting to check if result == end() before dereferencing
- Using wrong comparator (inconsistent with sort order)
- Using lower_bound where upper_bound is needed
- Returning index without checking end()
==================================================================
*/
