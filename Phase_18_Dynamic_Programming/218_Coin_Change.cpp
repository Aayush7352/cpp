/*
 * ====================================================================
 * PROGRAM 218: Coin_Change.cpp
 * TOPIC    : Dynamic Programming - Coin Change
 * ====================================================================
 * THEORY:
 * MINIMUM COINS:
 *   Given unlimited coins of denominations d[], find min coins to
 *   make amount N.
 *   DP[i] = 1 + min(DP[i - d[j]]) for all d[j] <= i
 *   Initialization: DP[0] = 0, rest = INF
 *
 * TOTAL WAYS FOR COIN CHANGE:
 *   Count number of ways to make amount N with given denominations.
 *   Unbounded knapsack variation.
 *   DP[i] = sum(DP[i - d[j]]) for all d[j] <= i
 *
 * COMPLEXITY:
 *   Time: O(N * M) where N = amount, M = number of denominations
 *   Space: O(N)
 *
 * INTERVIEW QUESTIONS:
 * Q: What if denominations have limited supply?
 * A: Use bounded knapsack DP.
 *
 * Q: How to handle very large amounts?
 * A: Use BFS if denominations are small, or modulo arithmetic.
 *
 * COMMON MISTAKES:
 * - Forgetting to initialize DP with INF for min coins
 * - Overflow when counting ways
 * - Not handling unreachable amounts (return -1 for min coins)
 * - Confusing min coins vs number of ways
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

// ---------- MINIMUM COINS ----------
int minCoins(const vector<int>& coins, int amount) {
    vector<long long> dp(amount + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i <= amount; ++i) {
        for (int coin : coins) {
            if (coin <= i)
                dp[i] = min(dp[i], 1 + dp[i - coin]);
        }
    }
    return dp[amount] == INT_MAX ? -1 : (int)dp[amount];
}

// ---------- TOTAL WAYS ----------
int totalWays(const vector<int>& coins, int amount) {
    vector<long long> dp(amount + 1, 0);
    dp[0] = 1;
    for (int coin : coins) {
        for (int i = coin; i <= amount; ++i)
            dp[i] += dp[i - coin];
    }
    return (int)dp[amount];
}

// ---------- RECONSTRUCTION OF COINS USED ----------
vector<int> minCoinsReconstruct(const vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX);
    vector<int> choice(amount + 1, -1);
    dp[0] = 0;
    for (int i = 1; i <= amount; ++i) {
        for (int coin : coins) {
            if (coin <= i && 1 + dp[i - coin] < dp[i]) {
                dp[i] = 1 + dp[i - coin];
                choice[i] = coin;
            }
        }
    }
    vector<int> result;
    if (dp[amount] == INT_MAX) return result;
    int cur = amount;
    while (cur > 0) {
        result.push_back(choice[cur]);
        cur -= choice[cur];
    }
    return result;
}

// ---------- MAIN ----------
int main() {
    cout << "=== COIN CHANGE ===\n\n";

    vector<int> coins = {1, 2, 5, 10, 20};
    int amount = 37;

    cout << "Denominations: ";
    for (int c : coins) cout << c << " ";
    cout << "\nAmount: " << amount << "\n\n";

    int min_c = minCoins(coins, amount);
    cout << "Minimum coins needed: " << min_c << "\n";
    if (min_c != -1) {
        vector<int> used = minCoinsReconstruct(coins, amount);
        cout << "Coins used: ";
        for (int c : used) cout << c << " ";
        cout << "\n";
    }
    cout << "Total ways to make change: " << totalWays(coins, amount) << "\n";

    /*
     * SAMPLE I/O:
     * Denominations: 1 2 5 10 20
     * Amount: 37
     *
     * Minimum coins needed: 4
     * Coins used: 2 5 10 20
     * Total ways to make change: 169
     *
     * (If amount = 3, coins = {2}):
     * Minimum coins needed: -1
     */

    return 0;
}
