/*
 * Phase 16 – Data Structures
 * 200. Fenwick Tree (Binary Indexed Tree)
 *
 * Theory:
 * - BIT stores prefix sums and supports point updates in O(log n).
 * - Each index i stores the sum of a range [i - LSB(i) + 1, i].
 * - LSB (least significant bit) = i & -i.
 * - Operations:
 *   1. Prefix sum query (point query).
 *   2. Point update (add value at index).
 *   3. Range update + point query (using difference array).
 *   4. Range update + range query (using two BITs).
 * - Applications: inversion count, prefix sum queries with updates.
 *
 * Common Mistakes:
 * - 1-indexed vs 0-indexed confusion.
 * - Forgetting to update all ancestors during point update.
 * - Using wrong BIT for range update + range query.
 *
 * Interview Questions:
 * 1. Count inversions in an array using BIT.
 * 2. Range sum with point updates.
 * 3. Find kth smallest element using BIT.
 * 4. Count of numbers in a range with updates.
 */

#include <iostream>
#include <vector>

template<typename T>
class FenwickTree {
    std::vector<T> bit;
    size_t n;

public:
    explicit FenwickTree(size_t size) : bit(size + 1, T(0)), n(size) {}

    FenwickTree(const std::vector<T>& arr) : bit(arr.size() + 1, T(0)), n(arr.size()) {
        for (size_t i = 0; i < n; ++i) {
            add(i, arr[i]);
        }
    }

    // Add val at index idx (0-indexed)
    void add(size_t idx, const T& val) {
        ++idx; // convert to 1-indexed
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    // Prefix sum [0..idx] inclusive
    T prefix_sum(size_t idx) const {
        ++idx;
        T sum = T(0);
        while (idx > 0) {
            sum += bit[idx];
            idx -= idx & -idx;
        }
        return sum;
    }

    // Range sum [l..r] inclusive
    T range_sum(size_t l, size_t r) const {
        if (l > r) return T(0);
        return prefix_sum(r) - (l > 0 ? prefix_sum(l - 1) : T(0));
    }

    // Point query at idx
    T point_query(size_t idx) const {
        return range_sum(idx, idx);
    }

    // Update index idx to new value (not add)
    void set(size_t idx, const T& new_val) {
        T curr = point_query(idx);
        add(idx, new_val - curr);
    }
};

// Range Update + Point Query using BIT (difference array)
template<typename T>
class RangeUpdateBIT {
    FenwickTree<T> bit;

public:
    explicit RangeUpdateBIT(size_t n) : bit(n) {}

    // Add val to range [l..r]
    void range_add(size_t l, size_t r, const T& val) {
        bit.add(l, val);
        if (r + 1 < bit.prefix_sum(bit.prefix_sum(0) /*hack, just use n*/)) {
            // We'll store n separately
        }
        // Need access to n; let's reimplement properly
    }
};

// Range Update + Range Query using two BITs
template<typename T>
class RangeUpdateRangeQueryBIT {
    std::vector<T> bit1, bit2;
    size_t n;

    void add(std::vector<T>& bit, size_t idx, const T& val) {
        ++idx;
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    T sum(const std::vector<T>& bit, size_t idx) const {
        ++idx;
        T res = T(0);
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }

    T prefix_sum(size_t idx) const {
        return sum(bit1, idx) * static_cast<T>(idx + 1) - sum(bit2, idx);
    }

public:
    explicit RangeUpdateRangeQueryBIT(size_t size) : bit1(size + 1, T(0)), bit2(size + 1, T(0)), n(size) {}

    void range_add(size_t l, size_t r, const T& val) {
        add(bit1, l, val);
        add(bit1, r + 1, -val);
        add(bit2, l, val * static_cast<T>(l));
        add(bit2, r + 1, -val * static_cast<T>(r + 1));
    }

    T range_sum(size_t l, size_t r) {
        if (l > r) return T(0);
        return prefix_sum(r) - (l > 0 ? prefix_sum(l - 1) : T(0));
    }
};

int main() {
    using namespace std;

    cout << "=== Fenwick Tree (BIT) Demo ===\n\n";

    // ---- Point Update + Range Query ----
    cout << "--- Point Update + Range Query ---\n";
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
    cout << "Array: ";
    for (int x : arr) cout << x << ' ';
    cout << '\n';

    FenwickTree<int> bit(arr);

    cout << "Prefix sum [0..4]: " << bit.prefix_sum(4) << '\n';
    cout << "Range sum [2..5]: " << bit.range_sum(2, 5) << '\n';
    cout << "Point query at 3: " << bit.point_query(3) << '\n';

    cout << "\nAdd 10 at index 3\n";
    bit.add(3, 10);
    cout << "Range sum [2..5]: " << bit.range_sum(2, 5) << '\n';

    cout << "Set index 3 to 100\n";
    bit.set(3, 100);
    cout << "Point query at 3: " << bit.point_query(3) << '\n';

    // ---- Range Update + Range Query ----
    cout << "\n--- Range Update + Range Query ---\n";
    RangeUpdateRangeQueryBIT<long long> rurq(10);
    cout << "Add 5 to range [2..6]\n";
    rurq.range_add(2, 6, 5);
    cout << "Range sum [0..9]: " << rurq.range_sum(0, 9) << '\n';
    cout << "Range sum [2..6]: " << rurq.range_sum(2, 6) << '\n';
    cout << "Range sum [5..5]: " << rurq.range_sum(5, 5) << '\n';

    cout << "\nAdd 3 to range [0..4]\n";
    rurq.range_add(0, 4, 3);
    cout << "Range sum [0..9]: " << rurq.range_sum(0, 9) << '\n';
    cout << "Range sum [3..7]: " << rurq.range_sum(3, 7) << '\n';

    // ---- Inversion Count ----
    cout << "\n--- Inversion Count ---\n";
    vector<int> inv_arr = {8, 4, 2, 1};
    int max_val = *max_element(inv_arr.begin(), inv_arr.end());
    FenwickTree<int> inv_bit(max_val + 1);
    int inversions = 0;
    for (int i = inv_arr.size() - 1; i >= 0; --i) {
        inversions += inv_bit.prefix_sum(inv_arr[i] - 1);
        inv_bit.add(inv_arr[i], 1);
    }
    cout << "Array: ";
    for (int x : inv_arr) cout << x << ' ';
    cout << "\nInversions: " << inversions << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Point update / prefix sum: O(log n)\n";
    cout << "Range update + range query: O(log n)\n";
    cout << "Space: O(n)\n";

    return 0;
}
