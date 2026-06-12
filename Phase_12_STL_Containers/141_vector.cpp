/*
==================================================================
141. std::vector
==================================================================
Theory:
- Dynamic array with contiguous storage (cache-friendly)
- Construction: default, fill, range, initializer list, copy, move
- Capacity: size(), capacity(), reserve(), shrink_to_fit()
- Modifiers: push_back, pop_back, insert, erase, clear, resize
- emplace_back vs push_back: emplace constructs in-place (avoids copy)
- Iterator invalidation: reallocation invalidates all iterators
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Expensive {
    std::string data;
    Expensive(const char* s) : data(s) {
        std::cout << "  Construct " << data << "\n";
    }
    Expensive(const Expensive& o) : data(o.data) {
        std::cout << "  Copy " << data << "\n";
    }
    Expensive(Expensive&& o) noexcept : data(std::move(o.data)) {
        std::cout << "  Move " << data << "\n";
    }
};

int main() {
    std::cout << "=== std::vector\n\n";

    // --- Construction ---
    std::cout << "1. Construction:\n";
    std::vector<int> empty;                                 // default
    std::vector<int> fill(5, 42);                           // 5 elements of 42
    std::vector<int> range(fill.begin(), fill.end());       // from iterators
    std::vector<int> ilist{1, 2, 3, 4, 5};                 // initializer list
    std::vector<int> copied(ilist);                         // copy
    std::vector<int> moved(std::move(copied));              // move

    std::cout << "   fill:  "; for (int x : fill)  std::cout << x << " ";
    std::cout << "\n   ilist: "; for (int x : ilist) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Capacity ---
    std::cout << "2. Capacity:\n";
    std::vector<int> v;
    std::cout << "   Initial: size=" << v.size() << " cap=" << v.capacity() << "\n";
    v.reserve(100);
    std::cout << "   After reserve(100): cap=" << v.capacity() << "\n";
    for (int i = 0; i < 10; ++i) v.push_back(i);
    std::cout << "   After 10 pushes: size=" << v.size() << " cap=" << v.capacity() << "\n";
    v.shrink_to_fit();
    std::cout << "   After shrink_to_fit: cap=" << v.capacity() << "\n\n";

    // --- Modifiers ---
    std::cout << "3. Modifiers:\n";
    v.clear();
    v.push_back(1);
    v.push_back(2);
    v.insert(v.begin(), 0);              // insert at front
    v.insert(v.end(), {3, 4, 5});        // insert multiple at end
    std::cout << "   After inserts: ";
    for (int x : v) std::cout << x << " ";

    v.erase(v.begin() + 2);             // erase 3rd element
    std::cout << "\n   After erase:  ";
    for (int x : v) std::cout << x << " ";

    v.resize(3);                         // truncate to 3
    std::cout << "\n   After resize(3): ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n\n";

    // --- emplace_back vs push_back ---
    std::cout << "4. push_back vs emplace_back:\n";
    std::vector<Expensive> ev;
    std::cout << "   push_back:\n";
    ev.push_back(Expensive("hello"));   // temp + move
    std::cout << "   emplace_back:\n";
    ev.emplace_back("world");           // construct in-place
    std::cout << "\n";

    // --- Algorithms ---
    std::cout << "5. Algorithms:\n";
    std::vector<int> nums = {5, 3, 1, 4, 2};
    std::sort(nums.begin(), nums.end());
    std::cout << "   Sorted: ";
    for (int x : nums) std::cout << x << " ";
    std::cout << "\n";

    // 2D vector
    std::cout << "\n6. 2D vector (matrix):\n";
    std::vector<std::vector<int>> matrix(3, std::vector<int>(4, 0));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            matrix[i][j] = i * 4 + j;

    for (const auto& row : matrix) {
        for (int x : row) std::cout << x << "\t";
        std::cout << "\n";
    }

    return 0;
}

/*
==================================================================
Sample I/O:
1. Construction:
   fill:  42 42 42 42 42
   ilist: 1 2 3 4 5

2. Capacity:
   Initial: size=0 cap=0
   After reserve(100): cap=100
   After 10 pushes: size=10 cap=100
   After shrink_to_fit: cap=10

3. Modifiers:
   After inserts: 0 1 2 3 4 5
   After erase:  0 1 3 4 5
   After resize(3): 0 1 3

4. push_back vs emplace_back:
   push_back:
     Construct hello
     Move hello
   emplace_back:
     Construct world

5. Sorted: 1 2 3 4 5

6. 2D vector:
0    1    2    3
4    5    6    7
8    9    10   11
==================================================================
Complexity:
- Random access: O(1)
- Push_back (amortized): O(1)
- Insert at position: O(n)
- Erase at position: O(n)
==================================================================
Interview Questions:
1. When does vector reallocate?
   -> When size > capacity. New capacity = old * growth_factor
      (typically 1.5x or 2x depending on implementation).

2. Why is emplace_back preferred over push_back?
   -> emplace_back constructs in-place, avoiding a temporary
      and a move/copy. For cheap types, difference is negligible.

3. How to avoid reallocation?
   -> Use reserve() to pre-allocate enough capacity.
==================================================================
Common Mistakes:
- Using v[i] when i >= size (undefined behavior; use at() for bounds checking)
- Keeping iterators after push_back (invalidation)
- Forgetting shrink_to_fit() to release excess memory
- Assuming sizeof(vector) reflects element count
- Passing vector by value unnecessarily (use const&)
==================================================================
*/
