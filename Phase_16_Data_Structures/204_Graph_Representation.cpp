/*
 * Phase 16 – Data Structures
 * 204. Graph Representation
 *
 * Theory:
 * - Graph G = (V, E). V = vertices, E = edges.
 * - Representations:
 *   1. Adjacency Matrix: O(V^2) space, O(1) edge check.
 *   2. Adjacency List: O(V + E) space, O(degree(u)) edge check.
 *   3. Edge List: O(E) space, simple, used in Kruskal's.
 * - Weighted graphs: store pair<neighbor, weight> in adjacency list.
 * - Directed vs undirected: undirected adds both (u,v) and (v,u).
 *
 * Common Mistakes:
 * - Forgetting reserve() for adjacency list leads to many reallocations.
 * - Off-by-one: 0-indexing vs 1-indexing.
 * - Not marking visited during traversal (infinite loops).
 *
 * Interview Questions:
 * 1. When to use matrix vs list?
 * 2. Detect cycle in directed/undirected graph.
 * 3. Find shortest path in unweighted graph (BFS).
 * 4. Transpose a directed graph.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

// ========== Adjacency Matrix ==========
template<typename T = int>
class GraphMatrix {
    vector<vector<T>> mat;
    int V;
    bool directed;

public:
    GraphMatrix(int vertices, bool dir = false) : V(vertices), directed(dir) {
        mat.assign(V, vector<T>(V, T(0)));
    }

    void add_edge(int u, int v, T weight = 1) {
        mat[u][v] = weight;
        if (!directed) mat[v][u] = weight;
    }

    bool has_edge(int u, int v) const { return mat[u][v] != T(0); }
    T get_weight(int u, int v) const { return mat[u][v]; }

    void display() const {
        cout << "  ";
        for (int i = 0; i < V; ++i) cout << setw(3) << i;
        cout << '\n';
        for (int i = 0; i < V; ++i) {
            cout << i << " ";
            for (int j = 0; j < V; ++j)
                cout << setw(3) << mat[i][j];
            cout << '\n';
        }
    }

    int vertices() const { return V; }
};

// ========== Adjacency List ==========
template<typename T = int>
class GraphList {
public:
    struct Edge {
        int to;
        T weight;
    };

    int V;
    bool directed;
    vector<vector<Edge>> adj;

    GraphList(int vertices, bool dir = false) : V(vertices), directed(dir) {
        adj.resize(V);
    }

    void add_edge(int u, int v, T weight = 1) {
        adj[u].push_back({v, weight});
        if (!directed) adj[v].push_back({u, weight});
    }

    void display() const {
        for (int i = 0; i < V; ++i) {
            cout << i << ": ";
            for (const auto& e : adj[i])
                cout << "(" << e.to << "," << e.weight << ") ";
            cout << '\n';
        }
    }

    // BFS traversal
    void bfs(int start) const {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << ' ';
            for (const auto& e : adj[u]) {
                if (!visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        cout << '\n';
    }

    // DFS traversal (iterative)
    void dfs(int start) const {
        vector<bool> visited(V, false);
        stack<int> st;
        st.push(start);

        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (visited[u]) continue;
            visited[u] = true;
            cout << u << ' ';
            // Push in reverse order to simulate recursive order
            for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it)
                if (!visited[it->to]) st.push(it->to);
        }
        cout << '\n';
    }

    int vertices() const { return V; }
};

// ========== Edge List ==========
template<typename T = int>
struct Edge {
    int u, v;
    T weight;
};

template<typename T = int>
class EdgeList {
public:
    vector<Edge<T>> edges;
    int V;
    bool directed;

    EdgeList(int vertices, bool dir = false) : V(vertices), directed(dir) {}

    void add_edge(int u, int v, T weight = 1) {
        edges.push_back({u, v, weight});
        if (!directed) edges.push_back({v, u, weight});
    }

    void display() const {
        for (const auto& e : edges)
            cout << e.u << " --" << e.weight << "--> " << e.v << '\n';
    }

    // Sort by weight (for Kruskal's)
    void sort_by_weight() {
        sort(edges.begin(), edges.end(),
             [](const Edge<T>& a, const Edge<T>& b) { return a.weight < b.weight; });
    }

    int vertices() const { return V; }
};

int main() {
    cout << "=== Graph Representation Demo ===\n\n";

    // ---- Adjacency Matrix ----
    cout << "--- Adjacency Matrix (undirected, unweighted) ---\n";
    GraphMatrix<int> gm(5);
    gm.add_edge(0, 1);
    gm.add_edge(0, 4);
    gm.add_edge(1, 2);
    gm.add_edge(1, 3);
    gm.add_edge(1, 4);
    gm.add_edge(2, 3);
    gm.add_edge(3, 4);
    gm.display();
    cout << "Edge 0-1? " << gm.has_edge(0, 1) << "  Edge 0-2? " << gm.has_edge(0, 2) << '\n';

    // ---- Adjacency List ----
    cout << "\n--- Adjacency List (undirected, unweighted) ---\n";
    GraphList<int> gl(5);
    gl.add_edge(0, 1);
    gl.add_edge(0, 4);
    gl.add_edge(1, 2);
    gl.add_edge(1, 3);
    gl.add_edge(1, 4);
    gl.add_edge(2, 3);
    gl.add_edge(3, 4);
    gl.display();

    cout << "BFS from 0: "; gl.bfs(0);
    cout << "DFS from 0: "; gl.dfs(0);

    // ---- Weighted Adjacency List ----
    cout << "\n--- Weighted Adjacency List (directed) ---\n";
    GraphList<int> gw(4, true);
    gw.add_edge(0, 1, 5);
    gw.add_edge(0, 2, 3);
    gw.add_edge(1, 3, 2);
    gw.add_edge(2, 3, 7);
    gw.add_edge(3, 0, 1);
    gw.display();

    // ---- Edge List ----
    cout << "\n--- Edge List (undirected, weighted) ---\n";
    EdgeList<int> el(4);
    el.add_edge(0, 1, 10);
    el.add_edge(0, 2, 6);
    el.add_edge(0, 3, 5);
    el.add_edge(1, 3, 15);
    el.add_edge(2, 3, 4);
    el.display();

    cout << "\nSort by weight:\n";
    el.sort_by_weight();
    el.display();

    cout << "\n--- Complexity ---\n";
    cout << "Adjacency Matrix: O(V^2) space, O(1) edge check\n";
    cout << "Adjacency List: O(V+E) space, O(degree) edge check\n";
    cout << "Edge List: O(E) space\n";

    return 0;
}
