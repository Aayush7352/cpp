/*
==================================================================
144. std::forward_list
==================================================================
Theory:
- Singly-linked list: minimal overhead (one pointer per node)
- No size() member (O(1) not possible without counting)
- Insert_after / erase_after (not insert/erase at arbitrary pos)
- before_begin(): iterator before first element (for insert_after)
- forward_list is optimized for minimal memory
- Useful for hash table chains, adjacency lists
*/

#include <iostream>
#include <forward_list>
#include <algorithm>

bool isEven(int x) { return x % 2 == 0; }

int main() {
    std::cout << "=== std::forward_list\n\n";

    // --- Construction ---
    std::forward_list<int> fl1;                          // empty
    std::forward_list<int> fl2(5, 42);                   // 5 × 42
    std::forward_list<int> fl3{1, 2, 3, 4, 5};          // init list

    std::cout << "fl3: "; for (int x : fl3) std::cout << x << " ";
    std::cout << "\n\n";

    // --- push_front / pop_front ---
    std::cout << "1. push_front/pop_front:\n";
    std::forward_list<int> fl;
    fl.push_front(30);
    fl.push_front(20);
    fl.push_front(10);
    std::cout << "   After pushes: ";
    for (int x : fl) std::cout << x << " ";

    fl.pop_front();
    std::cout << "\n   After pop_front: ";
    for (int x : fl) std::cout << x << " ";
    std::cout << "\n\n";

    // --- insert_after ---
    std::cout << "2. insert_after:\n";
    std::forward_list<int> fl4{1, 2, 3};
    auto it = fl4.before_begin();     // before first element
    fl4.insert_after(it, 0);          // insert 0 at front
    it = fl4.begin();
    std::advance(it, 2);              // point to element after 0,1,2 → 3
    fl4.insert_after(it, 99);         // after 3rd element
    std::cout << "   After inserts: ";
    for (int x : fl4) std::cout << x << " ";
    std::cout << "\n\n";

    // --- erase_after ---
    std::cout << "3. erase_after:\n";
    fl4.erase_after(fl4.before_begin()); // erase first element
    std::cout << "   After erase_after(before_begin): ";
    for (int x : fl4) std::cout << x << " ";
    std::cout << "\n\n";

    // --- unique, merge, sort, reverse (same as list) ---
    std::cout << "4. sort + unique:\n";
    std::forward_list<int> fl5{5, 1, 3, 1, 2, 3, 4};
    fl5.sort();
    std::cout << "   Sorted: ";
    for (int x : fl5) std::cout << x << " ";
    fl5.unique();
    std::cout << "\n   After unique: ";
    for (int x : fl5) std::cout << x << " ";
    std::cout << "\n\n";

    // --- remove_if ---
    std::cout << "5. remove_if:\n";
    std::forward_list<int> fl6{1, 2, 3, 4, 5, 6};
    fl6.remove_if(isEven);
    std::cout << "   After remove_if (even): ";
    for (int x : fl6) std::cout << x << " ";
    std::cout << "\n\n";

    // --- before_begin usage ---
    std::cout << "6. before_begin trick:\n";
    std::forward_list<int> fl7{2, 3, 4};
    // Insert at front without special case
    fl7.insert_after(fl7.before_begin(), 1);
    std::cout << "   Inserted 1 at front via before_begin: ";
    for (int x : fl7) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
fl3: 1 2 3 4 5

1. push_front/pop_front:
   After pushes: 10 20 30
   After pop_front: 20 30

2. insert_after:
   After inserts: 0 1 2 99 3

3. erase_after:
   After erase_after(before_begin): 1 2 99 3

4. sort + unique:
   Sorted: 1 1 2 3 3 4 5
   After unique: 1 2 3 4 5

5. remove_if:
   After remove_if (even): 1 3 5

6. before_begin trick:
   Inserted 1 at front via before_begin: 1 2 3 4
==================================================================
Complexity:
- push_front: O(1)
- insert_after/erase_after: O(1) (given iterator)
- sort: O(n log n)
- merge: O(n)
- remove_if: O(n)
==================================================================
Interview Questions:
1. Why no push_back / pop_back on forward_list?
   -> Singly-linked; O(n) to find tail. Use list for back ops.

2. Why no size()?
   -> Maintaining size counter would add overhead. Can use
      std::distance(begin, end) but O(n).

3. What is before_begin()?
   -> A special iterator pointing before the first element.
      Used to insert at front via insert_after.
==================================================================
Common Mistakes:
- Using insert instead of insert_after (doesn't exist)
- Expecting bidirectional iteration (only forward)
- Calling size() on forward_list (doesn't exist)
- Assuming end() can be decremented (no --end)
==================================================================
*/
