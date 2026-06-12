/*
 * ====================================================================
 * PROGRAM 216: Fibonacci_DP.cpp
 * TOPIC    : Dynamic Programming - Fibonacci
 * ====================================================================
 * THEORY:
 * Fibonacci numbers are defined as: F(0) = 0, F(1) = 1,
 * F(n) = F(n-1) + F(n-2) for n >= 2.
 *
 * DYNAMIC PROGRAMMING APPROACHES:
 * 1. Top-Down (Memoization) - Recursive with caching O(n), O(n) stack
 * 2. Bottom-Up (Tabulation)  - Iterative with array O(n), O(n)
 * 3. Space-Optimized         - Keep only last two values O(n), O(1)
 *
 * COMPLEXITY:
 * - Naive Recursive: O(2^n) time, O(n) stack space
 * - Memoization:     O(n) time, O(n) space
 * - Tabulation:      O(n) time, O(n) space
 * - Space-Optimized: O(n) time, O(1) space
 *
 * INTERVIEW QUESTIONS:
 * Q: Can Fibonacci be computed in O(log n)?
 * A: Yes, using matrix exponentiation: [[1,1],[1,0]]^n
 *
 * Q: How would you handle large n (e.g., n=100000)?
 * A: Use space-optimized DP with modulo arithmetic or big integers.
 *
 * COMMON MISTAKES:
 * - Off-by-one in DP array size
 * - Forgetting base cases for n=0 and n=1
 * - Stack overflow with deep recursion in naive approach
 * - Integer overflow for large n (use long long or big integers)
 * ====================================================================
 */

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// ---------- 1. TOP-DOWN MEMOIZATION ----------
long long fib_memo(int n, unordered_map<int, long long>& memo) {
    if (memo.count(n)) return memo[n];
    if (n <= 1) return n;
    memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    return memo[n];
}

long long fib_top_down(int n) {
    unordered_map<int, long long> memo;
    return fib_memo(n, memo);
}

// ---------- 2. BOTTOM-UP TABULATION ----------
long long fib_bottom_up(int n) {
    if (n <= 1) return n;
    vector<long long> dp(n + 1);
    dp[0] = 0; dp[1] = 1;
    for (int i = 2; i <= n; ++i)
        dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}

// ---------- 3. SPACE-OPTIMIZED O(1) ----------
long long fib_optimized(int n) {
    if (n <= 1) return n;
    long long prev2 = 0, prev1 = 1;
    for (int i = 2; i <= n; ++i) {
        long long cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

// ---------- MAIN ----------
int main() {
    cout << "=== FIBONACCI (DP) ===\n\n";

    int n;
    cout << "Enter n: ";
    cin >> n;

    cout << "\nTop-Down (Memoization):  " << fib_top_down(n);
    cout << "\nBottom-Up (Tabulation): " << fib_bottom_up(n);
    cout << "\nSpace-Optimized O(1):   " << fib_optimized(n);
    cout << "\n\n--- First " << min(n + 1, 20) << " Fibonacci numbers ---\n";
    for (int i = 0; i <= min(n, 19); ++i)
        cout << "F(" << i << ") = " << fib_optimized(i) << "\n";

    /*
     * SAMPLE I/O:
     * Enter n: 10
     * Top-Down (Memoization):  55
     * Bottom-Up (Tabulation): 55
     * Space-Optimized O(1):   55
     *
     * First 11 Fibonacci numbers:
     * F(0) = 0
     * F(1) = 1
     * F(2) = 1
     * F(3) = 2
     * ...
     * F(10) = 55
     */

    return 0;
}
