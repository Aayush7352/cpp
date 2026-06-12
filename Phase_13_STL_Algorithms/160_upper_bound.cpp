/*
==================================================================
160. std::upper_bound
==================================================================
Theory:
- Returns iterator to first element > value
- Binary search on SORTED range
- O(log n) time complexity
- Complement to lower_bound: [lower, upper) = elements == value
- Use with lower_bound to find range of equal elements
- If all elements <= value, returns end()
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

int main() {
    std::cout << "=== std::upper_bound\n\n";

    // --- Basic ---
    std::cout << "1. Basic upper_bound:\n";
    std::vector<int> v{1, 3, 5, 7, 9, 11, 13};
    std::cout << "   Data: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    auto it = std::upper_bound(v.begin(), v.end(), 6);
    std::cout << "   upper_bound(6): " << *it
              << " (index " << (it - v.begin()) << ")\n";

    it = std::upper_bound(v.begin(), v.end(), 7);
    std::cout << "   upper_bound(7): " << *it
              << " (index " << (it - v.begin()) << ")\n";

    it = std::upper_bound(v.begin(), v.end(), 13);
    std::cout << "   upper_bound(13): "
              << (it == v.end() ? "end()" : std::to_string(*it)) << "\n\n";

    // --- With duplicates (paired with lower_bound) ---
    std::cout << "2. upper_bound with duplicates (range query):\n";
    std::vector<int> dup{1, 2, 2, 2, 3, 4, 5};
    int target = 2;

    auto lb = std::lower_bound(dup.begin(), dup.end(), target);
    auto ub = std::upper_bound(dup.begin(), dup.end(), target);
    std::cout << "   Data: ";
    for (int x : dup) std::cout << x << " ";
    std::cout << "\n   Target value: " << target << "\n";
    std::cout << "   lower_bound: index " << (lb - dup.begin()) << " (value " << *lb << ")\n";
    std::cout << "   upper_bound: index " << (ub - dup.begin());
    if (ub != dup.end())
        std::cout << " (value " << *ub << ")\n";
    else
        std::cout << " (end)\n";
    std::cout << "   Count of " << target << ": " << (ub - lb) << "\n";
    std::cout << "   Elements: ";
    for (auto i = lb; i != ub; ++i) std::cout << *i << " ";
    std::cout << "\n\n";

    // --- Custom comparator ---
    std::cout << "3. upper_bound with custom comparator (descending):\n";
    std::vector<int> desc{13, 11, 9, 7, 5, 3, 1};
    it = std::upper_bound(desc.begin(), desc.end(), 7, std::greater<int>());
    if (it != desc.end())
        std::cout << "   upper_bound(7) with greater: " << *it
                  << " (index " << (it - desc.begin()) << ")\n";
    else
        std::cout << "   upper_bound(7) with greater: end()\n\n";

    // --- Practical: Find insertion point for new element ---
    std::cout << "4. Practical: Find count of elements <= value:\n";
    std::vector<int> scores{45, 60, 72, 85, 85, 90, 95};
    int passing = 85;
    auto passIt = std::upper_bound(scores.begin(), scores.end(), passing);
    int passedCount = passIt - scores.begin();
    std::cout << "   Scores: ";
    for (int s : scores) std::cout << s << " ";
    std::cout << "\n   Students with score <= " << passing << ": "
              << passedCount << "\n";

    // --- With strings ---
    std::cout << "\n5. upper_bound with strings:\n";
    std::vector<std::string> words{"apple", "banana", "cherry", "date"};
    auto sit = std::upper_bound(words.begin(), words.end(), "cherry");
    if (sit != words.end())
        std::cout << "   upper_bound(\"cherry\"): \"" << *sit << "\"\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic upper_bound:
   Data: 1 3 5 7 9 11 13
   upper_bound(6): 7 (index 3)
   upper_bound(7): 9 (index 4)
   upper_bound(13): end()

2. upper_bound with duplicates (range query):
   Data: 1 2 2 2 3 4 5
   Target value: 2
   lower_bound: index 1 (value 2)
   upper_bound: index 4 (value 3)
   Count of 2: 3
   Elements: 2 2 2

3. upper_bound with custom comparator (descending):
   upper_bound(7) with greater: 5 (index 4)

4. Practical: Find count of elements <= value:
   Scores: 45 60 72 85 85 90 95
   Students with score <= 85: 5

5. upper_bound with strings:
   upper_bound("cherry"): "date"
==================================================================
Complexity:
- Time: O(log n)
- Space: O(1)
==================================================================
Interview Questions:
1. lower_bound vs upper_bound in one sentence?
   -> lower_bound: first position where value can be inserted
      without breaking order. upper_bound: last such position + 1.

2. How to count occurrences with bounds?
   -> count = upper_bound - lower_bound for random-access
      iterators, or std::distance(lower, upper).

3. What if all elements are > value?
   -> Both lower_bound and upper_bound return begin().
==================================================================
Common Mistakes:
- Confusing upper_bound with lower_bound
- Using upper_bound where lower_bound is needed
- Dereferencing end() from upper_bound
- Using on unsorted range
- Wrong comparator inconsistent with sort order
==================================================================
*/
