/*
 * Phase 17 – Graph Algorithms
 * 205. BFS (Breadth-First Search)
 *
 * Theory:
 * - Traverses graph level by level using a queue.
 * - Finds shortest path in unweighted graph (minimum number of edges).
 * - Applications: shortest path, connected components, bipartite check,
 *   level-order traversal of tree, web crawling.
 * - Time: O(V + E), Space: O(V) for queue and visited array.
 *
 * Common Mistakes:
 * - Not marking node as visited when enqueuing (causes duplicate pushes).
 * - Forgetting to handle disconnected components.
 * - Not distinguishing visited for parent tracking.
 *
 * Interview Questions:
 * 1. Find shortest path in unweighted graph.
 * 2. Check if graph is bipartite.
 * 3. Find all connected components.
 * 4. Minimum number of knight moves on chessboard.
 * 5. Word ladder problem.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>

class Graph {
    int V;
    std::vector<std::vector<int>> adj;

public:
    Graph(int vertices) : V(vertices), adj(vertices) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // BFS traversal from source
    void bfs(int start) const {
        std::vector<bool> visited(V, false);
        std::queue<int> q;

        visited[start] = true;
        q.push(start);

        std::cout << "BFS from " << start << ": ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            std::cout << u << ' ';
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        std::cout << '\n';
    }

    // Shortest path from start to target (unweighted)
    std::vector<int> shortest_path(int start, int target) const {
        std::vector<int> dist(V, -1);
        std::vector<int> parent(V, -1);
        std::queue<int> q;

        dist[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == target) break;
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        std::vector<int> path;
        if (dist[target] == -1) return path; // no path

        for (int v = target; v != -1; v = parent[v])
            path.push_back(v);
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Find all connected components
    int connected_components() const {
        std::vector<bool> visited(V, false);
        int components = 0;

        auto component_bfs = [&](int start) {
            std::queue<int> q;
            q.push(start);
            visited[start] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        };

        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                ++components;
                component_bfs(i);
            }
        }
        return components;
    }

    // Check if bipartite (2-colorable)
    bool is_bipartite() const {
        std::vector<int> color(V, -1);
        for (int i = 0; i < V; ++i) {
            if (color[i] != -1) continue;
            color[i] = 0;
            std::queue<int> q;
            q.push(i);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // Level-order traversal (same as BFS, but show levels)
    void level_order(int start) const {
        std::vector<int> level(V, -1);
        std::queue<int> q;
        level[start] = 0;
        q.push(start);

        std::vector<std::vector<int>> levels;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (static_cast<size_t>(level[u]) >= levels.size())
                levels.resize(level[u] + 1);
            levels[level[u]].push_back(u);
            for (int v : adj[u]) {
                if (level[v] == -1) {
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }

        std::cout << "Level order from " << start << ":\n";
        for (size_t i = 0; i < levels.size(); ++i) {
            std::cout << "  Level " << i << ": ";
            for (int v : levels[i]) std::cout << v << ' ';
            std::cout << '\n';
        }
    }
};

int main() {
    using namespace std;

    cout << "=== BFS Demo ===\n\n";

    Graph g(8);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 5);
    g.add_edge(2, 6);
    g.add_edge(3, 7);
    g.add_edge(4, 7);
    g.add_edge(5, 7);

    g.bfs(0);

    cout << "\nLevel order:\n";
    g.level_order(0);

    cout << "\nShortest path from 0 to 7:\n  ";
    auto path = g.shortest_path(0, 7);
    if (!path.empty()) {
        for (size_t i = 0; i < path.size(); ++i) {
            if (i) cout << " -> ";
            cout << path[i];
        }
        cout << '\n';
    }

    // Disconnected graph
    cout << "\nConnected components of main graph: "
         << g.connected_components() << '\n';

    Graph g2(5);
    g2.add_edge(0, 1);
    g2.add_edge(2, 3);
    // 4 is isolated
    cout << "Connected components of g2: " << g2.connected_components() << '\n';

    // Bipartite check
    cout << "\nBipartite check:\n";
    Graph bp(4);
    bp.add_edge(0, 1);
    bp.add_edge(1, 2);
    bp.add_edge(2, 3);
    bp.add_edge(3, 0);
    cout << "  Even cycle (0-1-2-3-0): " << (bp.is_bipartite() ? "Yes" : "No") << '\n';

    Graph bp2(3);
    bp2.add_edge(0, 1);
    bp2.add_edge(1, 2);
    bp2.add_edge(2, 0);
    cout << "  Odd cycle (triangle): " << (bp2.is_bipartite() ? "Yes" : "No") << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(V + E)\n";
    cout << "Space: O(V)\n";

    return 0;
}
