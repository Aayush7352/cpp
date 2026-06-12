/*
 * Phase 17 – Graph Algorithms
 * 206. DFS (Depth-First Search)
 *
 * Theory:
 * - Explores as far as possible along each branch before backtracking.
 * - Recursive (implicit stack) or iterative (explicit stack).
 * - Applications: connected components, cycle detection, topological sort,
 *   strongly connected components, bipartite check, maze solving.
 * - Time: O(V + E), Space: O(V) (stack/recursion depth).
 *
 * Common Mistakes:
 * - Not handling disconnected components.
 * - Stack overflow with deep recursion (use iterative for large graphs).
 * - Confusing pre-order vs post-order traversal.
 * - Not marking visited properly in iterative version.
 *
 * Interview Questions:
 * 1. Detect cycle in directed graph using DFS.
 * 2. Detect cycle in undirected graph using DFS.
 * 3. Find connected components using DFS.
 * 4. Find topological ordering using DFS.
 * 5. Count paths between two nodes using DFS.
 */

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>

class Graph {
    int V;
    std::vector<std::vector<int>> adj;
    bool directed;

public:
    Graph(int vertices, bool dir = false) : V(vertices), adj(vertices), directed(dir) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        if (!directed) adj[v].push_back(u);
    }

    // ---- Recursive DFS ----
    void dfs_recursive(int start) const {
        std::vector<bool> visited(V, false);

        std::function<void(int)> dfs = [&](int u) {
            visited[u] = true;
            std::cout << u << ' ';
            for (int v : adj[u])
                if (!visited[v]) dfs(v);
        };

        std::cout << "DFS (recursive) from " << start << ": ";
        dfs(start);
        // Also visit disconnected components
        for (int i = 0; i < V; ++i)
            if (!visited[i]) dfs(i);
        std::cout << '\n';
    }

    // ---- Iterative DFS ----
    void dfs_iterative(int start) const {
        std::vector<bool> visited(V, false);
        std::stack<int> st;

        std::cout << "DFS (iterative) from " << start << ": ";
        st.push(start);

        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (visited[u]) continue;
            visited[u] = true;
            std::cout << u << ' ';
            // Push in reverse order for consistent order with recursive
            for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it)
                if (!visited[*it]) st.push(*it);
        }
        // Handle disconnected components
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                st.push(i);
                while (!st.empty()) {
                    int u = st.top(); st.pop();
                    if (visited[u]) continue;
                    visited[u] = true;
                    std::cout << u << ' ';
                    for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it)
                        if (!visited[*it]) st.push(*it);
                }
            }
        }
        std::cout << '\n';
    }

    // ---- Connected Components ----
    int count_connected_components() const {
        std::vector<bool> visited(V, false);
        int count = 0;

        std::function<void(int)> dfs = [&](int u) {
            visited[u] = true;
            for (int v : adj[u])
                if (!visited[v]) dfs(v);
        };

        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                ++count;
                dfs(i);
            }
        }
        return count;
    }

    // ---- Cycle Detection (undirected) ----
    bool has_cycle_undirected() const {
        std::vector<bool> visited(V, false);

        std::function<bool(int, int)> dfs = [&](int u, int parent) -> bool {
            visited[u] = true;
            for (int v : adj[u]) {
                if (!visited[v]) {
                    if (dfs(v, u)) return true;
                } else if (v != parent) {
                    return true;
                }
            }
            return false;
        };

        for (int i = 0; i < V; ++i)
            if (!visited[i] && dfs(i, -1)) return true;
        return false;
    }

    // ---- Cycle Detection (directed) ----
    bool has_cycle_directed() const {
        if (!directed) return has_cycle_undirected();

        std::vector<bool> visited(V, false);
        std::vector<bool> in_stack(V, false);

        std::function<bool(int)> dfs = [&](int u) -> bool {
            visited[u] = true;
            in_stack[u] = true;
            for (int v : adj[u]) {
                if (!visited[v]) {
                    if (dfs(v)) return true;
                } else if (in_stack[v]) {
                    return true;
                }
            }
            in_stack[u] = false;
            return false;
        };

        for (int i = 0; i < V; ++i)
            if (!visited[i] && dfs(i)) return true;
        return false;
    }

    // ---- Topological Order (DFS-based) ----
    std::vector<int> topological_sort() const {
        if (!directed) return {};

        std::vector<bool> visited(V, false);
        std::vector<int> order;

        std::function<void(int)> dfs = [&](int u) {
            visited[u] = true;
            for (int v : adj[u])
                if (!visited[v]) dfs(v);
            order.push_back(u);
        };

        for (int i = 0; i < V; ++i)
            if (!visited[i]) dfs(i);

        std::reverse(order.begin(), order.end());
        return order;
    }
};

int main() {
    using namespace std;

    cout << "=== DFS Demo ===\n\n";

    // Undirected graph
    Graph g(7);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 5);
    g.add_edge(2, 6);

    g.dfs_recursive(0);
    g.dfs_iterative(0);

    cout << "\nConnected components: " << g.count_connected_components() << '\n';

    // Cycle detection
    Graph cycle_graph(4);
    cycle_graph.add_edge(0, 1);
    cycle_graph.add_edge(1, 2);
    cycle_graph.add_edge(2, 0);
    cycle_graph.add_edge(2, 3);
    cout << "\nCycle in undirected graph? " << boolalpha
         << cycle_graph.has_cycle_undirected() << '\n';

    // Directed cycle
    Graph dg(4, true);
    dg.add_edge(0, 1);
    dg.add_edge(1, 2);
    dg.add_edge(2, 0);
    dg.add_edge(2, 3);
    cout << "Cycle in directed graph? " << dg.has_cycle_directed() << '\n';

    // DAG topological sort
    Graph dag(6, true);
    dag.add_edge(5, 2);
    dag.add_edge(5, 0);
    dag.add_edge(4, 0);
    dag.add_edge(4, 1);
    dag.add_edge(2, 3);
    dag.add_edge(3, 1);
    cout << "\nTopological sort of DAG: ";
    auto topo = dag.topological_sort();
    for (int v : topo) cout << v << ' ';
    cout << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(V + E)\n";
    cout << "Space: O(V) (recursion stack)\n";

    return 0;
}
