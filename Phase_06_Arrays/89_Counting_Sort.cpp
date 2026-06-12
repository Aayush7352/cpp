/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Counting Sort                           ║
* ║  Program 89 — O(n+k) integer sort, stable version          ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Non-comparison integer sort. Works when the range of possible
*   values (k) is not too large relative to n.
* - Steps:
*     1. Count frequencies of each value.
*     2. Compute prefix sums to determine positions.
*     3. Place elements in output array (stable: traverse input R→L).
* - Stable version: iterates input backwards so that equal elements
*   retain their original order.
*
* SAMPLE I/O:
*   Before: [4 2 2 8 3 3 1]
*   After:  [1 2 2 3 3 4 8]
*
* COMPLEXITY:
*   Time: O(n + k) where k = range (max - min + 1).
*   Space: O(n + k).
*
* INTERVIEW QUESTIONS:
* 1. Q: When would you use counting sort instead of std::sort?
*    A: When n is large and k is small (e.g., ages 0-150, grades 0-100).
* 2. Q: Can counting sort handle negative numbers?
*    A: Yes — shift each value by min_val so indices are non-negative.
* 3. Q: Is counting sort stable?
*    A: The version below is stable (iterates input right-to-left).
*
* COMMON MISTAKES:
* - Not accounting for the minimum value (array index must be
*   non-negative and start from min).
* - Off-by-one in prefix sum calculation.
* - Forgetting to copy output back into the original array.
*/

#include <iostream>
#include <vector>
#include <algorithm>

void counting_sort(std::vector<int>& arr) {
    if (arr.empty()) return;

    auto [min_it, max_it] = std::minmax_element(arr.begin(), arr.end());
    int min_val = *min_it;
    int max_val = *max_it;
    int range = max_val - min_val + 1;

    std::vector<int> count(range, 0);
    for (int v : arr) ++count[v - min_val];

    // Prefix sum → position of last occurrence
    for (int i = 1; i < range; ++i)
        count[i] += count[i - 1];

    std::vector<int> output(arr.size());
    // Traverse input right-to-left for stability
    for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
        int idx = *it - min_val;
        output[count[idx] - 1] = *it;
        --count[idx];
    }

    arr = std::move(output);
}

int main() {
    std::vector<int> arr{4, 2, 2, 8, 3, 3, 1};

    std::cout << "Before: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    counting_sort(arr);

    std::cout << "After:  ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Stable: yes, O(n+k)\n";

    return 0;
}
