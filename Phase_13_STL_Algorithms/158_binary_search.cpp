/*
==================================================================
158. std::binary_search
==================================================================
Theory:
- Performs binary search on SORTED range
- Returns bool: true if element found
- O(log n) time complexity
- Uses operator< (or custom comparator) for comparison
- Range must be partitioned by comparator (sorted ascending)
- If multiple elements match, returns true (first match found)
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>

int main() {
    std::cout << "=== std::binary_search\n\n";

    // --- Basic ---
    std::cout << "1. Basic binary_search:\n";
    std::vector<int> v{1, 3, 5, 7, 9, 11, 13};
    std::cout << "   Data: ";
    for (int x : v) std::cout << x << " ";

    for (int target : {5, 6, 7, 99}) {
        bool found = std::binary_search(v.begin(), v.end(), target);
        std::cout << "\n   Search " << target << ": "
                  << (found ? "found" : "not found");
    }
    std::cout << "\n\n";

    // --- Custom comparator (descending) ---
    std::cout << "2. binary_search with custom comparator:\n";
    std::vector<int> desc{13, 11, 9, 7, 5, 3, 1};
    // Must pass the same comparator as used for sorting
    bool found = std::binary_search(desc.begin(), desc.end(), 7,
                                    std::greater<int>());
    std::cout << "   Search 7 in descending arr: "
              << (found ? "found" : "not found") << "\n";

    // Not found if using wrong comparator!
    found = std::binary_search(desc.begin(), desc.end(), 7);
    std::cout << "   Same search without comparator: "
              << (found ? "found" : "not found") << " (wrong!)\n\n";

    // --- With strings ---
    std::cout << "3. binary_search on strings:\n";
    std::vector<std::string> words{"apple", "banana", "cherry", "date", "fig"};
    for (const auto& target : {"banana", "grape", "date"}) {
        bool f = std::binary_search(words.begin(), words.end(), target);
        std::cout << "   \"" << target << "\": "
                  << (f ? "found" : "not found") << "\n";
    }
    std::cout << "\n";

    // --- Using lower_bound equivalence ---
    std::cout << "4. Manual binary_search using lower_bound:\n";
    auto myBinarySearch = [](auto begin, auto end, const auto& val) {
        auto it = std::lower_bound(begin, end, val);
        return it != end && *it == val;
    };
    std::cout << "   Search 9: " << myBinarySearch(v.begin(), v.end(), 9) << "\n";
    std::cout << "   Search 8: " << myBinarySearch(v.begin(), v.end(), 8) << "\n";

    // --- Partitioned range error ---
    std::cout << "\n5. IMPORTANT: Range must be sorted!\n";
    std::vector<int> unsorted{5, 1, 9, 3, 7};
    bool unreliable = std::binary_search(unsorted.begin(), unsorted.end(), 9);
    std::cout << "   binary_search(9) on unsorted: "
              << (unreliable ? "found (lucky)" : "not found")
              << " (undefined behavior!)\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic binary_search:
   Data: 1 3 5 7 9 11 13
   Search 5: found
   Search 6: not found
   Search 7: found
   Search 99: not found

2. binary_search with custom comparator:
   Search 7 in descending arr: found
   Same search without comparator: not found (wrong!)

3. binary_search on strings:
   "banana": found
   "grape": not found
   "date": found

4. Manual binary_search using lower_bound:
   Search 9: 1
   Search 8: 0

5. IMPORTANT: Range must be sorted!
   binary_search(9) on unsorted: found (lucky) (undefined behavior!)
==================================================================
Complexity:
- Time: O(log n)
- Space: O(1)
==================================================================
Interview Questions:
1. Does binary_search return iterator or bool?
   -> Bool (true/false). Use lower_bound/equal_range if
      iterator is needed.

2. Can we use binary_search on unsorted range?
   -> No — result is undefined. Must sort first.

3. How to find position of found element?
   -> Use std::lower_bound + compare to value,
      or std::equal_range.
==================================================================
Common Mistakes:
- Using binary_search on unsorted range
- Using wrong comparator (different from sort order)
- Expecting iterator result (it returns bool)
- Using binary_search for elements with non-strict-weak-ordering
- Not #including <algorithm>
==================================================================
*/
