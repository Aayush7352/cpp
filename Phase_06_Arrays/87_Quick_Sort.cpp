/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Quick Sort                              ║
* ║  Program 87 — Median-of-three pivot, Lomuto partition      ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Divide-and-conquer: pick a pivot, partition array such that
*   elements < pivot come before elements ≥ pivot, then recurse.
* - Lomuto partition: scan left-to-right, swap smaller elements.
*   Simpler but O(n²) for already-sorted arrays if pivot is last.
* - Median-of-three: pivot = median of first, middle, last —
*   reduces chance of worst-case on nearly-sorted data.
* - Hoare partition: two pointers from both ends — more efficient but
*   trickier. Used here is Lomuto with median-of-three improvement.
*
* SAMPLE I/O:
*   Before: [10 7 8 9 1 5]
*   After:  [1 5 7 8 9 10]
*
* COMPLEXITY:
*   Time: O(n log n) average, O(n²) worst-case.
*   Space: O(log n) recursion stack.
*
* INTERVIEW QUESTIONS:
* 1. Q: Why is quick sort preferred over merge sort in practice?
*    A: In-place partitioning and good cache locality; often faster
*       despite O(n²) worst-case.
* 2. Q: How does median-of-three help?
*    A: Ensures pivot is approximately the median, splitting more
*       evenly. Avoids O(n²) on sorted arrays.
* 3. Q: Is quick sort stable?
*    A: Lomuto partition is not stable (swapping moves equal elements
*       across each other).
*
* COMMON MISTAKES:
* - Incorrect partition returning wrong pivot index.
* - Recursion base case: handle sub-array of size ≤ 1.
* - Forgetting to include the pivot element in one of the partitions.
*/

#include <iostream>
#include <vector>
#include <algorithm>

// Median-of-three: returns index of median value
int median_of_three(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[mid] < arr[low]) std::swap(arr[low], arr[mid]);
    if (arr[high] < arr[low]) std::swap(arr[low], arr[high]);
    if (arr[high] < arr[mid]) std::swap(arr[mid], arr[high]);
    return mid;   // median is now at mid
}

int lomuto_partition(std::vector<int>& arr, int low, int high) {
    int pivot_idx = median_of_three(arr, low, high);
    std::swap(arr[pivot_idx], arr[high]);   // move pivot to end
    int pivot = arr[high];

    int i = low;
    for (int j = low; j < high; ++j)
        if (arr[j] < pivot)
            std::swap(arr[i++], arr[j]);

    std::swap(arr[i], arr[high]);
    return i;
}

void quick_sort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = lomuto_partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

int main() {
    std::vector<int> arr{10, 7, 8, 9, 1, 5};

    std::cout << "Before: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    quick_sort(arr, 0, static_cast<int>(arr.size()) - 1);

    std::cout << "After:  ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Pivot: median-of-three, Partition: Lomuto\n";

    return 0;
}
