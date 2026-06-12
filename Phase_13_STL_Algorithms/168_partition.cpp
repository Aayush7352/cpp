/*
==================================================================
168. std::partition, stable_partition, partition_point, is_partitioned
==================================================================
Theory:
- std::partition: reorders elements by predicate (true first)
- stable_partition: preserves relative order within groups
- partition_point: finds partition boundary on partitioned range
- is_partitioned: checks if range is partitioned
- All O(n) for partition, O(n log n) for stable_partition
- Returns iterator to first element where predicate is false
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>

int main() {
    std::cout << "=== Partition Algorithms\n\n";

    // --- is_partitioned ---
    std::cout << "1. is_partitioned:\n";
    std::vector<int> part{2, 4, 6, 1, 3, 5};  // evens then odds
    bool already = std::is_partitioned(part.begin(), part.end(),
                                        [](int x) { return x % 2 == 0; });
    std::cout << "   {2,4,6,1,3,5} partitioned by even: "
              << already << "\n\n";

    // --- partition ---
    std::cout << "2. std::partition:\n";
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    auto parEnd = std::partition(v.begin(), v.end(),
                                  [](int x) { return x % 2 == 0; });
    std::cout << "   Partitioned by even:\n";
    std::cout << "   Evens: ";
    for (auto it = v.begin(); it != parEnd; ++it) std::cout << *it << " ";
    std::cout << "\n   Odds:  ";
    for (auto it = parEnd; it != v.end(); ++it) std::cout << *it << " ";
    std::cout << "\n   Relative order NOT preserved\n\n";

    // --- stable_partition ---
    std::cout << "3. std::stable_partition:\n";
    std::vector<int> sv{1, 2, 3, 4, 5, 6, 7, 8};
    auto spEnd = std::stable_partition(sv.begin(), sv.end(),
                                        [](int x) { return x % 2 == 0; });
    std::cout << "   Stable partitioned by even:\n";
    std::cout << "   Evens (original order): ";
    for (auto it = sv.begin(); it != spEnd; ++it) std::cout << *it << " ";
    std::cout << "\n   Odds (original order):  ";
    for (auto it = spEnd; it != sv.end(); ++it) std::cout << *it << " ";
    std::cout << "\n\n";

    // --- partition_point ---
    std::cout << "4. partition_point:\n";
    auto pp = std::partition_point(sv.begin(), sv.end(),
                                    [](int x) { return x % 2 == 0; });
    std::cout << "   Partition point at index " << (pp - sv.begin())
              << " (value " << *pp << ")\n";
    // Verify: all in [begin, pp) satisfy, all in [pp, end) don't
    bool ok = std::all_of(sv.begin(), pp,
                           [](int x) { return x % 2 == 0; })
           && std::none_of(pp, sv.end(),
                           [](int x) { return x % 2 == 0; });
    std::cout << "   Verification: " << ok << "\n\n";

    // --- Partitioning strings ---
    std::cout << "5. Partition strings by length:\n";
    std::vector<std::string> words{"cat", "giraffe", "dog", "elephant", "fox"};
    auto mid = std::partition(words.begin(), words.end(),
                               [](const std::string& s) { return s.length() <= 3; });
    std::cout << "   Short (<=3): ";
    for (auto it = words.begin(); it != mid; ++it) std::cout << *it << " ";
    std::cout << "\n   Long (>3):   ";
    for (auto it = mid; it != words.end(); ++it) std::cout << *it << " ";
    std::cout << "\n\n";

    // --- Partition + stable combined ---
    std::cout << "6. Partition by multiple predicates:\n";
    std::vector<int> multi{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // First partition: evens vs odds (stable)
    auto evenEnd = std::stable_partition(multi.begin(), multi.end(),
                                          [](int x) { return x % 2 == 0; });
    // Then partition evens into divisible by 4 vs not
    auto div4End = std::stable_partition(multi.begin(), evenEnd,
                                          [](int x) { return x % 4 == 0; });
    std::cout << "   Divisible by 4: ";
    for (auto it = multi.begin(); it != div4End; ++it) std::cout << *it << " ";
    std::cout << "\n   Even not div4: ";
    for (auto it = div4End; it != evenEnd; ++it) std::cout << *it << " ";
    std::cout << "\n   Odds:          ";
    for (auto it = evenEnd; it != multi.end(); ++it) std::cout << *it << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. is_partitioned:
   {2,4,6,1,3,5} partitioned by even: 1

2. std::partition:
   Partitioned by even:
   Evens: 2 4 6 8
   Odds:  1 3 5 7
   Relative order NOT preserved

3. std::stable_partition:
   Stable partitioned by even:
   Evens (original order): 2 4 6 8
   Odds (original order):  1 3 5 7

4. partition_point:
   Partition point at index 4 (value 1)
   Verification: 1

5. Partition strings by length:
   Short (<=3): cat dog fox
   Long (>3):   giraffe elephant

6. Partition by multiple predicates:
   Divisible by 4: 4 8
   Even not div4: 2 6 10
   Odds:          1 3 5 7 9
==================================================================
Complexity:
- partition: O(n)
- stable_partition: O(n log n) if memory allocated, O(n) otherwise
- partition_point: O(log n)
- is_partitioned: O(n)
==================================================================
Interview Questions:
1. partition vs stable_partition?
   -> partition: may reorder equal elements (unstable).
      stable_partition: preserves relative order, uses extra memory.

2. When to use partition_point?
   -> After partitioning, to find boundary. O(log n) binary search.

3. How to partition into more than 2 groups?
   -> Apply partition sequentially on each subrange.
==================================================================
Common Mistakes:
- Using partition_point on non-partitioned range (wrong result)
- Expecting stable_partition to be O(n) without memory
- Forgetting partition returns iterator to partition point
- Assuming is_partitioned before partition
- Not using stable_partition when order matters
==================================================================
*/
