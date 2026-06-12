/*
 * Phase 17 – Graph Algorithms
 * 211. Prim's MST Algorithm
 *
 * Theory:
 * - Builds MST starting from an arbitrary root, greedily adding the cheapest edge
 *   connecting a visited vertex to an unvisited vertex.
 * - Uses a priority_queue (min-heap) for efficient extraction of minimum edge.
 * - Time: O(E log V) with binary heap, O(V^2) without PQ (dense graphs).
 * - Space: O(V + E).
 *
 * Common Mistakes:
 * - Not marking vertex as visited when processing.
 * - Pushing duplicate edges (lazy deletion handled by visited check).
 * - Wrong initialization of distances (INF).
 * - Using default max-heap instead of min-heap.
 *
 * Interview Questions:
 * 1. Prim's vs Kruskal's: when to use which?
 * 2. Minimum spanning tree for dense graph.
 * 3. Find second-best MST.
 * 4. Steiner tree problem (NP-hard).
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <functional>

class Prim {
    int V;
    std::vector<std::vector<std::pair<int, long long>>> adj;

public:
    explicit Prim(int n) : V(n), adj(n) {}

    void add_edge(int u, int v, long long w) {
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    struct Result {
        long long mst_weight;
        std::vector<int> parent;
        bool possible;
    };

    Result run(int start = 0) const {
        std::vector<long long> key(V, std::numeric_limits<long long>::max());
        std::vector<int> parent(V, -1);
        std::vector<bool> in_mst(V, false);
        std::vector<bool> visited(V, false);

        using P = std::pair<long long, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

        key[start] = 0;
        pq.emplace(0, start);

        while (!pq.empty()) {
            auto [w, u] = pq.top(); pq.pop();
            if (visited[u]) continue;
            visited[u] = true;

            for (auto [v, weight] : adj[u]) {
                if (!visited[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    pq.emplace(weight, v);
                }
            }
        }

        // Check if all vertices visited
        long long total_weight = 0;
        bool all_visited = true;
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) { all_visited = false; break; }
            total_weight += key[i];
        }

        return {total_weight, parent, all_visited};
    }

    std::vector<std::pair<int,int>> get_mst_edges(const Result& res) const {
        std::vector<std::pair<int,int>> edges;
        for (int i = 0; i < V; ++i) {
            if (res.parent[i] != -1)
                edges.emplace_back(res.parent[i], i);
        }
        return edges;
    }
};

int main() {
    using namespace std;

    cout << "=== Prim's MST Demo ===\n\n";

    Prim prim(6);

    // Same graph as Kruskal demo
    prim.add_edge(0, 1, 4);
    prim.add_edge(0, 3, 3);
    prim.add_edge(0, 5, 2);
    prim.add_edge(1, 2, 2);
    prim.add_edge(1, 3, 1);
    prim.add_edge(2, 3, 3);
    prim.add_edge(2, 4, 5);
    prim.add_edge(3, 4, 2);
    prim.add_edge(4, 5, 4);

    auto result = prim.run(0);

    if (result.possible) {
        cout << "MST weight (Prim's): " << result.mst_weight << '\n';
        cout << "MST edges:\n";
        auto edges = prim.get_mst_edges(result);
        for (auto [u, v] : edges) {
            cout << "  " << u << " -- " << v << '\n';
        }
    } else {
        cout << "MST not possible (disconnected)\n";
    }

    // Compare with Kruskal result
    cout << "\n--- Simple 4-vertex graph ---\n";
    Prim prim2(4);
    prim2.add_edge(0, 1, 10);
    prim2.add_edge(0, 2, 6);
    prim2.add_edge(0, 3, 5);
    prim2.add_edge(1, 3, 15);
    prim2.add_edge(2, 3, 4);

    auto r2 = prim2.run(0);
    cout << "MST weight: " << r2.mst_weight << '\n';
    auto edges2 = prim2.get_mst_edges(r2);
    for (auto [u, v] : edges2)
        cout << "  " << u << " -- " << v << '\n';

    // Different starting point yields same MST
    cout << "\nStarting from vertex 2:\n";
    auto r3 = prim2.run(2);
    cout << "MST weight: " << r3.mst_weight << " (same)\n";

    // Dense graph demo
    cout << "\n--- Dense graph (complete K4) ---\n";
    Prim prim3(4);
    prim3.add_edge(0, 1, 1);
    prim3.add_edge(0, 2, 2);
    prim3.add_edge(0, 3, 3);
    prim3.add_edge(1, 2, 4);
    prim3.add_edge(1, 3, 5);
    prim3.add_edge(2, 3, 6);
    auto r4 = prim3.run(0);
    cout << "MST weight: " << r4.mst_weight << '\n';
    for (auto [u, v] : prim3.get_mst_edges(r4))
        cout << "  " << u << " -- " << v << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(E log V) with binary heap\n";
    cout << "Space: O(V)\n";
    cout << "Best for dense graphs with O(V^2) implementation\n";

    return 0;
}
