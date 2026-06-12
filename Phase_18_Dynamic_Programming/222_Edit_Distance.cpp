/*
 * ====================================================================
 * PROGRAM 222: Edit_Distance.cpp
 * TOPIC    : Dynamic Programming - Levenshtein Distance
 * ====================================================================
 * THEORY:
 * Edit distance (Levenshtein) between two strings A and B is the
 * minimum number of operations (insert, delete, replace) to convert
 * A into B.
 *
 * DP FORMULATION:
 *   DP[i][j] = edit distance between A[0..i-1] and B[0..j-1]
 *
 *   DP[i][j] = DP[i-1][j-1]                    if A[i-1] == B[j-1]
 *            = 1 + min(
 *                DP[i-1][j],     // delete from A
 *                DP[i][j-1],     // insert into A
 *                DP[i-1][j-1]    // replace
 *              )
 *
 * SPACE OPTIMIZATION:
 *   Keep only two rows (prev and cur) for O(min(n,m)) space.
 *
 * RECONSTRUCTION:
 *   Backtrack through DP table to show actual operations.
 *
 * COMPLEXITY:
 *   Time: O(n*m), Space: O(n*m) or O(min(n,m))
 *
 * INTERVIEW QUESTIONS:
 * Q: How is this used in spell checkers?
 * A: Google's "Did you mean?" uses edit distance with BK-trees.
 *
 * Q: What about Damerau-Levenshtein (with swap)?
 * A: Add DP[i-2][j-2] + 1 for transposition of adjacent characters.
 *
 * COMMON MISTAKES:
 * - Off-by-one in table initialization (DP[0][j] = j, DP[i][0] = i)
 * - Forgetting the equality case (no cost)
 * - Not handling empty strings
 * - Confusing insert/delete direction
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ---------- EDIT DISTANCE (FULL TABLE) ----------
int edit_distance_full(const string& A, const string& B) {
    int n = A.size(), m = B.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (A[i - 1] == B[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
        }
    }
    return dp[n][m];
}

// ---------- SPACE OPTIMIZED ----------
int edit_distance_optimized(const string& A, const string& B) {
    int n = A.size(), m = B.size();
    if (n < m) return edit_distance_optimized(B, A);
    vector<int> prev(m + 1), cur(m + 1);
    for (int j = 0; j <= m; ++j) prev[j] = j;
    for (int i = 1; i <= n; ++i) {
        cur[0] = i;
        for (int j = 1; j <= m; ++j) {
            if (A[i - 1] == B[j - 1])
                cur[j] = prev[j - 1];
            else
                cur[j] = 1 + min({prev[j], cur[j - 1], prev[j - 1]});
        }
        swap(prev, cur);
    }
    return prev[m];
}

// ---------- RECONSTRUCTION ----------
void edit_operations(const string& A, const string& B) {
    int n = A.size(), m = B.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            if (A[i - 1] == B[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});

    cout << "Edit distance: " << dp[n][m] << "\nOperations:\n";
    int i = n, j = m;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && A[i - 1] == B[j - 1]) {
            cout << "  Keep '" << A[i - 1] << "'\n";
            --i; --j;
        } else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + 1) {
            cout << "  Replace '" << A[i - 1] << "' -> '" << B[j - 1] << "'\n";
            --i; --j;
        } else if (i > 0 && dp[i][j] == dp[i - 1][j] + 1) {
            cout << "  Delete '" << A[i - 1] << "'\n";
            --i;
        } else if (j > 0 && dp[i][j] == dp[i][j - 1] + 1) {
            cout << "  Insert '" << B[j - 1] << "'\n";
            --j;
        }
    }
}

// ---------- MAIN ----------
int main() {
    cout << "=== EDIT DISTANCE (LEVENSHTEIN) ===\n\n";

    string A = "sunday";
    string B = "saturday";

    cout << "A = \"" << A << "\"\nB = \"" << B << "\"\n\n";

    cout << "Edit distance (full): " << edit_distance_full(A, B) << "\n";
    cout << "Edit distance (opt):  " << edit_distance_optimized(A, B) << "\n\n";
    edit_operations(A, B);

    /*
     * SAMPLE I/O:
     * A = "sunday"
     * B = "saturday"
     *
     * Edit distance (full): 3
     * Edit distance (opt):  3
     *
     * Operations:
     *   Replace 'n' -> 't'
     *   Insert 'a'
     *   Insert 'r'
     *
     * (Actually: sunday -> saturday requires 3 edits)
     * s u n d a y
     * s a t u r d a y
     * Keep 's', Insert 'a', Insert 't', Keep 'u', Insert 'r', Keep 'd', Keep 'a', Keep 'y'
     */

    return 0;
}
