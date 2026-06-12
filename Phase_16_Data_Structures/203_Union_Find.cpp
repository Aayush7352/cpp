/*
 * Phase 16 – Data Structures
 * 203. Union-Find (Disjoint Set Union / DSU)
 *
 * Theory:
 * - DSU maintains a collection of disjoint sets.
 * - Operations:
 *   find(x): find representative of set containing x.
 *   union(x,y): merge sets containing x and y.
 * - Optimizations:
 *   Path compression: during find, make each node point to root directly.
 *   Union by rank/size: attach smaller tree under larger tree.
 * - Amortized O(α(n)) per operation (inverse Ackermann).
 * - Applications: cycle detection in graphs, Kruskal's MST, connected components.
 *
 * Common Mistakes:
 * - Not using path compression (find degrades to O(n)).
 * - Union by rank but forgetting to update rank.
 * - Confusing parent array vs rank array.
 * - Not checking if already in same set before union.
 *
 * Interview Questions:
 * 1. Detect cycles in an undirected graph.
 * 2. Number of connected components in a graph.
 * 3. Find if two nodes are in same set.
 * 4. Union by size vs rank: trade-offs.
 * 5. Count islands in a grid using DSU.
 */

#include <iostream>
#include <vector>
#include <numeric>

class UnionFind {
    std::vector<int> parent;
    std::vector<int> rank; // rank = approximate height
    int num_sets;

public:
    explicit UnionFind(int n) : parent(n), rank(n, 0), num_sets(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        // Path compression
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    // Union by rank
    bool unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return false;

        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            rank[rx]++;
        }
        --num_sets;
        return true;
    }

    bool same_set(int x, int y) {
        return find(x) == find(y);
    }

    int count_sets() const { return num_sets; }

    void display_sets() const {
        std::vector<std::vector<int>> sets(parent.size());
        for (size_t i = 0; i < parent.size(); ++i) {
            sets[find(i)].push_back(i);
        }
        for (size_t i = 0; i < sets.size(); ++i) {
            if (!sets[i].empty()) {
                std::cout << "Set " << i << ": ";
                for (int x : sets[i]) std::cout << x << ' ';
                std::cout << '\n';
            }
        }
    }
};

// Alternative: Union by size
class UnionFindSize {
    std::vector<int> parent;
    std::vector<int> size;
    int num_sets;

public:
    explicit UnionFindSize(int n) : parent(n), size(n, 1), num_sets(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // path halving
            x = parent[x];
        }
        return x;
    }

    bool unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return false;

        if (size[rx] < size[ry]) {
            parent[rx] = ry;
            size[ry] += size[rx];
        } else {
            parent[ry] = rx;
            size[rx] += size[ry];
        }
        --num_sets;
        return true;
    }

    int component_size(int x) {
        return size[find(x)];
    }

    int count_sets() const { return num_sets; }
    bool same_set(int x, int y) { return find(x) == find(y); }
};

// Cycle detection in undirected graph
bool has_cycle(int V, const std::vector<std::pair<int,int>>& edges) {
    UnionFind uf(V);
    for (auto [u, v] : edges) {
        if (uf.same_set(u, v)) return true;
        uf.unite(u, v);
    }
    return false;
}

int main() {
    using namespace std;

    cout << "=== Union-Find (DSU) Demo ===\n\n";

    // Basic operations
    cout << "--- Union-Find with Path Compression + Union by Rank ---\n";
    UnionFind uf(10);

    cout << "Initial sets: " << uf.count_sets() << '\n';

    uf.unite(0, 1);
    uf.unite(2, 3);
    uf.unite(4, 5);
    uf.unite(6, 7);
    uf.unite(8, 9);
    cout << "After 5 unions: " << uf.count_sets() << " sets\n";

    uf.unite(0, 2);
    uf.unite(4, 6);
    uf.unite(0, 4); // merge {0,1,2,3} with {4,5,6,7}
    cout << "After merging: " << uf.count_sets() << " sets\n";

    cout << "0 and 7 same set? " << boolalpha << uf.same_set(0, 7) << '\n';
    cout << "0 and 8 same set? " << uf.same_set(0, 8) << '\n';

    cout << "\nSets:\n";
    uf.display_sets();

    // ---- Union by Size ----
    cout << "\n--- Union-Find with Union by Size ---\n";
    UnionFindSize ufs(8);
    ufs.unite(0, 1);
    ufs.unite(0, 2);
    ufs.unite(3, 4);
    ufs.unite(5, 6);
    ufs.unite(5, 7);
    cout << "Component size of 0: " << ufs.component_size(0) << '\n';
    cout << "Component size of 3: " << ufs.component_size(3) << '\n';
    cout << "Component size of 5: " << ufs.component_size(5) << '\n';

    ufs.unite(0, 3);
    cout << "After uniting 0's set with 3's set:\n";
    cout << "Component size of 0: " << ufs.component_size(0) << '\n';
    cout << "Sets count: " << ufs.count_sets() << '\n';

    // ---- Cycle Detection ----
    cout << "\n--- Cycle Detection in Graph ---\n";
    // Graph: 0-1-2-0 (triangle)
    vector<pair<int,int>> edges = {{0,1}, {1,2}, {2,0}};
    cout << "Graph with edges (0-1, 1-2, 2-0): ";
    cout << (has_cycle(3, edges) ? "Has cycle" : "No cycle") << '\n';

    // Graph: 0-1-2 (no cycle)
    vector<pair<int,int>> edges2 = {{0,1}, {1,2}};
    cout << "Graph with edges (0-1, 1-2): ";
    cout << (has_cycle(3, edges2) ? "Has cycle" : "No cycle") << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Find (path compression): O(α(n)) amortized\n";
    cout << "Union (by rank/size): O(α(n)) amortized\n";
    cout << "Space: O(n)\n";
    cout << "α(n) ≤ 4 for practical n\n";

    return 0;
}
