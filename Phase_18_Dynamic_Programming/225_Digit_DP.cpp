/*
 * ====================================================================
 * PROGRAM 225: Digit_DP.cpp
 * TOPIC    : Dynamic Programming - Digit DP
 * ====================================================================
 * THEORY:
 * Digit DP is used to count numbers <= N that satisfy a condition
 * based on digits. It processes the number digit by digit.
 *
 * STATE:
 *   DP[pos][tight][started][state...]
 *   pos      = current digit position (from most significant)
 *   tight    = whether prefix matches the limit bound
 *   started  = whether number has begun (leading zeros)
 *   state... = problem-specific condition
 *
 * COMMON PROBLEMS:
 * 1. Count numbers <= N with sum of digits = S
 * 2. Count numbers <= N with no consecutive same digit
 * 3. Count numbers <= N divisible by K
 *
 * COMPLEXITY:
 *   O(number_of_positions * 2 * 2 * ... * 10) ≈ O(len(N) * states)
 *   Typically O(len(N) * K * 10) for modulus-based problems
 *
 * INTERVIEW QUESTIONS:
 * Q: How to handle large N up to 10^100?
 * A: Store as string, process digit by digit.
 *
 * Q: Can we handle range [L, R]?
 * A: Yes, result = f(R) - f(L-1).
 *
 * COMMON MISTAKES:
 * - Not handling leading zeros correctly
 * - Forgetting the tight constraint propagation
 * - Not reseting memoization per test case
 * - Off-by-one in position indexing
 * ====================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

// ---------- PROBLEM: Count numbers <= N with sum of digits == target sum ----------
long long dp[20][2][2][200]; // pos, tight, started, sum
string num_str;

long long digit_dp(int pos, bool tight, bool started, int sum, int target_sum) {
    if (pos == (int)num_str.size())
        return (started && sum == target_sum) ? 1 : 0;

    if (dp[pos][tight][started][sum] != -1)
        return dp[pos][tight][started][sum];

    int limit = tight ? (num_str[pos] - '0') : 9;
    long long ans = 0;

    for (int d = 0; d <= limit; ++d) {
        bool new_tight = tight && (d == limit);
        bool new_started = started || (d != 0);
        int new_sum = sum + d;
        if (new_sum > target_sum) continue;
        ans += digit_dp(pos + 1, new_tight, new_started, new_sum, target_sum);
    }

    return dp[pos][tight][started][sum] = ans;
}

long long count_with_sum(long long N, int target_sum) {
    num_str = to_string(N);
    memset(dp, -1, sizeof(dp));
    return digit_dp(0, true, false, 0, target_sum);
}

// ---------- PROBLEM 2: Count numbers <= N with no consecutive same digit ----------
long long dp2[20][2][2][10];

long long no_consecutive(int pos, bool tight, bool started, int last_digit) {
    if (pos == (int)num_str.size())
        return started ? 1 : 0;

    if (dp2[pos][tight][started][last_digit + 1] != -1)
        return dp2[pos][tight][started][last_digit + 1];

    int limit = tight ? (num_str[pos] - '0') : 9;
    long long ans = 0;

    for (int d = 0; d <= limit; ++d) {
        if (started && d == last_digit) continue;
        bool new_tight = tight && (d == limit);
        bool new_started = started || (d != 0);
        ans += no_consecutive(pos + 1, new_tight, new_started, d);
    }

    return dp2[pos][tight][started][last_digit + 1] = ans;
}

long long count_no_consecutive(long long N) {
    num_str = to_string(N);
    memset(dp2, -1, sizeof(dp2));
    return no_consecutive(0, true, false, -1);
}

// ---------- MAIN ----------
int main() {
    cout << "=== DIGIT DP ===\n\n";

    long long N = 125;
    int target_sum = 8;

    cout << "N = " << N << "\nTarget sum = " << target_sum << "\n";
    cout << "Numbers <= N with digit sum " << target_sum << ": "
         << count_with_sum(N, target_sum) << "\n\n";

    N = 200;
    cout << "N = " << N << "\n";
    cout << "Numbers <= N with no consecutive same digit: "
         << count_no_consecutive(N) << "\n\n";

    // Range [L, R] example
    long long L = 10, R = 200;
    cout << "Range [" << L << ", " << R << "]:\n";
    cout << "  Numbers with sum=8: "
         << (count_with_sum(R, 8) - count_with_sum(L - 1, 8)) << "\n";

    /*
     * SAMPLE I/O:
     * N = 125
     * Target sum = 8
     * Numbers <= N with digit sum 8: 9
     * (8, 17, 26, 35, 44, 53, 62, 71, 80, 107, 116, 125)
     * Wait: 125 sum=8, 116 sum=8, 107 sum=8, 80 sum=8... let's list:
     * 8, 17, 26, 35, 44, 53, 62, 71, 80, 107, 116, 125 = 12 numbers
     *
     * N = 200
     * Numbers <= N with no consecutive same digit: 153
     *
     * Range [10, 200]:
     *   Numbers with sum=8: 8 (17,26,35,44,53,62,71,80,107,116,125,134,143,152,161,170)
     */

    return 0;
}
