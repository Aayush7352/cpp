/*
 * Phase 17 – Graph Algorithms
 * 214. Articulation Points (Cut Vertices)
 *
 * Theory:
 * - A vertex whose removal increases the number of connected components.
 * - Uses DFS with low-link values:
 *   - disc[u] = discovery time during DFS.
 *   - low[u] = min discovery time reachable from subtree of u.
 * - Root is articulation point if it has ≥ 2 children in DFS tree.
 * - Non-root u is articulation point if there exists child v where low[v] ≥ disc[u].
 * - Time: O(V + E), Space: O(V).
 *
 * Common Mistakes:
 * - Not handling root condition separately.
 * - Incorrect low-link update for back edges (disc[v], not low[v]).
 * - Not resetting visited/timer for disconnected graphs.
 *
 * Interview Questions:
 * 1. Find critical nodes in a network (cut vertices).
 * 2. Difference between articulation points and bridges.
 * 3. Find biconnected components.
 * 4. How to make a graph biconnected?
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

    std::vector<int> find_articulation_points() const {
        std::vector<int> disc(V, -1), low(V, -1);
        std::vector<bool> is_ap(V, false);
        int timer = 0;

        std::function<void(int, int)> dfs = [&](int u, int parent) {
            disc[u] = low[u] = timer++;
            int children = 0;

            for (int v : adj[u]) {
                if (v == parent) continue;

                if (disc[v] == -1) {
                    ++children;
                    dfs(v, u);
                    low[u] = std::min(low[u], low[v]);

                    // Non-root: articulation point if low[v] >= disc[u]
                    if (parent != -1 && low[v] >= disc[u])
                        is_ap[u] = true;
                } else {
                    // Back edge: update low with disc[v] (not low[v])
                    low[u] = std::min(low[u], disc[v]);
                }
            }

            // Root: articulation point if it has >= 2 children
            if (parent == -1 && children >= 2)
                is_ap[u] = true;
        };

        for (int i = 0; i < V; ++i)
            if (disc[i] == -1) dfs(i, -1);

        std::vector<int> result;
        for (int i = 0; i < V; ++i)
            if (is_ap[i]) result.push_back(i);
        return result;
    }

    // Find biconnected components using articulation points
    bool is_biconnected() const {
        return find_articulation_points().empty();
    }
};

int main() {
    using namespace std;

    cout << "=== Articulation Points Demo ===\n\n";

    // Graph with articulation points
    Graph g(7);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(3, 4);
    g.add_edge(3, 5);
    g.add_edge(4, 5);

    // 6 is a leaf attached to 5
    g.add_edge(5, 6);

    auto aps = g.find_articulation_points();
    cout << "Articulation points: ";
    if (aps.empty()) cout << "none";
    else for (int v : aps) cout << v << ' ';
    cout << '\n';

    // Another example
    cout << "\n--- Graph 2 ---\n";
    Graph g2(5);
    g2.add_edge(1, 0);
    g2.add_edge(0, 2);
    g2.add_edge(2, 1);
    g2.add_edge(0, 3);
    g2.add_edge(3, 4);

    aps = g2.find_articulation_points();
    cout << "Articulation points: ";
    for (int v : aps) cout << v << ' ';
    cout << '\n';

    // Biconnected graph
    cout << "\n--- Biconnected graph (no articulation points) ---\n";
    Graph g3(4);
    g3.add_edge(0, 1);
    g3.add_edge(1, 2);
    g3.add_edge(2, 3);
    g3.add_edge(3, 0);
    g3.add_edge(0, 2);
    aps = g3.find_articulation_points();
    cout << "Articulation points: ";
    if (aps.empty()) cout << "none";
    else for (int v : aps) cout << v << ' ';
    cout << '\n';
    cout << "Is biconnected? " << boolalpha << g3.is_biconnected() << '\n';

    // Cycle graph (no articulation points)
    cout << "\n--- Simple cycle ---\n";
    Graph cycle(4);
    cycle.add_edge(0, 1);
    cycle.add_edge(1, 2);
    cycle.add_edge(2, 3);
    cycle.add_edge(3, 0);
    aps = cycle.find_articulation_points();
    cout << "Articulation points: ";
    if (aps.empty()) cout << "none (cycle)";
    else for (int v : aps) cout << v << ' ';
    cout << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(V + E)\n";
    cout << "Space: O(V)\n";

    return 0;
}
