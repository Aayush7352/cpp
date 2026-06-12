/*
==================================================================
143. std::list
==================================================================
Theory:
- Doubly-linked list: O(1) insert/erase at any position (if
  iterator known)
- splice: transfer elements between lists without copying
- merge: merge two sorted lists
- unique: remove consecutive duplicates
- remove_if: remove elements matching predicate
- sort: list-specific merge sort (O(n log n), stable)
- No random access (no operator[])
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <cctype>

bool isOdd(int x) { return x % 2 != 0; }

int main() {
    std::cout << "=== std::list\n\n";

    // --- Construction ---
    std::list<int> l1{1, 2, 3, 4, 5};
    std::list<int> l2{10, 20, 30};

    std::cout << "l1: "; for (int x : l1) std::cout << x << " ";
    std::cout << "\nl2: "; for (int x : l2) std::cout << x << " ";
    std::cout << "\n\n";

    // --- splice ---
    std::cout << "1. splice:\n";
    auto it = l1.begin();
    std::advance(it, 2); // point to 3
    l1.splice(it, l2);   // move all of l2 before position 3
    std::cout << "   l1 after splice: ";
    for (int x : l1) std::cout << x << " ";
    std::cout << " (l2 now empty)\n\n";

    // --- sort ---
    std::cout << "2. sort:\n";
    std::list<int> unsorted{9, 2, 7, 1, 5, 3};
    unsorted.sort(); // list::sort (not std::sort)
    std::cout << "   Sorted: ";
    for (int x : unsorted) std::cout << x << " ";
    std::cout << "\n\n";

    // --- merge ---
    std::cout << "3. merge:\n";
    std::list<int> a{1, 3, 5};
    std::list<int> b{2, 4, 6};
    a.merge(b); // both must be sorted; b becomes empty
    std::cout << "   Merged: ";
    for (int x : a) std::cout << x << " ";
    std::cout << "\n\n";

    // --- unique ---
    std::cout << "4. unique:\n";
    std::list<int> dup{1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
    dup.unique(); // removes consecutive duplicates only
    std::cout << "   After unique: ";
    for (int x : dup) std::cout << x << " ";
    std::cout << "\n\n";

    // --- remove_if ---
    std::cout << "5. remove_if:\n";
    std::list<int> nums{1, 2, 3, 4, 5, 6, 7, 8};
    nums.remove_if(isOdd);
    std::cout << "   After removing odds: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << "\n\n";

    // --- remove (value) ---
    std::cout << "6. remove(value):\n";
    std::list<int> vals{1, 2, 3, 2, 4, 2, 5};
    vals.remove(2);
    std::cout << "   After remove(2): ";
    for (int x : vals) std::cout << x << " ";
    std::cout << "\n\n";

    // --- reverse ---
    std::cout << "7. reverse:\n";
    std::list<int> rev{1, 2, 3, 4, 5};
    rev.reverse();
    std::cout << "   Reversed: ";
    for (int x : rev) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
l1: 1 2 3 4 5
l2: 10 20 30

1. splice:
   l1 after splice: 1 2 10 20 30 3 4 5  (l2 now empty)

2. sort:
   Sorted: 1 2 3 5 7 9

3. merge:
   Merged: 1 2 3 4 5 6

4. unique:
   After unique: 1 2 3 4 5

5. remove_if:
   After removing odds: 2 4 6 8

6. remove(value):
   After remove(2): 1 3 4 5

7. reverse:
   Reversed: 5 4 3 2 1
==================================================================
Complexity:
- splice: O(1) (iterator transfer)
- merge: O(n) comparisons
- sort: O(n log n)
- unique: O(n)
- remove_if: O(n)
==================================================================
Interview Questions:
1. When to use list vs vector?
   -> list: frequent insert/erase in middle, no random access
      needed. vector: cache-friendly, random access, mostly
      push_back/pop_back.

2. Why does list have its own sort?
   -> std::sort requires random-access iterators. list provides
      only bidirectional iterators. Its member sort uses merge sort.

3. What's splice?
   -> Transfers elements from one list to another without copying.
      O(1) time. Ownership transfer, not element copy.
==================================================================
Common Mistakes:
- Using std::sort on list (compiler error)
- Expecting O(1) random access
- unique on unsorted list (only removes consecutive duplicates)
- merge without sorting both lists first
- Assuming size() is O(1) in list (C++11 guarantees O(1))
==================================================================
*/
