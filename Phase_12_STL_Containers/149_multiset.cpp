/*
==================================================================
149. std::multiset
==================================================================
Theory:
- Ordered set allowing duplicate keys
- Insertion always succeeds (no uniqueness enforcement)
- equal_range: returns [begin, end) of all elements with given key
- count: number of elements with given key
- Erase by value removes ALL matching elements
- Used for: multi-set semantics, frequency counting with order
*/

#include <iostream>
#include <set>
#include <string>
#include <algorithm>

int main() {
    std::cout << "=== std::multiset\n\n";

    // --- Construction ---
    std::multiset<int> ms{5, 2, 8, 2, 5, 1, 5, 9};
    std::cout << "1. Multiset content (sorted, with duplicates):\n";
    std::cout << "   ";
    for (int x : ms) std::cout << x << " ";
    std::cout << " (size: " << ms.size() << ")\n\n";

    // --- Insert ---
    std::cout << "2. Insert (always succeeds):\n";
    ms.insert(5);
    ms.insert(2);
    std::cout << "   After inserting 5 and 2: ";
    for (int x : ms) std::cout << x << " ";
    std::cout << " (size: " << ms.size() << ")\n\n";

    // --- Count ---
    std::cout << "3. Count occurrences:\n";
    for (int v : {1, 2, 5, 99})
        std::cout << "   count(" << v << ") = " << ms.count(v) << "\n";
    std::cout << "\n";

    // --- equal_range ---
    std::cout << "4. equal_range:\n";
    auto [lo, hi] = ms.equal_range(5);
    std::cout << "   Elements equal to 5: ";
    for (auto it = lo; it != hi; ++it)
        std::cout << *it << " ";
    std::cout << " (" << std::distance(lo, hi) << " elements)\n\n";

    // --- lower_bound / upper_bound ---
    std::cout << "5. lower_bound / upper_bound for 5:\n";
    auto lb = ms.lower_bound(5);
    auto ub = ms.upper_bound(5);
    std::cout << "   lower_bound(5): " << *lb << "\n";
    std::cout << "   upper_bound(5): " << *ub << "\n";
    std::cout << "   Range [lb, ub): ";
    for (auto it = lb; it != ub; ++it) std::cout << *it << " ";
    std::cout << "\n\n";

    // --- Erase ---
    std::cout << "6. Erase:\n";
    // Erase by value (removes ALL matching)
    int erased = ms.erase(2);
    std::cout << "   Erase 2: removed " << erased << " occurrences\n";
    std::cout << "   After: ";
    for (int x : ms) std::cout << x << " ";

    // Erase by iterator (single element)
    auto it = ms.find(8);
    if (it != ms.end()) {
        ms.erase(it);
        std::cout << "\n   Erase single 8 via iterator: ";
        for (int x : ms) std::cout << x << " ";
    }
    std::cout << "\n\n";

    // --- Practical: Median maintenance ---
    std::cout << "7. Median maintenance with two multisets:\n";
    std::multiset<int> lower, upper;
    auto addAndGetMedian = [&](int x) -> double {
        if (lower.empty() || x <= *lower.rbegin())
            lower.insert(x);
        else
            upper.insert(x);

        // Rebalance
        if (lower.size() > upper.size() + 1) {
            upper.insert(*lower.rbegin());
            lower.erase(std::prev(lower.end()));
        } else if (upper.size() > lower.size()) {
            lower.insert(*upper.begin());
            upper.erase(upper.begin());
        }

        if (lower.size() == upper.size())
            return (*lower.rbegin() + *upper.begin()) / 2.0;
        return *lower.rbegin();
    };

    std::vector<int> stream{1, 5, 2, 8, 3, 9, 4};
    std::cout << "   Stream: ";
    for (int x : stream) {
        double med = addAndGetMedian(x);
        std::cout << x << "(med=" << med << ") ";
    }
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
Multiset content (sorted, with duplicates):
   1 2 2 5 5 5 8 9  (size: 8)

Insert (always succeeds):
   After inserting 5 and 2: 1 2 2 2 5 5 5 5 8 9  (size: 10)

Count occurrences:
   count(1) = 1
   count(2) = 3
   count(5) = 4
   count(99) = 0

equal_range:
   Elements equal to 5: 5 5 5 5  (4 elements)

lower_bound / upper_bound for 5:
   lower_bound(5): 5
   upper_bound(5): 8
   Range [lb, ub): 5 5 5 5

Erase:
   Erase 2: removed 3 occurrences
   After: 1 5 5 5 5 8 9
   Erase single 8 via iterator: 1 5 5 5 5 9

Median maintenance with two multisets:
   Stream: 1(med=1) 5(med=3) 2(med=2) 8(med=3.5) 3(med=3) 9(med=4) 4(med=4)
==================================================================
Complexity:
- Insert: O(log n)
- Find: O(log n)
- Erase(value): O(k + log n) where k = count
- Erase(iterator): O(1) amortized
- equal_range: O(log n)
==================================================================
Interview Questions:
1. multiset vs set + count vector?
   -> multiset is self-balancing with O(log n) operations.
      Manual count vector is O(1) but no order.

2. How to erase a single element from multiset?
   -> Use iterator from find: ms.erase(ms.find(value));
      Erase(value) removes all occurrences.

3. When to use multiset?
   -> When duplicates matter and order is needed.
      e.g., median maintenance, scheduling with priorities.
==================================================================
Common Mistakes:
- Assuming insert returns pair (set behavior) — multiset insert
  returns iterator only (always succeeds)
- Erase(value) removing all duplicates unintentionally
- Using operator[] (not available for set/multiset)
- Expecting O(1) count — it's O(k + log n) or O(log n + count)
==================================================================
*/
