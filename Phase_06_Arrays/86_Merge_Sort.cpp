/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Merge Sort                              ║
* ║  Program 86 — Recursive merge sort, in-place merging       ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Divide-and-conquer: split array into halves, recursively sort,
*   then merge the two sorted halves.
* - Stable: equal elements from left half appear before right half
*   because we merge the left first on ties.
* - O(n log n) guaranteed — no worst-case degradation.
* - Merging requires O(n) auxiliary space (for the temp array).
*
* SAMPLE I/O:
*   Before: [38 27 43 3 9 82 10]
*   After:  [3 9 10 27 38 43 82]
*
* COMPLEXITY:
*   Time: O(n log n) always.
*   Space: O(n) auxiliary.
*
* INTERVIEW QUESTIONS:
* 1. Q: Why is merge sort not in-place?
*    A: Merging two sorted sub-arrays needs O(n) extra space to
*       combine them in linear time. In-place merge sort exists but
*       is complex and slower (O(log n) space at minimum).
* 2. Q: Is merge sort stable?
*    A: Yes — if the merge prefers left when equal.
* 3. Q: Compare merge sort and quick sort.
*    A: Merge sort guarantees O(n log n) and is stable; quick sort
*       is in-place but O(n²) worst-case, usually faster in practice.
*
* COMMON MISTAKES:
* - Copying too much or too little in the merge step.
* - Forgetting to recombine the temp array back into original.
* - Recursive base case: if low >= high (or low == high for size 1).
*/

#include <iostream>
#include <vector>

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];   // ≤ for stability
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main() {
    std::vector<int> arr{38, 27, 43, 3, 9, 82, 10};

    std::cout << "Before: ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    merge_sort(arr, 0, static_cast<int>(arr.size()) - 1);

    std::cout << "After:  ";
    for (auto x : arr) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Stable: yes, O(n log n) guaranteed\n";

    return 0;
}
