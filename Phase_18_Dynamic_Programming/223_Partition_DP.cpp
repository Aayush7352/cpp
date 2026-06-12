/*
 * ====================================================================
 * PROGRAM 223: Partition_DP.cpp
 * TOPIC    : Dynamic Programming - Partition Problems
 * ====================================================================
 * THEORY:
 * 1. PARTITION EQUAL SUBSET SUM:
 *    Can the array be partitioned into two subsets with equal sum?
 *    If total sum S is odd, impossible.
 *    Otherwise, find subset with sum S/2 (0/1 knapsack).
 *    DP[i] = true if subset sum i is achievable.
 *
 * 2. PALINDROME PARTITIONING:
 *    Given string S, partition it such that every substring is a
 *    palindrome. Find minimum cuts needed.
 *    DP[i] = min cuts for prefix S[0..i-1]
 *    Precompute isPal[i][j] = whether S[i..j] is palindrome.
 *    DP[i] = min(DP[j] + 1) for j < i and isPal[j][i-1] true.
 *
 * BACKTRACKING:
 *    For palindrome partitioning, also reconstruct the partitions.
 *
 * COMPLEXITY:
 *   Equal Subset Sum: O(n * sum/2) time, O(sum/2) space
 *   Palindrome Partition: O(n^2) time, O(n^2) / O(n) space
 *
 * INTERVIEW QUESTIONS:
 * Q: Can equal subset sum be done with negative numbers?
 * A: Then total sum variable, need different approach.
 *
 * Q: Minimum palindromic cuts vs print all partitions?
 * A: Minimum = DP; all partitions = backtracking with DFS.
 *
 * COMMON MISTAKES:
 * - Not checking if total sum is odd for equal partition
 * - Off-by-one in palindrome DP initialization
 * - Forgetting single characters are palindromes (isPal[i][i] = true)
 * - Not handling empty strings or zero-length arrays
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <climits>

using namespace std;

// ---------- PARTITION EQUAL SUBSET SUM ----------
bool canPartition(const vector<int>& nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    if (total % 2 != 0) return false;
    int target = total / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;
    for (int num : nums) {
        for (int i = target; i >= num; --i)
            if (dp[i - num]) dp[i] = true;
    }
    return dp[target];
}

// ---------- MINIMUM PALINDROME PARTITIONING ----------
int minPalindromicCuts(const string& s) {
    int n = s.size();
    if (n <= 1) return 0;
    vector<vector<bool>> isPal(n, vector<bool>(n, false));
    for (int i = 0; i < n; ++i) isPal[i][i] = true;
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && (len == 2 || isPal[i + 1][j - 1]))
                isPal[i][j] = true;
        }
    }
    vector<int> dp(n, INT_MAX);
    for (int i = 0; i < n; ++i) {
        if (isPal[0][i]) {
            dp[i] = 0;
        } else {
            for (int j = 0; j < i; ++j) {
                if (isPal[j + 1][i] && dp[j] + 1 < dp[i])
                    dp[i] = dp[j] + 1;
            }
        }
    }
    return dp[n - 1];
}

// ---------- PRINT ALL PALINDROME PARTITIONS (BACKTRACKING) ----------
void backtrack(const string& s, int start, vector<string>& cur,
               vector<vector<string>>& result, const vector<vector<bool>>& isPal) {
    if (start >= (int)s.size()) {
        result.push_back(cur);
        return;
    }
    for (int end = start; end < (int)s.size(); ++end) {
        if (isPal[start][end]) {
            cur.push_back(s.substr(start, end - start + 1));
            backtrack(s, end + 1, cur, result, isPal);
            cur.pop_back();
        }
    }
}

void printAllPalPartitions(const string& s) {
    int n = s.size();
    vector<vector<bool>> isPal(n, vector<bool>(n, false));
    for (int i = 0; i < n; ++i) isPal[i][i] = true;
    for (int len = 2; len <= n; ++len)
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && (len == 2 || isPal[i + 1][j - 1]))
                isPal[i][j] = true;
        }
    vector<vector<string>> partitions;
    vector<string> cur;
    backtrack(s, 0, cur, partitions, isPal);
    cout << "All palindrome partitions:\n";
    for (auto& p : partitions) {
        cout << "  [";
        for (size_t i = 0; i < p.size(); ++i)
            cout << (i ? ", " : "") << "\"" << p[i] << "\"";
        cout << "]\n";
    }
}

// ---------- MAIN ----------
int main() {
    cout << "=== PARTITION DP ===\n\n";

    // Equal subset sum
    vector<int> nums = {1, 5, 11, 5};
    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << "\nEqual partition possible? " << (canPartition(nums) ? "Yes" : "No") << "\n\n";

    // Palindrome partitioning
    string s = "aab";
    cout << "String: \"" << s << "\"\n";
    cout << "Minimum palindromic cuts: " << minPalindromicCuts(s) << "\n";
    printAllPalPartitions(s);

    /*
     * SAMPLE I/O:
     * Array: 1 5 11 5
     * Equal partition possible? Yes
     * (Subsets: {1,5,5} and {11})
     *
     * String: "aab"
     * Minimum palindromic cuts: 1
     * All palindrome partitions:
     *   ["a", "a", "b"]
     *   ["aa", "b"]
     */

    return 0;
}
