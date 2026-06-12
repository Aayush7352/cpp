/*
 * ====================================================================
 * PROGRAM 220: LIS.cpp
 * TOPIC    : Dynamic Programming - Longest Increasing Subsequence
 * ====================================================================
 * THEORY:
 * LIS is the longest subsequence of an array where elements are
 * in strictly increasing order.
 *
 * APPROACHES:
 * 1. O(n^2) DP:
 *    dp[i] = 1 + max(dp[j]) for j < i and arr[j] < arr[i]
 *    Initialize dp[i] = 1 for all i.
 *
 * 2. O(n log n) Patience Sorting:
 *    Maintain a vector "tails" where tails[k] = minimum possible
 *    last element of an increasing subsequence of length k+1.
 *    For each x, find lower_bound of x in tails and replace.
 *    Length of tails = LIS length.
 *
 * COMPLEXITY:
 *   O(n^2): Time O(n^2), Space O(n)
 *   O(n log n): Time O(n log n), Space O(n)
 *
 * INTERVIEW QUESTIONS:
 * Q: How to print LIS?
 * A: In O(n^2), store predecessor indices and backtrack.
 *    In O(n log n), track indices and reconstruct.
 *
 * Q: What about non-decreasing LIS?
 * A: Use upper_bound instead of lower_bound for patience sorting.
 *
 * Q: Longest Decreasing Subsequence?
 * A: Negate values or reverse array, same algorithm.
 *
 * COMMON MISTAKES:
 * - Confusing subsequence vs subarray (subarray = contiguous)
 * - Using < instead of <= for strictly increasing
 * - Forgetting initial dp[i] = 1
 * - Off-by-one with lower_bound/upper_bound
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ---------- O(n^2) LIS ----------
int lis_n2(const vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return 0;
    vector<int> dp(n, 1);
    int ans = 1;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i])
                dp[i] = dp[j] + 1;
        }
        ans = max(ans, dp[i]);
    }
    return ans;
}

// ---------- O(n log n) PATIENCE SORTING ----------
int lis_nlogn(const vector<int>& arr) {
    vector<int> tails;
    for (int x : arr) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end())
            tails.push_back(x);
        else
            *it = x;
    }
    return tails.size();
}

// ---------- PRINT LIS (using O(n^2) with predecessor) ----------
vector<int> lis_print(const vector<int>& arr) {
    int n = arr.size();
    if (n == 0) return {};
    vector<int> dp(n, 1), parent(n, -1);
    int best_len = 1, best_idx = 0;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
        if (dp[i] > best_len) {
            best_len = dp[i];
            best_idx = i;
        }
    }
    vector<int> result;
    while (best_idx != -1) {
        result.push_back(arr[best_idx]);
        best_idx = parent[best_idx];
    }
    reverse(result.begin(), result.end());
    return result;
}

// ---------- MAIN ----------
int main() {
    cout << "=== LONGEST INCREASING SUBSEQUENCE ===\n\n";

    vector<int> arr = {10, 22, 9, 33, 21, 50, 41, 60, 80};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << "\n\n";

    cout << "LIS O(n^2): " << lis_n2(arr) << "\n";
    cout << "LIS O(n log n): " << lis_nlogn(arr) << "\n";

    vector<int> lis_seq = lis_print(arr);
    cout << "LIS Sequence: ";
    for (int x : lis_seq) cout << x << " ";
    cout << "\n";

    /*
     * SAMPLE I/O:
     * Array: 10 22 9 33 21 50 41 60 80
     *
     * LIS O(n^2): 6
     * LIS O(n log n): 6
     * LIS Sequence: 10 22 33 41 60 80  (or 10 22 33 50 60 80)
     */

    return 0;
}
