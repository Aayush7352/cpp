/*
 * Phase 17 – Graph Algorithms
 * 212. Topological Sort
 *
 * Theory:
 * - Linear ordering of vertices in a DAG (Directed Acyclic Graph).
 * - If u → v, then u comes before v in the ordering.
 * - Two algorithms:
 *   1. Kahn's algorithm (BFS-based): use in-degree array, queue.
 *   2. DFS-based: push to stack after visiting all descendants (post-order).
 * - Applications: build systems, course scheduling, dependency resolution.
 * - Time: O(V + E), Space: O(V).
 *
 * Common Mistakes:
 * - Applying to graph with cycles (no valid topological order exists).
 * - DFS-based: forgetting to reverse the result.
 * - Kahn's: not updating in-degrees correctly.
 *
 * Interview Questions:
 * 1. Find all possible topological orders.
 * 2. Detect cycle in directed graph (topological sort fails).
 * 3. Find the minimum number of semesters to complete courses.
 * 4. Alien dictionary (topological order of characters).
 * 5. Find topological order with lexicographically smallest.
 */

#include <iostream>
#include <vector>
#include <queue>
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

    // Kahn's Algorithm (BFS-based)
    std::vector<int> topological_sort_kahn() const {
        std::vector<int> in_degree(V, 0);
        for (int u = 0; u < V; ++u)
            for (int v : adj[u])
                in_degree[v]++;

        std::queue<int> q;
        for (int i = 0; i < V; ++i)
            if (in_degree[i] == 0) q.push(i);

        std::vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (--in_degree[v] == 0)
                    q.push(v);
            }
        }

        if (static_cast<size_t>(order.size()) != static_cast<size_t>(V))
            order.clear(); // cycle detected
        return order;
    }

    // DFS-based topological sort
    std::vector<int> topological_sort_dfs() const {
        std::vector<bool> visited(V, false);
        std::vector<int> order;
        order.reserve(V);

        std::function<void(int)> dfs = [&](int u) {
            visited[u] = true;
            for (int v : adj[u])
                if (!visited[v]) dfs(v);
            order.push_back(u);
        };

        for (int i = 0; i < V; ++i)
            if (!visited[i]) dfs(i);

        std::reverse(order.begin(), order.end());

        // Verify (check remaining edges)
        if (static_cast<size_t>(order.size()) != static_cast<size_t>(V))
            order.clear();
        return order;
    }

    // Check if cycle exists (can't topologically sort)
    bool has_cycle() const {
        return topological_sort_kahn().empty();
    }

    // Find lexicographically smallest topological order
    std::vector<int> topological_sort_lexicographically_smallest() const {
        std::vector<int> in_degree(V, 0);
        for (int u = 0; u < V; ++u)
            for (int v : adj[u])
                in_degree[v]++;

        std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
        for (int i = 0; i < V; ++i)
            if (in_degree[i] == 0) pq.push(i);

        std::vector<int> order;
        while (!pq.empty()) {
            int u = pq.top(); pq.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (--in_degree[v] == 0)
                    pq.push(v);
            }
        }

        if (static_cast<size_t>(order.size()) != static_cast<size_t>(V))
            order.clear();
        return order;
    }

    void display() const {
        for (int i = 0; i < V; ++i) {
            std::cout << i << ": ";
            for (int v : adj[i]) std::cout << v << ' ';
            std::cout << '\n';
        }
    }
};

int main() {
    using namespace std;

    cout << "=== Topological Sort Demo ===\n\n";

    // DAG for course prerequisites
    // 0: CS101, 1: CS102, 2: CS201, 3: MATH101, 4: MATH201, 5: CS301
    Graph courses(6);
    courses.add_edge(0, 1);  // CS101 → CS102
    courses.add_edge(1, 2);  // CS102 → CS201
    courses.add_edge(3, 4);  // MATH101 → MATH201
    courses.add_edge(1, 5);  // CS102 → CS301
    courses.add_edge(4, 5);  // MATH201 → CS301
    courses.add_edge(2, 5);  // CS201 → CS301

    cout << "Course dependencies:\n";
    courses.display();

    cout << "\nKahn's topological order: ";
    auto order = courses.topological_sort_kahn();
    for (int v : order) cout << v << ' ';
    cout << '\n';

    cout << "DFS topological order:   ";
    order = courses.topological_sort_dfs();
    for (int v : order) cout << v << ' ';
    cout << '\n';

    cout << "Lexicographically smallest: ";
    order = courses.topological_sort_lexicographically_smallest();
    for (int v : order) cout << v << ' ';
    cout << '\n';

    // Another DAG example
    cout << "\n--- DAG: 5->2, 5->0, 4->0, 4->1, 2->3, 3->1 ---\n";
    Graph dag(6);
    dag.add_edge(5, 2);
    dag.add_edge(5, 0);
    dag.add_edge(4, 0);
    dag.add_edge(4, 1);
    dag.add_edge(2, 3);
    dag.add_edge(3, 1);

    cout << "Kahn:  ";
    for (int v : dag.topological_sort_kahn()) cout << v << ' ';
    cout << '\n';
    cout << "DFS:   ";
    for (int v : dag.topological_sort_dfs()) cout << v << ' ';
    cout << '\n';

    // Graph with cycle
    cout << "\n--- Graph with cycle ---\n";
    Graph cyclic(3);
    cyclic.add_edge(0, 1);
    cyclic.add_edge(1, 2);
    cyclic.add_edge(2, 0);
    cout << "Kahn result: ";
    order = cyclic.topological_sort_kahn();
    if (order.empty()) cout << "(cycle detected, no valid order)\n";
    else for (int v : order) cout << v << ' ';
    cout << '\n';

    cout << "Has cycle? " << boolalpha << cyclic.has_cycle() << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(V + E)\n";
    cout << "Space: O(V)\n";
    cout << "Only applicable to DAGs\n";

    return 0;
}
