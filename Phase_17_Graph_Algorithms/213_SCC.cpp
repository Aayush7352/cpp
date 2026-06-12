/*
 * Phase 17 – Graph Algorithms
 * 213. Strongly Connected Components (SCC)
 *
 * Theory:
 * - SCC = maximal set of vertices where every vertex is reachable from every other.
 * - Kosaraju's algorithm:
 *   1. DFS on original graph, push to stack in post-order.
 *   2. Reverse graph.
 *   3. Pop from stack, DFS on reversed graph to get SCCs.
 *   Time: O(V + E), Space: O(V + E).
 * - Tarjan's algorithm:
 *   1. Single DFS with low-link values and a stack.
 *   2. If low[u] == disc[u], pop SCC from stack.
 *   Time: O(V + E), Space: O(V).
 *
 * Common Mistakes:
 * - Kosaraju: forgetting to reverse graph or process in correct order.
 * - Tarjan: not managing indices/scc_id correctly.
 * - Confusion between articulation points/low-link.
 *
 * Interview Questions:
 * 1. Find if whole graph is strongly connected.
 * 2. Condense graph (DAG of SCCs).
 * 3. Minimum edges to make graph strongly connected.
 * 4. 2-SAT problem (uses SCC).
 */

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>

class Graph {
    int V;
    std::vector<std::vector<int>> adj;

public:
    explicit Graph(int n) : V(n), adj(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    int vertices() const { return V; }

    const std::vector<int>& neighbors(int u) const { return adj[u]; }

    Graph reverse() const {
        Graph rev(V);
        for (int u = 0; u < V; ++u)
            for (int v : adj[u])
                rev.add_edge(v, u);
        return rev;
    }

    // ========== Kosaraju's Algorithm ==========
    std::vector<std::vector<int>> kosaraju_scc() const {
        std::vector<bool> visited(V, false);
        std::stack<int> st;

        // Step 1: DFS and fill stack in post-order
        std::function<void(int)> fill_order = [&](int u) {
            visited[u] = true;
            for (int v : adj[u])
                if (!visited[v]) fill_order(v);
            st.push(u);
        };

        for (int i = 0; i < V; ++i)
            if (!visited[i]) fill_order(i);

        // Step 2: Reverse graph
        Graph rev = reverse();

        // Step 3: DFS on reversed graph in stack order
        std::fill(visited.begin(), visited.end(), false);
        std::vector<std::vector<int>> sccs;

        std::function<void(int, std::vector<int>&)> dfs_rev = [&](int u, std::vector<int>& comp) {
            visited[u] = true;
            comp.push_back(u);
            for (int v : rev.adj[u])
                if (!visited[v]) dfs_rev(v, comp);
        };

        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (!visited[u]) {
                std::vector<int> comp;
                dfs_rev(u, comp);
                std::sort(comp.begin(), comp.end());
                sccs.push_back(comp);
            }
        }

        return sccs;
    }

    // ========== Tarjan's Algorithm ==========
    std::vector<std::vector<int>> tarjan_scc() const {
        std::vector<int> disc(V, -1), low(V, -1);
        std::vector<bool> in_stack(V, false);
        std::stack<int> st;
        std::vector<std::vector<int>> sccs;
        int timer = 0;

        std::function<void(int)> dfs = [&](int u) {
            disc[u] = low[u] = timer++;
            st.push(u);
            in_stack[u] = true;

            for (int v : adj[u]) {
                if (disc[v] == -1) {
                    dfs(v);
                    low[u] = std::min(low[u], low[v]);
                } else if (in_stack[v]) {
                    low[u] = std::min(low[u], disc[v]);
                }
            }

            if (low[u] == disc[u]) {
                std::vector<int> comp;
                while (true) {
                    int v = st.top(); st.pop();
                    in_stack[v] = false;
                    comp.push_back(v);
                    if (v == u) break;
                }
                std::sort(comp.begin(), comp.end());
                sccs.push_back(comp);
            }
        };

        for (int i = 0; i < V; ++i)
            if (disc[i] == -1) dfs(i);

        return sccs;
    }

    // Condense graph to DAG of SCCs
    Graph condense(const std::vector<std::vector<int>>& sccs) const {
        int n = sccs.size();
        std::vector<int> comp_id(V, -1);
        for (size_t i = 0; i < sccs.size(); ++i)
            for (int v : sccs[i])
                comp_id[v] = i;

        Graph dag(n);
        for (int u = 0; u < V; ++u) {
            for (int v : adj[u]) {
                if (comp_id[u] != comp_id[v])
                    dag.add_edge(comp_id[u], comp_id[v]);
            }
        }
        return dag;
    }
};

int main() {
    using namespace std;

    cout << "=== Strongly Connected Components Demo ===\n\n";

    Graph g(8);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    g.add_edge(4, 5);
    g.add_edge(5, 3);
    g.add_edge(6, 5);
    g.add_edge(6, 7);
    g.add_edge(7, 6);

    auto sccs_k = g.kosaraju_scc();
    cout << "Kosaraju's SCCs:\n";
    int idx = 0;
    for (const auto& comp : sccs_k) {
        cout << "  SCC " << idx++ << ": ";
        for (int v : comp) cout << v << ' ';
        cout << '\n';
    }

    auto sccs_t = g.tarjan_scc();
    cout << "\nTarjan's SCCs:\n";
    idx = 0;
    for (const auto& comp : sccs_t) {
        cout << "  SCC " << idx++ << ": ";
        for (int v : comp) cout << v << ' ';
        cout << '\n';
    }

    // Condensed DAG
    cout << "\nCondensed graph (DAG of SCCs):\n";
    auto dag = g.condense(sccs_k);
    for (int i = 0; i < dag.vertices(); ++i) {
        cout << "  " << i << " -> ";
        for (int v : dag.neighbors(i)) cout << v << ' ';
        cout << '\n';
    }

    // Simple strongly connected check
    cout << "\nWhole graph strongly connected? ";
    cout << (sccs_k.size() == 1 ? "Yes" : "No") << '\n';

    Graph single_scc(3);
    single_scc.add_edge(0, 1);
    single_scc.add_edge(1, 2);
    single_scc.add_edge(2, 0);
    auto scc = single_scc.kosaraju_scc();
    cout << "Single SCC graph has " << scc.size() << " SCC(s)\n";

    cout << "\n--- Complexity ---\n";
    cout << "Kosaraju: O(V + E)\n";
    cout << "Tarjan: O(V + E)\n";
    cout << "Space: O(V + E)\n";

    return 0;
}
