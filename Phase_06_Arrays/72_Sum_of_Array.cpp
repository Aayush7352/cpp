/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Sum of Array Elements                   ║
* ║  Program 72 — Manual sum, std::accumulate, parallel policy ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Summation is O(n). The naïve loop accumulates into a running total.
* - std::accumulate (from <numeric>) works on any range and accepts
*   an initial value and optional binary op. Default is operator+.
* - With C++17 Execution Policies (C++17, <execution>) you can request
*   parallel execution: std::execution::par (or par_unseq). This splits
*   the work across threads — beneficial for large arrays.
* - Parallel accumulate is NOT guaranteed to be faster for small arrays
*   due to overhead of thread creation / synchronisation.
*
* SAMPLE I/O:
*   Array: [1 3 5 7 9]
*   Manual sum    = 25
*   Accumulate    = 25
*   Parallel      = 25
*
* COMPLEXITY:
*   Time:  O(n) sequential; O(n / P) with sufficient parallel hardware.
*   Space: O(1) sequential; O(P) for parallel bookkeeping.
*
* INTERVIEW QUESTIONS:
* 1. Q: What is the danger of integer overflow in sum?
*    A: Signed overflow is UB. Use a wider type (long long) or check
*       against numeric_limits<T>::max() before adding.
* 2. Q: When does parallel std::accumulate help?
*    A: For large arrays (10⁵+ elements) on multi-core systems where
*       the cost of spawning threads is amortised.
*
* COMMON MISTAKES:
* - Forgetting <numeric> for std::accumulate.
* - Passing wrong initial value type (e.g. 0 instead of 0LL for large sums).
* - Using parallel execution on very small arrays — slower, not faster.
*/

#include <iostream>
#include <numeric>
#include <array>
#include <chrono>
#include <vector>

auto now() { return std::chrono::steady_clock::now(); }

int main() {
    constexpr std::array arr{1LL, 3LL, 5LL, 7LL, 9LL};

    // ── 1. Manual loop ───────────────────────────────────────
    long long sum1{0};
    for (auto x : arr) sum1 += x;
    std::cout << "Manual sum    = " << sum1 << '\n';

    // ── 2. std::accumulate (sequential, <numeric>) ──────────
    auto sum2 = std::accumulate(arr.begin(), arr.end(), 0LL);
    std::cout << "std::accumulate = " << sum2 << '\n';

    // ── 3. Sequential reduce (C++17 <numeric>, parallel not available on all compilers) ──
    auto sum3 = std::reduce(arr.begin(), arr.end(), 0LL);
    std::cout << "Seq. reduce  = " << sum3 << '\n';

    // ── Benchmark (optional, illustrative) ───────────────────
    constexpr std::size_t big_n = 10'000'000;
    std::vector<long long> big(big_n);
    std::iota(big.begin(), big.end(), 1LL);   // 1, 2, 3, …

    auto t1 = now();
    auto s1 = std::accumulate(big.begin(), big.end(), 0LL);
    auto t2 = now();

    auto t3 = now();
    auto s2 = std::reduce(big.begin(), big.end(), 0LL);
    auto t4 = now();

    std::cout << "\nBenchmark for " << big_n << " elements:\n"
              << "  Accumulate: " << s1
              << "  (" << std::chrono::duration<double>(t2-t1).count() << "s)\n"
              << "  Reduce    : " << s2
              << "  (" << std::chrono::duration<double>(t4-t3).count() << "s)\n"
              << "  (Use std::execution::par for parallel where supported)\n";

    return 0;
}
