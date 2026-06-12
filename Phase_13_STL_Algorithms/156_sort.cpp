/*
==================================================================
156. std::sort
==================================================================
Theory:
- IntroSort: QuickSort + HeapSort + InsertionSort hybrid
- Average O(n log n), worst-case O(n log n) (HeapSort fallback)
- Requires random-access iterators (vector, array, deque)
- Not stable (see stable_sort)
- partial_sort: top k elements sorted (HeapSort)
- nth_element: partition at n-th position (QuickSelect)
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <random>

int main() {
    std::cout << "=== std::sort, partial_sort, nth_element\n\n";

    // --- Basic sort ---
    std::cout << "1. std::sort (ascending):\n";
    std::vector<int> v{9, 2, 7, 1, 5, 3, 8, 4, 6};
    std::sort(v.begin(), v.end());
    std::cout << "   ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Descending ---
    std::cout << "2. std::sort (descending):\n";
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::cout << "   ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n\n";

    // --- partial_sort ---
    std::cout << "3. partial_sort (top 3):\n";
    std::vector<int> v2{9, 2, 7, 1, 5, 3, 8, 4, 6};
    std::partial_sort(v2.begin(), v2.begin() + 3, v2.end(), std::greater<int>());
    std::cout << "   Top 3 sorted: ";
    for (size_t i = 0; i < 3; ++i) std::cout << v2[i] << " ";
    std::cout << "\n   Rest (unsorted): ";
    for (size_t i = 3; i < v2.size(); ++i) std::cout << v2[i] << " ";
    std::cout << "\n\n";

    // --- nth_element ---
    std::cout << "4. nth_element (median = 5th element):\n";
    std::vector<int> v3{9, 2, 7, 1, 5, 3, 8, 4, 6};
    auto mid = v3.begin() + v3.size() / 2;
    std::nth_element(v3.begin(), mid, v3.end());
    std::cout << "   Median: " << *mid << "\n";
    std::cout << "   Before median (all <= median): ";
    for (auto it = v3.begin(); it != mid; ++it) std::cout << *it << " ";
    std::cout << "\n   After median (all >= median): ";
    for (auto it = mid + 1; it != v3.end(); ++it) std::cout << *it << " ";
    std::cout << "\n\n";

    // --- Lambda comparator ---
    std::cout << "5. Sort with lambda (by string length):\n";
    std::vector<std::string> words{"apple", "kiwi", "banana", "grape", "fig"};
    std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
        return a.length() < b.length();
    });
    for (const auto& w : words) std::cout << "   " << w << " (len=" << w.length() << ")\n";

    // --- Sorting pairs ---
    std::cout << "\n6. Sort pairs (by second then first):\n";
    std::vector<std::pair<int,int>> pairs{{1,5}, {2,3}, {3,5}, {4,2}};
    std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });
    for (const auto& [f, s] : pairs)
        std::cout << "   (" << f << "," << s << ")\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. std::sort (ascending):
   1 2 3 4 5 6 7 8 9

2. std::sort (descending):
   9 8 7 6 5 4 3 2 1

3. partial_sort (top 3):
   Top 3 sorted: 9 8 7
   Rest (unsorted): 1 3 2 6 5 4

4. nth_element (median = 5th element):
   Median: 5
   Before median (all <= median): 2 1 3 4
   After median (all >= median): 7 8 9 6

5. Sort with lambda (by string length):
   fig (len=3)
   kiwi (len=4)
   apple (len=5)
   grape (len=5)
   banana (len=6)

6. Sort pairs (by second then first):
   (4,2)
   (2,3)
   (1,5)
   (3,5)
==================================================================
Complexity:
- sort: O(n log n)
- partial_sort: O(n log k) where k = sorted element count
- nth_element: O(n) average, O(n^2) worst-case
==================================================================
Interview Questions:
1. Which sort does std::sort use?
   -> Introsort (QuickSort + HeapSort fallback + InsertionSort
      for small partitions).

2. When is nth_element useful?
   -> Finding median, percentile, or kth smallest/largest in O(n).

3. partial_sort vs nth_element?
   -> partial_sort gives k sorted elements. nth_element gives
      one element in correct position and partitions around it.
==================================================================
Common Mistakes:
- Using sort on list/forward_list (needs random-access iterators)
- Forgetting comparator returns strict weak ordering
- Using sort where stable_sort needed
- Expecting partial_sort to sort all elements
- Passing end iterator as middle to partial_sort/nth_element
==================================================================
*/
