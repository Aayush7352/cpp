/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 6: ARRAYS — Average (Mean) of Array                 ║
* ║  Program 73 — Arithmetic mean with floating-point precision ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Mean μ = (Σ xᵢ) / n.
* - Floating point: use double (not float) for summation to preserve
*   precision. For very large arrays, a two-pass or Kahan summation
*   reduces error.
* - Integer division truncates — cast to double before division.
*
* SAMPLE I/O:
*   Enter 5 numbers: 10 20 30 40 50
*   Mean = 30.00
*
* COMPLEXITY:
*   Time: O(n) — one pass for sum, O(1) for division.
*   Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: What if the array is empty?
*    A: Division by zero! Check n == 0 and handle gracefully.
* 2. Q: How do you compute the mean without overflow?
*    A: Use a wider accumulator (long double) or process in chunks.
* 3. Q: What is the difference between mean, median, and mode?
*    A: Mean is arithmetic average; median is middle element when sorted;
*       mode is most frequent value.
*
* COMMON MISTAKES:
* - Integer division: `sum / n` when both are ints — cast one to double.
* - Using float instead of double for accumulator — loss of precision.
* - Not handling empty array (division by zero).
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <numeric>

int main() {
    std::cout << "Enter number of elements: ";
    std::size_t n{};
    std::cin >> n;
    if (n == 0) {
        std::cerr << "Error: empty array, cannot compute mean.\n";
        return 1;
    }

    std::vector<double> data(n);
    std::cout << "Enter " << n << " numbers: ";
    for (auto& x : data) std::cin >> x;

    // ── Compute mean ─────────────────────────────────────────
    // Use long double for accumulator to minimise rounding error.
    long double sum = std::accumulate(data.begin(), data.end(), 0.0L);
    double mean = static_cast<double>(sum / static_cast<long double>(n));

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Mean = " << mean << '\n';

    // ── Numeric guard: Kahan summation (illustration) ────────
    // For extreme datasets, Kahan summation reduces error:
    long double kahan_sum{0.0L}, c{0.0L};
    for (auto x : data) {
        long double y = static_cast<long double>(x) - c;
        long double t = kahan_sum + y;
        c = (t - kahan_sum) - y;
        kahan_sum = t;
    }
    std::cout << "Kahan mean = " << (kahan_sum / n) << '\n';

    return 0;
}
