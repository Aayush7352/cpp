/*
 * Phase 17 – Graph Algorithms
 * 215. Bridges (Cut Edges)
 *
 * Theory:
 * - An edge whose removal increases the number of connected components.
 * - Uses DFS with low-link values (Tarjan's algorithm).
 * - Edge (u, v) is a bridge if low[v] > disc[u] (strictly greater).
 * - Note: For articulation points, condition is low[v] >= disc[u];
 *   for bridges, it's low[v] > disc[u] (no back edge from v's subtree to u or above).
 * - Time: O(V + E), Space: O(V).
 *
 * Common Mistakes:
 * - Confusing articulation point condition with bridge condition.
 * - Not handling parallel edges (mutli-edges are NOT bridges).
 * - Forgetting to check for parent edge (v == parent) in DFS.
 *
 * Interview Questions:
 * 1. Find critical connections in a network.
 * 2. Difference between bridges and articulation points.
 * 3. Find bridges in a graph that disconnect components.
 * 4. How to make graph bridge-less?
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

class Graph {
    int V;
    std::vector<std::vector<int>> adj;

public:
    explicit Graph(int n) : V(n), adj(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Find all bridges using Tarjan's algorithm
    std::vector<std::pair<int, int>> find_bridges() const {
        std::vector<int> disc(V, -1), low(V, -1);
        std::vector<std::pair<int, int>> bridges;
        int timer = 0;

        std::function<void(int, int)> dfs = [&](int u, int parent) {
            disc[u] = low[u] = timer++;

            for (int v : adj[u]) {
                if (v == parent) continue;

                if (disc[v] == -1) {
                    dfs(v, u);
                    low[u] = std::min(low[u], low[v]);

                    // Bridge condition: low[v] > disc[u]
                    if (low[v] > disc[u]) {
                        if (u < v)
                            bridges.emplace_back(u, v);
                        else
                            bridges.emplace_back(v, u);
                    }
                } else {
                    // Back edge
                    low[u] = std::min(low[u], disc[v]);
                }
            }
        };

        for (int i = 0; i < V; ++i)
            if (disc[i] == -1) dfs(i, -1);

        std::sort(bridges.begin(), bridges.end());
        return bridges;
    }

    bool is_bridge(int u, int v) const {
        auto bridges = find_bridges();
        for (auto [a, b] : bridges)
            if ((a == u && b == v) || (a == v && b == u))
                return true;
        return false;
    }
};

int main() {
    using namespace std;

    cout << "=== Bridges Demo ===\n\n";

    // Graph with bridges
    Graph g(5);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0); // cycle 0-1-2
    g.add_edge(1, 3);
    g.add_edge(3, 4); // bridge: 3-4

    auto bridges = g.find_bridges();
    cout << "Bridges: ";
    if (bridges.empty()) cout << "none";
    else {
        for (auto [u, v] : bridges)
            cout << "(" << u << "," << v << ") ";
    }
    cout << '\n';

    // Another example
    cout << "\n--- Graph 2 ---\n";
    Graph g2(7);
    g2.add_edge(0, 1);
    g2.add_edge(0, 2);
    g2.add_edge(1, 2);
    g2.add_edge(1, 3);
    g2.add_edge(3, 4);
    g2.add_edge(4, 5);
    g2.add_edge(5, 3); // cycle 3-4-5
    g2.add_edge(1, 6);

    bridges = g2.find_bridges();
    cout << "Bridges: ";
    for (auto [u, v] : bridges)
        cout << "(" << u << "," << v << ") ";
    cout << '\n';

    // Check specific edge
    cout << "\nIs edge (1,3) a bridge? " << boolalpha
         << g2.is_bridge(1, 3) << '\n';
    cout << "Is edge (3,4) a bridge? " << g2.is_bridge(3, 4) << '\n';

    // Graph with no bridges
    cout << "\n--- Graph with no bridges (biconnected) ---\n";
    Graph g3(4);
    g3.add_edge(0, 1);
    g3.add_edge(1, 2);
    g3.add_edge(2, 3);
    g3.add_edge(3, 0);
    g3.add_edge(0, 2);
    bridges = g3.find_bridges();
    cout << "Bridges: ";
    if (bridges.empty()) cout << "none";
    else for (auto [u, v] : bridges)
        cout << "(" << u << "," << v << ") ";
    cout << '\n';

    // Tree (all edges are bridges)
    cout << "\n--- Tree (all edges are bridges) ---\n";
    Graph tree(5);
    tree.add_edge(0, 1);
    tree.add_edge(1, 2);
    tree.add_edge(1, 3);
    tree.add_edge(3, 4);
    bridges = tree.find_bridges();
    cout << "Bridges: ";
    for (auto [u, v] : bridges)
        cout << "(" << u << "," << v << ") ";
    cout << "\n(all edges in a tree are bridges)\n";

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(V + E)\n";
    cout << "Space: O(V)\n";

    return 0;
}
