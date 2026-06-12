/*
==================================================================
161. std::next_permutation / prev_permutation
==================================================================
Theory:
- next_permutation: transforms range to lexicographically next
  permutation (returns false if last permutation)
- prev_permutation: transforms to previous permutation
- Permutations: n! total for unique elements
- Uses operator< (or custom comparator)
- Permutations are generated in lexicographic order
- Often used with do-while loop to generate all permutations
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

int main() {
    std::cout << "=== next_permutation / prev_permutation\n\n";

    // --- Basic: next_permutation ---
    std::cout << "1. All permutations of {1,2,3}:\n";
    std::vector<int> v{1, 2, 3};
    std::sort(v.begin(), v.end()); // must start sorted
    int count = 0;
    do {
        std::cout << "   ";
        for (int x : v) std::cout << x << " ";
        std::cout << "\n";
        ++count;
    } while (std::next_permutation(v.begin(), v.end()));
    std::cout << "   Total: " << count << " permutations\n\n";

    // --- prev_permutation ---
    std::cout << "2. Previous permutations from last:\n";
    std::vector<int> w{3, 2, 1}; // start at last permutation
    std::cout << "   Starting: ";
    for (int x : w) std::cout << x << " ";
    std::cout << "\n";
    std::prev_permutation(w.begin(), w.end());
    std::cout << "   After prev: ";
    for (int x : w) std::cout << x << " ";
    std::cout << "\n\n";

    // --- With duplicates ---
    std::cout << "3. Permutations with duplicates {1,1,2}:\n";
    std::vector<int> dup{1, 1, 2};
    std::sort(dup.begin(), dup.end());
    int cnt = 0;
    do {
        std::cout << "   ";
        for (int x : dup) std::cout << x << " ";
        std::cout << "\n";
        ++cnt;
    } while (std::next_permutation(dup.begin(), dup.end()));
    std::cout << "   Total unique: " << cnt << "\n\n";

    // --- With string ---
    std::cout << "4. Permutations of \"ABC\":\n";
    std::string s = "ABC";
    std::sort(s.begin(), s.end());
    do {
        std::cout << "   " << s << "\n";
    } while (std::next_permutation(s.begin(), s.end()));
    std::cout << "\n";

    // --- Custom comparator (descending) ---
    std::cout << "5. Permutations with custom comparator:\n";
    std::vector<int> custom{3, 2, 1}; // start at largest
    std::cout << "   All permutations in descending order:\n";
    do {
        std::cout << "   ";
        for (int x : custom) std::cout << x << " ";
        std::cout << "\n";
    } while (std::prev_permutation(custom.begin(), custom.end()));

    // --- K-th permutation ---
    std::cout << "\n6. K-th permutation:\n";
    auto kthPerm = [](int n, int k) -> std::string {
        std::string s;
        for (int i = 1; i <= n; ++i) s += '0' + i;
        std::sort(s.begin(), s.end());
        for (int i = 1; i < k; ++i)
            std::next_permutation(s.begin(), s.end());
        return s;
    };
    std::cout << "   n=4, k=9: " << kthPerm(4, 9) << " (0-indexed: 9th perm)\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. All permutations of {1,2,3}:
   1 2 3
   1 3 2
   2 1 3
   2 3 1
   3 1 2
   3 2 1
   Total: 6 permutations

2. Previous permutations from last:
   Starting: 3 2 1
   After prev: 3 1 2

3. Permutations with duplicates {1,1,2}:
   1 1 2
   1 2 1
   2 1 1
   Total unique: 3

4. Permutations of "ABC":
   ABC
   ACB
   BAC
   BCA
   CAB
   CBA

5. Permutations with custom comparator:
   3 2 1
   3 1 2
   2 3 1
   2 1 3
   1 3 2
   1 2 3

6. K-th permutation:
   n=4, k=9: 2341
==================================================================
Complexity:
- next_permutation: O(n) per call (amortized O(1) for all n!)
- Total for all permutations: O(n × n!)
==================================================================
Interview Questions:
1. How does next_permutation work internally?
   -> Find largest i where a[i] < a[i+1]; find largest j > i
      where a[j] > a[i]; swap a[i] and a[j]; reverse [i+1, end).

2. How to generate permutations of a vector with duplicates?
   -> std::next_permutation handles duplicates correctly,
      generating each distinct permutation once.

3. How to find K-th permutation efficiently?
   -> Factorial number system O(n^2), or call next_permutation
      K times O(K×n) for small K.
==================================================================
Common Mistakes:
- Forgetting to sort before first next_permutation
- Using while instead of do-while (skips first permutation)
- Expecting next_permutation to restart from beginning
- Using prev_permutation where next_permutation is needed
- Not handling duplicates efficiently (generating n! even for dupes)
==================================================================
*/
