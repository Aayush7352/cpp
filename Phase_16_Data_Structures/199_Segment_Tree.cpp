/*
 * Phase 16 – Data Structures
 * 199. Segment Tree
 *
 * Theory:
 * - Segment tree stores interval/range information in a binary tree.
 * - Leaf nodes = individual array elements; internal nodes = union of children.
 * - Supports range queries (e.g., sum, min, max, gcd) and point updates in O(log n).
 * - Lazy propagation: defers updates to children until needed → O(log n) range updates.
 * - Space: O(4*n) for array-based implementation.
 *
 * Common Mistakes:
 * - Off-by-one errors in tree index calculation (2*i+1, 2*i+2).
 * - Not propagating lazy values during query.
 * - Segment tree out of bounds (need 4*n size).
 * - Integer overflow for sums.
 *
 * Interview Questions:
 * 1. Range minimum query with point updates.
 * 2. Range sum with range updates (lazy propagation).
 * 3. Count of primes in a range with updates.
 * 4. Find first index with prefix sum ≥ x.
 * 5. Number of distinct elements in a range.
 */

#include <iostream>
#include <vector>
#include <span>
#include <algorithm>

template<typename T>
class SegmentTree {
    std::vector<T> tree;
    std::vector<T> lazy;
    size_t n;

    void build(const std::vector<T>& arr, size_t node, size_t l, size_t r) {
        if (l == r) {
            tree[node] = arr[l];
            return;
        }
        size_t mid = l + (r - l) / 2;
        size_t left = 2 * node + 1;
        size_t right = 2 * node + 2;
        build(arr, left, l, mid);
        build(arr, right, mid + 1, r);
        tree[node] = tree[left] + tree[right];
    }

    void apply(size_t node, size_t l, size_t r, const T& val) {
        tree[node] += (r - l + 1) * val;
        lazy[node] += val;
    }

    void push(size_t node, size_t l, size_t r) {
        if (lazy[node] != T(0)) {
            size_t mid = l + (r - l) / 2;
            apply(2 * node + 1, l, mid, lazy[node]);
            apply(2 * node + 2, mid + 1, r, lazy[node]);
            lazy[node] = T(0);
        }
    }

    void point_update(size_t node, size_t l, size_t r, size_t idx, const T& val) {
        if (l == r) {
            tree[node] = val;
            return;
        }
        push(node, l, r);
        size_t mid = l + (r - l) / 2;
        if (idx <= mid)
            point_update(2 * node + 1, l, mid, idx, val);
        else
            point_update(2 * node + 2, mid + 1, r, idx, val);
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    }

    void range_update(size_t node, size_t l, size_t r, size_t ql, size_t qr, const T& val) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(node, l, r, val);
            return;
        }
        push(node, l, r);
        size_t mid = l + (r - l) / 2;
        range_update(2 * node + 1, l, mid, ql, qr, val);
        range_update(2 * node + 2, mid + 1, r, ql, qr, val);
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    }

    T range_query(size_t node, size_t l, size_t r, size_t ql, size_t qr) {
        if (ql > r || qr < l) return T(0);
        if (ql <= l && r <= qr) return tree[node];
        push(node, l, r);
        size_t mid = l + (r - l) / 2;
        return range_query(2 * node + 1, l, mid, ql, qr) +
               range_query(2 * node + 2, mid + 1, r, ql, qr);
    }

public:
    explicit SegmentTree(const std::vector<T>& arr) : n(arr.size()) {
        tree.resize(4 * n);
        lazy.resize(4 * n, T(0));
        build(arr, 0, 0, n - 1);
    }

    void point_update(size_t idx, const T& val) {
        point_update(0, 0, n - 1, idx, val);
    }

    void range_update(size_t l, size_t r, const T& val) {
        range_update(0, 0, n - 1, l, r, val);
    }

    T range_sum(size_t l, size_t r) {
        return range_query(0, 0, n - 1, l, r);
    }
};

int main() {
    using namespace std;

    cout << "=== Segment Tree Demo (with Lazy Propagation) ===\n\n";

    vector<int> arr = {1, 3, 5, 7, 9, 11};
    cout << "Original array: ";
    for (int x : arr) cout << x << ' ';
    cout << '\n';

    SegmentTree<int> seg(arr);

    cout << "\nRange sum [1..4]: " << seg.range_sum(1, 4) << '\n';
    cout << "Range sum [0..5]: " << seg.range_sum(0, 5) << '\n';
    cout << "Range sum [2..3]: " << seg.range_sum(2, 3) << '\n';

    cout << "\nPoint update: set index 2 to 10\n";
    seg.point_update(2, 10);
    cout << "Range sum [0..5]: " << seg.range_sum(0, 5) << '\n';

    cout << "\nRange update: add 2 to indices [1..3]\n";
    seg.range_update(1, 3, 2);
    cout << "Range sum [0..5]: " << seg.range_sum(0, 5) << '\n';
    cout << "Range sum [1..3]: " << seg.range_sum(1, 3) << '\n';

    // Build with larger array
    cout << "\n--- Performance test: sum of first 1000 numbers ---\n";
    vector<long long> big;
    for (int i = 0; i < 1000; ++i) big.push_back(i + 1);

    SegmentTree<long long> big_seg(big);
    cout << "Sum 0..999: " << big_seg.range_sum(0, 999) << '\n';
    cout << "Expected: " << 1000LL * 1001 / 2 << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Build: O(n)\n";
    cout << "Point update: O(log n)\n";
    cout << "Range update (w/ lazy): O(log n)\n";
    cout << "Range query: O(log n)\n";
    cout << "Space: O(n)\n";

    return 0;
}
