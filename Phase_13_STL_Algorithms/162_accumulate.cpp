/*
==================================================================
162. std::accumulate, inner_product, partial_sum, reduce
==================================================================
Theory:
- accumulate (numeric): sum/product over range, left fold
- inner_product: dot product of two ranges
- partial_sum: write cumulative sums to output
- reduce (C++17): parallelizable version of accumulate
  (order not guaranteed, associative + commutative op)
- All in <numeric>
*/

#include <iostream>
#include <numeric>
#include <vector>
#include <execution> // for std::reduce (C++17 parallel)

int main() {
    std::cout << "=== accumulate, inner_product, partial_sum, reduce\n\n";

    // --- accumulate ---
    std::cout << "1. std::accumulate:\n";
    std::vector<int> v{1, 2, 3, 4, 5};
    int sum = std::accumulate(v.begin(), v.end(), 0);
    int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
    std::cout << "   Sum: " << sum << "\n";
    std::cout << "   Product: " << product << "\n";
    std::cout << "   Concatenate (string): ";

    // accumulate with strings
    std::vector<std::string> words{"Hello", " ", "World", "!"};
    std::string concat = std::accumulate(words.begin(), words.end(), std::string());
    std::cout << "\"" << concat << "\"\n\n";

    // --- inner_product ---
    std::cout << "2. std::inner_product (dot product):\n";
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 6};
    int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::cout << "   [1,2,3] . [4,5,6] = " << dot << "\n";
    // Custom: sum of squared differences
    int sumSqDiff = std::inner_product(a.begin(), a.end(), b.begin(), 0,
        std::plus<int>(),
        [](int x, int y) { return (x - y) * (x - y); });
    std::cout << "   Sum of squared diffs: " << sumSqDiff << "\n\n";

    // --- partial_sum ---
    std::cout << "3. std::partial_sum:\n";
    std::vector<int> ps(v.size());
    std::partial_sum(v.begin(), v.end(), ps.begin());
    std::cout << "   Original: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n   Partial sum: ";
    for (int x : ps) std::cout << x << " ";
    std::cout << "\n\n";

    // --- reduce (C++17, parallel) ---
    std::cout << "4. std::reduce (C++17):\n";
    std::vector<int> big(1000);
    std::iota(big.begin(), big.end(), 1);
    // Sequential reduce
    int red = std::reduce(big.begin(), big.end(), 0);
    std::cout << "   Reduce sum 1..1000: " << red << "\n";

    // Parallel reduce
    int parRed = std::reduce(std::execution::par, big.begin(), big.end(), 0);
    std::cout << "   Parallel reduce: " << parRed << "\n";

    // --- inclusive_scan (C++17) ---
    std::cout << "\n5. std::inclusive_scan:\n";
    std::vector<int> scan(v.size());
    std::inclusive_scan(v.begin(), v.end(), scan.begin());
    std::cout << "   Inclusive scan: ";
    for (int x : scan) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. std::accumulate:
   Sum: 15
   Product: 120
   Concatenate (string): "Hello World!"

2. std::inner_product (dot product):
   [1,2,3] . [4,5,6] = 32
   Sum of squared diffs: 27

3. std::partial_sum:
   Original: 1 2 3 4 5
   Partial sum: 1 3 6 10 15

4. std::reduce (C++17):
   Reduce sum 1..1000: 500500
   Parallel reduce: 500500

5. std::inclusive_scan:
   Inclusive scan: 1 3 6 10 15
==================================================================
Complexity:
- accumulate: O(n)
- inner_product: O(n)
- partial_sum: O(n)
- reduce: O(n) (parallel speedup hardware-dependent)
==================================================================
Interview Questions:
1. accumulate vs reduce?
   -> accumulate: left fold, deterministic order.
      reduce: parallelizable, non-deterministic fold order,
      requires associative + commutative op.

2. What happens if the range is empty?
   -> accumulate returns the initial value.
      reduce returns the initial value.

3. Can accumulate work with non-numeric types?
   -> Yes, with custom binary op, e.g., concatenating strings.
==================================================================
Common Mistakes:
- Not providing initial value (bad, leads to UB if empty)
- Forgetting <numeric> header
- Using reduce with non-associative operation (wrong results)
- Confusing partial_sum with adjacent_difference
- Not reserving output container for partial_sum
==================================================================
*/
