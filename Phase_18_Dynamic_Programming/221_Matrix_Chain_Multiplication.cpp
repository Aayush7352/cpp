/*
 * ====================================================================
 * PROGRAM 221: Matrix_Chain_Multiplication.cpp
 * TOPIC    : Dynamic Programming - Matrix Chain Multiplication
 * ====================================================================
 * THEORY:
 * Given matrices A1, A2, ..., An with dimensions p0×p1, p1×p2, ...
 * Find optimal parenthesization minimizing scalar multiplications.
 *
 * DP FORMULATION:
 *   DP[i][j] = min scalar mults to multiply matrices i..j
 *   DP[i][j] = min(DP[i][k] + DP[k+1][j] + p[i-1]*p[k]*p[j])
 *   for k in [i, j-1]
 *   Base: DP[i][i] = 0
 *
 * PRINT OPTIMAL ORDER:
 *   Store split point k[i][j] and recursively print parentheses.
 *
 * COMPLEXITY:
 *   Time: O(n^3), Space: O(n^2)
 *
 * INTERVIEW QUESTIONS:
 * Q: Can this be solved greedily?
 * A: No, greedy doesn't work; DP is required.
 *
 * Q: How to handle large n?
 * A: Use Hu-Shing algorithm O(n log n), or modify DP order.
 *
 * Q: What if some matrices are square?
 * A: Same algorithm applies, p[i-1] == p[i] for square.
 *
 * COMMON MISTAKES:
 * - Off-by-one in dimensions indexing (p[i-1]*p[k]*p[j])
 * - Forgetting DP[i][i] = 0
 * - Not using long long for large numbers
 * - Wrong iteration order (need increasing chain length)
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <climits>
#include <string>

using namespace std;

// ---------- MATRIX CHAIN MULTIPLICATION ----------
pair<long long, string> matrix_chain_order(const vector<int>& p) {
    int n = p.size() - 1; // number of matrices
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    vector<vector<int>> split(n + 1, vector<int>(n + 1, 0));

    // l = chain length
    for (int len = 2; len <= n; ++len) {
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            dp[i][j] = LLONG_MAX;
            for (int k = i; k < j; ++k) {
                long long cost = dp[i][k] + dp[k + 1][j]
                               + (long long)p[i - 1] * p[k] * p[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }

    // Build the parenthesization string recursively
    function<string(int,int)> build = [&](int i, int j) -> string {
        if (i == j) return "A" + to_string(i);
        int k = split[i][j];
        return "(" + build(i, k) + " x " + build(k + 1, j) + ")";
    };

    return {dp[1][n], build(1, n)};
}

// ---------- MAIN ----------
int main() {
    cout << "=== MATRIX CHAIN MULTIPLICATION ===\n\n";

    // Matrices: A1(10×30), A2(30×5), A3(5×60)
    vector<int> p = {10, 30, 5, 60};

    cout << "Matrix dimensions:\n";
    for (size_t i = 1; i < p.size(); ++i)
        cout << "  A" << i << ": " << p[i - 1] << "x" << p[i] << "\n";

    auto [min_cost, order] = matrix_chain_order(p);
    cout << "\nMinimum scalar multiplications: " << min_cost << "\n";
    cout << "Optimal order: " << order << "\n";

    /*
     * SAMPLE I/O:
     * Matrix dimensions:
     *   A1: 10x30
     *   A2: 30x5
     *   A3: 5x60
     *
     * Minimum scalar multiplications: 4500
     * Optimal order: (A1 x (A2 x A3))
     *
     * Explanation:
     *   (A1 x A2) x A3: 10*30*5 + 10*5*60 = 1500 + 3000 = 4500
     *   A1 x (A2 x A3): 30*5*60 + 10*30*60 = 9000 + 18000 = 27000
     *   Optimal = 4500
     */

    return 0;
}
