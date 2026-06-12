/*
 * ====================================================================
 * PROGRAM 219: LCS.cpp
 * TOPIC    : Dynamic Programming - Longest Common Subsequence
 * ====================================================================
 * THEORY:
 * LONGEST COMMON SUBSEQUENCE (LCS):
 *   DP[i][j] = length of LCS of X[0..i-1] and Y[0..j-1]
 *
 *   DP[i][j] = DP[i-1][j-1] + 1              if X[i-1] == Y[j-1]
 *            = max(DP[i-1][j], DP[i][j-1])   otherwise
 *
 * PRINT LCS:
 *   Backtrack through DP table to reconstruct the subsequence.
 *
 * SPACE OPTIMIZED LCS:
 *   Only keep two rows (current and previous) O(min(n,m)) space.
 *
 * LONGEST COMMON SUBSTRING:
 *   Continuity required. DP[i][j] = DP[i-1][j-1] + 1 if match, else 0.
 *
 * COMPLEXITY:
 *   LCS: O(n*m) time, O(n*m) / O(min(n,m)) space
 *   Longest Common Substring: O(n*m) time, O(1) space (sliding)
 *
 * INTERVIEW QUESTIONS:
 * Q: How to find LCS of 3 strings?
 * A: Extend DP to 3D - DP[i][j][k].
 *
 * Q: Can we do better than O(n*m)?
 * A: O((n+m)*log(n+m)) possible for LCS with repeating characters.
 *
 * COMMON MISTAKES:
 * - Off-by-one in indices (1-indexed vs 0-indexed)
 * - Forgetting base case DP[0][j] = DP[i][0] = 0
 * - Printing LCS without handling empty cases
 * - Confusing substring and subsequence
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ---------- LCS LENGTH ----------
int lcs_length(const string& X, const string& Y) {
    int n = X.size(), m = Y.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[n][m];
}

// ---------- PRINT LCS ----------
string lcs_print(const string& X, const string& Y) {
    int n = X.size(), m = Y.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);

    string lcs;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs.push_back(X[i - 1]);
            --i; --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
}

// ---------- SPACE OPTIMIZED LCS ----------
int lcs_space_optimized(const string& X, const string& Y) {
    int n = X.size(), m = Y.size();
    vector<int> prev(m + 1, 0), cur(m + 1, 0);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (X[i - 1] == Y[j - 1])
                cur[j] = prev[j - 1] + 1;
            else
                cur[j] = max(prev[j], cur[j - 1]);
        }
        swap(prev, cur);
    }
    return prev[m];
}

// ---------- LONGEST COMMON SUBSTRING ----------
int longest_common_substring(const string& X, const string& Y) {
    int n = X.size(), m = Y.size();
    vector<int> prev(m + 1, 0), cur(m + 1, 0);
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (X[i - 1] == Y[j - 1]) {
                cur[j] = prev[j - 1] + 1;
                ans = max(ans, cur[j]);
            } else {
                cur[j] = 0;
            }
        }
        swap(prev, cur);
    }
    return ans;
}

// ---------- MAIN ----------
int main() {
    cout << "=== LONGEST COMMON SUBSEQUENCE ===\n\n";

    string X = "ABCDGH";
    string Y = "AEDFHR";

    cout << "X = " << X << "\nY = " << Y << "\n\n";

    cout << "LCS Length: " << lcs_length(X, Y) << "\n";
    cout << "LCS String: " << lcs_print(X, Y) << "\n";
    cout << "LCS (Space Opt): " << lcs_space_optimized(X, Y) << "\n";
    cout << "Longest Common Substring: " << longest_common_substring(X, Y) << "\n";

    /*
     * SAMPLE I/O:
     * X = ABC DGH
     * Y = AEDFHR
     *
     * LCS Length: 3
     * LCS String: ADH
     * LCS (Space Opt): 3
     * Longest Common Substring: 2 ("DH"? Actually no - no common substring >1 with continuity)
     * Let's verify: X=ABCDGH, Y=AEDFHR
     * Common chars: A, D, H - but not contiguous in both
     * Actually "ADH" is subsequence, not substring.
     * Longest Common Substring: 1 (any single matching char)
     */

    return 0;
}
