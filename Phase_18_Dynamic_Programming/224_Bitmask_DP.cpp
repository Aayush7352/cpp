/*
 * ====================================================================
 * PROGRAM 224: Bitmask_DP.cpp
 * TOPIC    : Dynamic Programming - DP over subsets (Bitmask DP)
 * ====================================================================
 * THEORY:
 * Bitmask DP uses bitwise operations to represent subsets.
 * Each bit in an integer represents whether an element is selected.
 * mask = 0b1101 means elements 0, 2, 3 are selected (bits 0,2,3 set).
 *
 * TRAVELING SALESMAN PROBLEM (TSP) WITH BITMASK:
 *   DP[mask][last] = minimum cost to visit all nodes in mask,
 *   ending at node 'last'.
 *   Start: DP[1<<0][0] = 0 (starting from node 0)
 *   Transition: DP[mask | (1<<k)][k] = min(DP[mask][last] + cost[last][k])
 *
 * TERMINATION:
 *   Answer = min(DP[(1<<n)-1][last] + cost[last][0]) for all last
 *   (return to start).
 *
 * COMPLEXITY:
 *   TSP: O(n^2 * 2^n) time, O(n * 2^n) space
 *
 * INTERVIEW QUESTIONS:
 * Q: What are other applications of bitmask DP?
 * A: Matching, graph partitioning, subset sum with constraints.
 *
 * Q: Can we handle n > 20?
 * A: n=20 is practical (2^20 ≈ 1M). For larger, use DP with pruning
 *    or branch and bound. For n=25, 2^25=33M, possible with optimization.
 *
 * COMMON MISTAKES:
 * - Forgetting to include the starting node in mask
 * - Wrong bit operations (| vs & vs >>)
 * - Off-by-one in bit positions
 * - Not handling unreachable states (initializing with INF)
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

// ---------- TSP WITH BITMASK ----------
int tsp(const vector<vector<int>>& cost) {
    int n = cost.size();
    int total_masks = 1 << n;
    vector<vector<int>> dp(total_masks, vector<int>(n, INT_MAX));

    dp[1][0] = 0; // start at node 0, mask = 1 (bit 0 set)

    for (int mask = 1; mask < total_masks; ++mask) {
        for (int last = 0; last < n; ++last) {
            if (!(mask & (1 << last))) continue;
            if (dp[mask][last] == INT_MAX) continue;

            for (int nxt = 0; nxt < n; ++nxt) {
                if (mask & (1 << nxt)) continue; // already visited
                int newMask = mask | (1 << nxt);
                if (dp[mask][last] + cost[last][nxt] < dp[newMask][nxt])
                    dp[newMask][nxt] = dp[mask][last] + cost[last][nxt];
            }
        }
    }

    int fullMask = total_masks - 1;
    int ans = INT_MAX;
    for (int last = 1; last < n; ++last) {
        if (dp[fullMask][last] != INT_MAX && dp[fullMask][last] + cost[last][0] < ans)
            ans = dp[fullMask][last] + cost[last][0];
    }
    return ans;
}

// ---------- SUBSET SUM WITH BITMASK ----------
bool subsetSumBitmask(const vector<int>& arr, int target) {
    int n = arr.size();
    int total = 1 << n;
    for (int mask = 0; mask < total; ++mask) {
        int sum = 0;
        for (int i = 0; i < n; ++i)
            if (mask & (1 << i)) sum += arr[i];
        if (sum == target) return true;
    }
    return false;
}

// ---------- MAIN ----------
int main() {
    cout << "=== BITMASK DP ===\n\n";

    // TSP example
    vector<vector<int>> cost = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    cout << "Cost matrix:\n";
    for (auto& row : cost) {
        for (int c : row) cout << c << "\t";
        cout << "\n";
    }
    cout << "\nTSP minimum cost: " << tsp(cost) << "\n\n";

    // Subset sum with bitmask
    vector<int> arr = {3, 7, 2, 9, 1};
    int target = 10;
    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << "\nSubset sum " << target << " possible? "
         << (subsetSumBitmask(arr, target) ? "Yes" : "No") << "\n";

    /*
     * SAMPLE I/O:
     * Cost matrix:
     * 0	10	15	20
     * 10	0	35	25
     * 15	35	0	30
     * 20	25	30	0
     *
     * TSP minimum cost: 80
     * Path: 0 -> 1 -> 3 -> 2 -> 0 (10+25+30+15=80)
     *
     * Array: 3 7 2 9 1
     * Subset sum 10 possible? Yes
     */

    return 0;
}
