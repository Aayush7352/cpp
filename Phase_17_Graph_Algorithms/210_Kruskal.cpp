/*
 * Phase 17 – Graph Algorithms
 * 210. Kruskal's MST Algorithm
 *
 * Theory:
 * - Finds Minimum Spanning Tree (MST) of a connected, undirected graph.
 * - Sort edges by weight, then greedily add edges if they don't create a cycle.
 * - Cycle detection using DSU (Union-Find).
 * - Time: O(E log E) for sorting + O(E α(V)) for union-find.
 * - Space: O(V + E).
 *
 * Common Mistakes:
 * - Not sorting edges by weight.
 * - Adding edge without checking if vertices are already connected.
 * - Returning wrong MST weight (overflow).
 * - Forgetting to handle disconnected graphs (returns MST of components).
 *
 * Interview Questions:
 * 1. Kruskal vs Prim.
 * 2. Find second-best MST.
 * 3. Maximum spanning tree (sort descending).
 * 4. Minimum product spanning tree.
 * 5. Number of spanning trees (Kirchhoff's theorem).
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

struct Edge {
    int u, v;
    long long weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class DSU {
    std::vector<int> parent, rank;
public:
    explicit DSU(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry])
            parent[rx] = ry;
        else if (rank[rx] > rank[ry])
            parent[ry] = rx;
        else {
            parent[ry] = rx;
            rank[rx]++;
        }
        return true;
    }
};

class Kruskal {
    int V;
    std::vector<Edge> edges;

public:
    explicit Kruskal(int n) : V(n) {}

    void add_edge(int u, int v, long long w) {
        edges.push_back({u, v, w});
    }

    struct Result {
        long long mst_weight;
        std::vector<Edge> mst_edges;
        bool possible;
    };

    Result run() const {
        std::vector<Edge> sorted = edges;
        std::sort(sorted.begin(), sorted.end());

        DSU dsu(V);
        Result result{0, {}, false};
        result.possible = false;

        for (const auto& e : sorted) {
            if (dsu.unite(e.u, e.v)) {
                result.mst_weight += e.weight;
                result.mst_edges.push_back(e);
            }
        }

        // Check if all vertices are connected
        if (result.mst_edges.size() == static_cast<size_t>(V - 1))
            result.possible = true;

        return result;
    }
};

int main() {
    using namespace std;

    cout << "=== Kruskal's MST Demo ===\n\n";

    Kruskal kruskal(6);

    // Graph:
    //   1 --2-- 2
    //  / \     / \
    // 4   1   3   5
    // /     \ /     \
    // 0 --3-- 3 --2-- 4
    //  \         /
    //   2       4
    //    \     /
    //      5

    kruskal.add_edge(0, 1, 4);
    kruskal.add_edge(0, 3, 3);
    kruskal.add_edge(0, 5, 2);
    kruskal.add_edge(1, 2, 2);
    kruskal.add_edge(1, 3, 1);
    kruskal.add_edge(2, 3, 3);
    kruskal.add_edge(2, 4, 5);
    kruskal.add_edge(3, 4, 2);
    kruskal.add_edge(4, 5, 4);

    auto result = kruskal.run();

    if (result.possible) {
        cout << "MST weight: " << result.mst_weight << '\n';
        cout << "MST edges:\n";
        for (const auto& e : result.mst_edges) {
            cout << "  " << e.u << " -- " << e.v << "  weight=" << e.weight << '\n';
        }
    } else {
        cout << "MST not possible (graph disconnected)\n";
    }

    // Simple graph
    cout << "\n--- Simple 4-vertex graph ---\n";
    Kruskal k2(4);
    k2.add_edge(0, 1, 10);
    k2.add_edge(0, 2, 6);
    k2.add_edge(0, 3, 5);
    k2.add_edge(1, 3, 15);
    k2.add_edge(2, 3, 4);

    auto r2 = k2.run();
    cout << "MST weight: " << r2.mst_weight << '\n';
    cout << "Edges:\n";
    for (const auto& e : r2.mst_edges)
        cout << "  " << e.u << " -- " << e.v << "  weight=" << e.weight << '\n';

    // Disconnected graph
    cout << "\n--- Disconnected graph ---\n";
    Kruskal k3(5);
    k3.add_edge(0, 1, 1);
    k3.add_edge(2, 3, 1);
    // vertex 4 is isolated
    auto r3 = k3.run();
    cout << "MST possible? " << boolalpha << r3.possible << '\n';
    cout << "Edges count: " << r3.mst_edges.size() << " (need " << 4 << ")\n";

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(E log E) sorting + O(E α(V)) DSU\n";
    cout << "Space: O(V + E)\n";

    return 0;
}
