/*
 * Phase 17 – Graph Algorithms
 * 208. Bellman-Ford Algorithm
 *
 * Theory:
 * - Finds shortest paths from source to all vertices even with negative weights.
 * - Relaxes all edges V-1 times. After V-1 iterations, shortest paths are found.
 * - One more iteration checks for negative-weight cycles.
 * - Time: O(V * E), Space: O(V).
 * - Slower than Dijkstra but handles negative weights.
 *
 * Common Mistakes:
 * - Forgetting the V-1 relaxations bound.
 * - Int overflow with large distances.
 * - Incorrectly detecting negative cycles (must check all edges one more time).
 * - Not initializing distances to INF.
 *
 * Interview Questions:
 * 1. How does Bellman-Ford detect negative cycles?
 * 2. Why must we relax exactly V-1 times?
 * 3. Difference between Dijkstra and Bellman-Ford.
 * 4. Find negative cycle in graph.
 * 5. Currency arbitrage (negative cycle detection).
 */

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

struct Edge {
    int u, v;
    long long weight;
};

class BellmanFord {
    int V;
    std::vector<Edge> edges;

public:
    explicit BellmanFord(int n) : V(n) {}

    void add_edge(int u, int v, long long w) {
        edges.push_back({u, v, w});
    }

    struct Result {
        std::vector<long long> dist;
        std::vector<int> parent;
        bool has_negative_cycle;
    };

    Result shortest_path(int source) const {
        std::vector<long long> dist(V, std::numeric_limits<long long>::max());
        std::vector<int> parent(V, -1);
        dist[source] = 0;

        // Relax all edges V-1 times
        for (int i = 0; i < V - 1; ++i) {
            bool relaxed = false;
            for (const auto& [u, v, w] : edges) {
                if (dist[u] != std::numeric_limits<long long>::max() &&
                    dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    relaxed = true;
                }
            }
            if (!relaxed) break; // early termination
        }

        // Check for negative cycles
        bool has_neg_cycle = false;
        for (const auto& [u, v, w] : edges) {
            if (dist[u] != std::numeric_limits<long long>::max() &&
                dist[v] > dist[u] + w) {
                has_neg_cycle = true;
                break;
            }
        }

        return {dist, parent, has_neg_cycle};
    }

    std::vector<int> get_path(const Result& res, int target) const {
        std::vector<int> path;
        if (res.has_negative_cycle) return path;
        if (res.dist[target] == std::numeric_limits<long long>::max())
            return path;
        for (int v = target; v != -1; v = res.parent[v])
            path.push_back(v);
        std::reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    using namespace std;

    cout << "=== Bellman-Ford Demo ===\n\n";

    // Graph with negative edges
    BellmanFord bf(5);
    bf.add_edge(0, 1, 6);
    bf.add_edge(0, 2, 7);
    bf.add_edge(1, 2, 8);
    bf.add_edge(1, 3, 5);
    bf.add_edge(1, 4, -4);
    bf.add_edge(2, 3, -3);
    bf.add_edge(2, 4, 9);
    bf.add_edge(3, 1, -2);
    bf.add_edge(4, 0, 2);
    bf.add_edge(4, 3, 7);

    int source = 0;
    auto result = bf.shortest_path(source);

    cout << "Shortest distances from node " << source << ":\n";
    for (int i = 0; i < 5; ++i) {
        cout << "  to " << i << ": ";
        if (result.dist[i] == numeric_limits<long long>::max())
            cout << "INF";
        else
            cout << result.dist[i];
        cout << '\n';
    }

    cout << "\nNegative cycle present? " << boolalpha
         << result.has_negative_cycle << '\n';

    cout << "\nPath from " << source << " to 4:\n  ";
    auto path = bf.get_path(result, 4);
    if (!path.empty()) {
        for (size_t i = 0; i < path.size(); ++i) {
            if (i) cout << " -> ";
            cout << path[i];
        }
        cout << '\n';
    }

    // Graph with negative cycle
    cout << "\n--- Graph with negative cycle ---\n";
    BellmanFord neg(3);
    neg.add_edge(0, 1, 1);
    neg.add_edge(1, 2, -1);
    neg.add_edge(2, 0, -1); // cycle 0->1->2->0 with weight -1
    auto res2 = neg.shortest_path(0);
    cout << "Negative cycle present? " << res2.has_negative_cycle << '\n';

    // Non-negative graph comparison with Dijkstra
    cout << "\n--- Non-negative graph ---\n";
    BellmanFord bf2(4);
    bf2.add_edge(0, 1, 4);
    bf2.add_edge(0, 2, 2);
    bf2.add_edge(1, 3, 1);
    bf2.add_edge(2, 1, 1);
    bf2.add_edge(2, 3, 5);
    auto res3 = bf2.shortest_path(0);
    cout << "Distances from 0:\n";
    for (int i = 0; i < 4; ++i)
        cout << "  to " << i << ": " << res3.dist[i] << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(V * E)\n";
    cout << "Space: O(V)\n";
    cout << "Handles negative weights and detects negative cycles\n";

    return 0;
}
