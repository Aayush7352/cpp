/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Binary Search                           ║
* ║  Program 82 — Iterative, recursive, std::binary_search,    ║
* ║               lower_bound, upper_bound                     ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Binary search works on a sorted array. O(log n).
* - Iterative: maintain low/high pointers; mid = low + (high-low)/2
*   (avoids overflow of (low+high)/2).
* - Recursive: same logic but with function call overhead.
* - Standard library:
*   • std::binary_search — returns bool (found or not). C++11, <algorithm>.
*   • std::lower_bound — iterator to first element ≥ value.
*   • std::upper_bound — iterator to first element > value.
*   • std::equal_range — pair of lower/upper bound.
*
* SAMPLE I/O:
*   Sorted array: [2 5 8 12 16 23 38 56 72 91]
*   Search 23: found
*   Search 15: not found
*   lower_bound(23) → index 5 (value 23)
*   upper_bound(23) → index 6 (value 38)
*
* COMPLEXITY:
*   Time: O(log n)
*   Space: O(1) iterative, O(log n) recursive (call stack).
*
* INTERVIEW QUESTIONS:
* 1. Q: Why use mid = low + (high - low) / 2?
*    A: Avoids integer overflow when low+high exceeds INT_MAX.
* 2. Q: What if the array contains duplicates?
*    A: binary_search still returns true. lower_bound gives first
*       occurrence; upper_bound gives one past the last.
* 3. Q: Can binary search be applied to an unsorted array?
*    A: No — the result is undefined.
*
* COMMON MISTAKES:
* - Off-by-one in high = mid - 1 vs high = mid.
* - Using (low + high) / 2 which can overflow.
* - Forgetting that the array must be sorted.
*/

#include <iostream>
#include <algorithm>
#include <array>

// ── Iterative binary search ──────────────────────────────────
int binary_search_iter(const int arr[], std::size_t n, int key) {
    std::size_t low = 0, high = n;           // high is exclusive
    while (low < high) {
        std::size_t mid = low + (high - low) / 2;
        if (arr[mid] == key) return static_cast<int>(mid);
        if (arr[mid] < key)  low = mid + 1;
        else                 high = mid;
    }
    return -1;
}

// ── Recursive binary search ──────────────────────────────────
int binary_search_rec(const int arr[], int low, int high, int key) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (arr[mid] == key) return mid;
    if (arr[mid] < key)  return binary_search_rec(arr, mid + 1, high, key);
    return binary_search_rec(arr, low, mid - 1, key);
}

int main() {
    constexpr std::array arr{2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    constexpr auto n = arr.size();

    std::cout << "Sorted array: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    // ── Iterative ────────────────────────────────────────────
    int idx = binary_search_iter(arr.data(), n, 23);
    std::cout << "Search 23 (iter): " << (idx != -1 ? "found" : "not found") << '\n';

    idx = binary_search_iter(arr.data(), n, 15);
    std::cout << "Search 15 (iter): " << (idx != -1 ? "found" : "not found") << '\n';

    // ── Recursive ────────────────────────────────────────────
    idx = binary_search_rec(arr.data(), 0, static_cast<int>(n) - 1, 23);
    std::cout << "Search 23 (rec):  " << (idx != -1 ? "found" : "not found") << '\n';

    // ── Standard library ────────────────────────────────────
    bool found = std::binary_search(arr.begin(), arr.end(), 23);
    std::cout << "std::binary_search(23): " << std::boolalpha << found << '\n';

    auto lb = std::lower_bound(arr.begin(), arr.end(), 23);
    auto ub = std::upper_bound(arr.begin(), arr.end(), 23);
    std::cout << "lower_bound(23) → index " << (lb - arr.begin())
              << " (value " << *lb << ")\n";
    std::cout << "upper_bound(23) → index " << (ub - arr.begin())
              << " (value " << *ub << ")\n";

    return 0;
}
