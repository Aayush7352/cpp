/*
 * Phase 17 – Graph Algorithms
 * 207. Dijkstra's Shortest Path Algorithm
 *
 * Theory:
 * - Finds shortest paths from source to all vertices in graph with non-negative weights.
 * - Uses a priority queue (min-heap) to greedily pick the closest unvisited vertex.
 * - Relaxes outgoing edges: if dist[v] > dist[u] + w(u,v), update.
 * - Time: O((V + E) log V) with binary heap, O(V^2) without PQ.
 * - Space: O(V + E).
 * - Does NOT work with negative weights (use Bellman-Ford).
 *
 * Common Mistakes:
 * - Using priority_queue with default max-heap (need std::greater).
 * - Not using `visited` array can cause excessive relaxations.
 * - Integer overflow for large distances (use long long / int64_t).
 * - Modifying priority queue by push instead of decrease-key (OK with lazy deletion).
 *
 * Interview Questions:
 * 1. Dijkstra vs Bellman-Ford vs Floyd-Warshall.
 * 2. Number of shortest paths from source to target.
 * 3. Dijkstra on grid (movement in 4 directions).
 * 4. Network delay time problem.
 * 5. Cheapest flight with at most k stops.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using WeightedGraph = std::vector<std::vector<std::pair<int, int>>>;

class Dijkstra {
    int V;
    WeightedGraph adj;

public:
    Dijkstra(int n) : V(n), adj(n) {}

    void add_edge(int u, int v, int w) {
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w); // undirected by default
    }

    struct Result {
        std::vector<long long> dist;
        std::vector<int> parent;
        bool reachable;
    };

    Result shortest_path(int source) const {
        std::vector<long long> dist(V, std::numeric_limits<long long>::max());
        std::vector<int> parent(V, -1);
        std::vector<bool> visited(V, false);

        // Min-heap: (distance, vertex)
        using P = std::pair<long long, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

        dist[source] = 0;
        pq.emplace(0, source);

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (visited[u]) continue;
            visited[u] = true;

            for (auto [v, w] : adj[u]) {
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    parent[v] = u;
                    pq.emplace(dist[v], v);
                }
            }
        }

        return {dist, parent, true};
    }

    // Reconstruct path from source to target
    std::vector<int> get_path(const Result& res, int target) const {
        std::vector<int> path;
        if (res.dist[target] == std::numeric_limits<long long>::max())
            return path; // unreachable

        for (int v = target; v != -1; v = res.parent[v])
            path.push_back(v);
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Directed version
    void add_directed_edge(int u, int v, int w) {
        adj[u].emplace_back(v, w);
    }
};

int main() {
    using namespace std;

    cout << "=== Dijkstra's Shortest Path Demo ===\n\n";

    Dijkstra dijkstra(6);

    // Graph:
    //     2
    // 0 --- 1
    // |    /|\
    // 4  1 | 3
    // | /   |  
    // 2 --- 3
    //   1     1
    //         |
    //         4

    dijkstra.add_edge(0, 1, 2);
    dijkstra.add_edge(0, 2, 4);
    dijkstra.add_edge(1, 2, 1);
    dijkstra.add_edge(1, 3, 3);
    dijkstra.add_edge(2, 3, 1);
    dijkstra.add_edge(3, 4, 1);
    dijkstra.add_edge(3, 5, 2);
    dijkstra.add_edge(4, 5, 3);

    int source = 0;
    auto result = dijkstra.shortest_path(source);

    cout << "Shortest distances from node " << source << ":\n";
    for (int i = 0; i < 6; ++i) {
        cout << "  to " << i << ": ";
        if (result.dist[i] == numeric_limits<long long>::max())
            cout << "INF";
        else
            cout << result.dist[i];
        cout << '\n';
    }

    cout << "\nPath from " << source << " to 5:\n  ";
    auto path = dijkstra.get_path(result, 5);
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) cout << " -> ";
        cout << path[i];
    }
    cout << '\n';

    cout << "\nPath from " << source << " to 4:\n  ";
    path = dijkstra.get_path(result, 4);
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) cout << " -> ";
        cout << path[i];
    }
    cout << '\n';

    // Directed graph demo
    cout << "\n--- Directed Graph ---\n";
    Dijkstra dg(4);
    dg.add_directed_edge(0, 1, 5);
    dg.add_directed_edge(0, 2, 3);
    dg.add_directed_edge(1, 3, 2);
    dg.add_directed_edge(2, 3, 7);

    auto res2 = dg.shortest_path(0);
    cout << "Shortest distances from 0:\n";
    for (int i = 0; i < 4; ++i)
        cout << "  to " << i << ": " << res2.dist[i] << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O((V+E) log V) with binary heap\n";
    cout << "Space: O(V + E)\n";
    cout << "Note: Does not handle negative weights\n";

    return 0;
}
