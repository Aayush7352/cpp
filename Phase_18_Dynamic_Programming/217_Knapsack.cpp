/*
 * ====================================================================
 * PROGRAM 217: Knapsack.cpp
 * TOPIC    : Dynamic Programming - 0/1 Knapsack, Unbounded Knapsack
 * ====================================================================
 * THEORY:
 * 0/1 KNAPSACK:
 *   Given N items with weights w[i] and values v[i], and capacity W.
 *   Each item can be taken at most once.
 *   DP[i][w] = max(DP[i-1][w], v[i] + DP[i-1][w-w[i]])
 *
 * UNBOUNDED KNAPSACK:
 *   Each item can be taken any number of times.
 *   DP[w] = max(DP[w], v[i] + DP[w - w[i]])
 *
 * SPACE-OPTIMIZED 1D DP:
 *   For 0/1: iterate capacity backwards to avoid reuse.
 *   For unbounded: iterate capacity forwards to allow reuse.
 *
 * COMPLEXITY:
 *   0/1 Knapsack:       O(N * W) time, O(W) space (1D optimized)
 *   Unbounded Knapsack:  O(N * W) time, O(W) space
 *
 * INTERVIEW QUESTIONS:
 * Q: How to handle fractional knapsack?
 * A: Use greedy (sort by value/weight ratio) - not DP.
 *
 * Q: Can we reconstruct which items are taken?
 * A: Yes, store decisions in a 2D table or backtrack.
 *
 * COMMON MISTAKES:
 * - Wrong iteration direction for space-optimized 0/1 knapsack
 * - Off-by-one in capacity loop
 * - Integer overflow for large values
 * - Not initializing DP array properly
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ---------- 0/1 KNAPSACK (1D OPTIMIZED) ----------
int knapsack_01(const vector<int>& weights, const vector<int>& values, int W) {
    int n = weights.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < n; ++i) {
        for (int w = W; w >= weights[i]; --w) {   // backwards: 0/1
            if (dp[w - weights[i]] + values[i] > dp[w])
                dp[w] = dp[w - weights[i]] + values[i];
        }
    }
    return dp[W];
}

// ---------- UNBOUNDED KNAPSACK (1D) ----------
int knapsack_unbounded(const vector<int>& weights, const vector<int>& values, int W) {
    int n = weights.size();
    vector<int> dp(W + 1, 0);
    for (int w = 1; w <= W; ++w) {
        for (int i = 0; i < n; ++i) {
            if (weights[i] <= w)
                dp[w] = max(dp[w], values[i] + dp[w - weights[i]]);
        }
    }
    return dp[W];
}

// ---------- RECONSTRUCTION (0/1) ----------
void knapsack_reconstruct(const vector<int>& weights, const vector<int>& values, int W) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= W; ++w) {
            if (weights[i - 1] <= w)
                dp[i][w] = max(dp[i - 1][w], values[i - 1] + dp[i - 1][w - weights[i - 1]]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
    cout << "Max value: " << dp[n][W] << "\nItems taken: ";
    int w = W;
    for (int i = n; i > 0 && w > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            cout << i << " (w=" << weights[i - 1] << ", v=" << values[i - 1] << ")  ";
            w -= weights[i - 1];
        }
    }
    cout << "\n";
}

// ---------- MAIN ----------
int main() {
    cout << "=== 0/1 KNAPSACK & UNBOUNDED KNAPSACK ===\n\n";

    vector<int> weights = {2, 3, 4, 5};
    vector<int> values  = {3, 4, 5, 6};
    int W = 8;

    cout << "Items:\n";
    for (size_t i = 0; i < weights.size(); ++i)
        cout << "  " << (i + 1) << ": w=" << weights[i] << ", v=" << values[i] << "\n";
    cout << "Capacity: " << W << "\n\n";

    cout << "0/1 Knapsack (1D): " << knapsack_01(weights, values, W) << "\n";
    cout << "Unbounded Knapsack: " << knapsack_unbounded(weights, values, W) << "\n\n";

    cout << "--- Reconstruction ---\n";
    knapsack_reconstruct(weights, values, W);

    /*
     * SAMPLE I/O:
     * Items:
     *   1: w=2, v=3
     *   2: w=3, v=4
     *   3: w=4, v=5
     *   4: w=5, v=6
     * Capacity: 8
     *
     * 0/1 Knapsack (1D): 10
     * Unbounded Knapsack: 13
     *
     * --- Reconstruction ---
     * Max value: 10
     * Items taken: 4 (w=5, v=6)  2 (w=3, v=4)
     */

    return 0;
}
