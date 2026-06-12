/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Rotate Array                            ║
* ║  Program 78 — Left/right rotation, reversal alg, std::rotate║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Left rotate by k: move first k elements to the end.
* - Right rotate by k: move last k elements to the front.
* - Reversal algorithm (popularised by B. Kernighan):
*     1. Reverse the entire array.
*     2. Reverse first k elements.
*     3. Reverse remaining n-k elements.
*   This is O(n) and O(1) space — elegant and fast.
* - std::rotate (C++11, <algorithm>) does this in-place for any
*   forward range. It returns an iterator to the new position of
*   *first (C++11).
*
* SAMPLE I/O:
*   Original  : [1 2 3 4 5 6 7]
*   Left 3    : [4 5 6 7 1 2 3]
*   Right 2   : [6 7 1 2 3 4 5]
*   std::rotate left 3: [4 5 6 7 1 2 3]
*
* COMPLEXITY:
*   Time: O(n), Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: What if k > n?
*    A: k %= n. A rotation by n is a no-op.
* 2. Q: Can you rotate with O(k) extra space?
*    A: Yes — copy first k to temp, shift the rest, copy temp back.
*       That is O(k) memory instead of O(1).
* 3. Q: Is std::rotate stable?
*    A: Yes — relative order of elements within each segment is preserved.
*
* COMMON MISTAKES:
* - Not taking k modulo n — whole cycles may repeat unnecessarily.
* - Confusing left vs right direction in the reversal algorithm.
* - Off-by-one in custom reversal helper.
*/

#include <iostream>
#include <algorithm>
#include <array>

void reverse(int arr[], std::size_t l, std::size_t r) {
    while (l < r) std::swap(arr[l++], arr[r--]);
}

void rotate_left_reversal(int arr[], std::size_t n, std::size_t k) {
    if (n == 0) return;
    k %= n;
    reverse(arr, 0, n - 1);
    reverse(arr, 0, n - k - 1);
    reverse(arr, n - k, n - 1);
}

void rotate_right_reversal(int arr[], std::size_t n, std::size_t k) {
    if (n == 0) return;
    k %= n;
    reverse(arr, 0, n - 1);
    reverse(arr, 0, k - 1);
    reverse(arr, k, n - 1);
}

int main() {
    std::array arr{1, 2, 3, 4, 5, 6, 7};
    const auto print = [&]{
        for (auto x : arr) std::cout << x << ' ';
        std::cout << '\n';
    };

    std::cout << "Original  : "; print();

    // Left by 3
    rotate_left_reversal(arr.data(), arr.size(), 3);
    std::cout << "Left 3    : "; print();

    // Right by 2
    rotate_right_reversal(arr.data(), arr.size(), 2);
    std::cout << "Right 2   : "; print();

    // Reset
    arr = {1, 2, 3, 4, 5, 6, 7};

    // Using std::rotate (left by 3)
    std::rotate(arr.begin(), arr.begin() + 3, arr.end());
    std::cout << "std::rotate left 3: "; print();

    return 0;
}
