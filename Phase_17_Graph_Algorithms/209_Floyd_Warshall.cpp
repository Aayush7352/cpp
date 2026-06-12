/*
 * Phase 17 – Graph Algorithms
 * 209. Floyd-Warshall Algorithm
 *
 * Theory:
 * - All-pairs shortest path algorithm using dynamic programming.
 * - dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]) for k = 0..V-1.
 * - Also can compute transitive closure (reachability).
 * - Time: O(V^3), Space: O(V^2).
 * - Handles negative weights but not negative cycles.
 *
 * Common Mistakes:
 * - Not initializing dist[i][i] = 0.
 * - Using INT_MAX for INF and overflow when adding.
 * - Forgetting to set dist[i][j] = INF when i != j initially.
 * - Wrong loop order (k must be outermost).
 *
 * Interview Questions:
 * 1. Why must k be the outermost loop?
 * 2. Detect negative cycle using Floyd-Warshall (dist[i][i] < 0).
 * 3. Transitive closure of a graph.
 * 4. Shortest path with exactly k edges.
 * 5. Find the shortest cycle in a graph.
 */

#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>

class FloydWarshall {
    int V;
    std::vector<std::vector<long long>> dist;
    std::vector<std::vector<int>> next; // for path reconstruction
    static constexpr long long INF = std::numeric_limits<long long>::max() / 2;

public:
    explicit FloydWarshall(int n) : V(n) {
        dist.assign(V, std::vector<long long>(V, INF));
        next.assign(V, std::vector<int>(V, -1));
        for (int i = 0; i < V; ++i) dist[i][i] = 0;
    }

    void add_edge(int u, int v, long long w) {
        if (w < dist[u][v]) {
            dist[u][v] = w;
            next[u][v] = v;
        }
    }

    void run() {
        for (int k = 0; k < V; ++k) {
            for (int i = 0; i < V; ++i) {
                for (int j = 0; j < V; ++j) {
                    if (dist[i][k] != INF && dist[k][j] != INF &&
                        dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    // Transitive closure (reachability)
    std::vector<std::vector<bool>> transitive_closure() const {
        std::vector<std::vector<bool>> reach(V, std::vector<bool>(V, false));
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                reach[i][j] = (dist[i][j] != INF || i == j);
            }
        }
        return reach;
    }

    long long shortest_distance(int u, int v) const {
        return dist[u][v];
    }

    bool has_negative_cycle() const {
        for (int i = 0; i < V; ++i)
            if (dist[i][i] < 0) return true;
        return false;
    }

    std::vector<int> get_path(int u, int v) const {
        if (dist[u][v] == INF) return {};
        std::vector<int> path = {u};
        while (u != v) {
            u = next[u][v];
            path.push_back(u);
        }
        return path;
    }

    void print_distances() const {
        std::cout << "Distance matrix:\n   ";
        for (int j = 0; j < V; ++j) std::cout << std::setw(5) << j;
        std::cout << '\n';
        for (int i = 0; i < V; ++i) {
            std::cout << i << ": ";
            for (int j = 0; j < V; ++j) {
                if (dist[i][j] == INF)
                    std::cout << std::setw(5) << "INF";
                else
                    std::cout << std::setw(5) << dist[i][j];
            }
            std::cout << '\n';
        }
    }
};

int main() {
    using namespace std;

    cout << "=== Floyd-Warshall Demo ===\n\n";

    // Graph from common example
    FloydWarshall fw(4);
    fw.add_edge(0, 1, 3);
    fw.add_edge(0, 2, 6);
    fw.add_edge(1, 2, 2);
    fw.add_edge(1, 3, 4);
    fw.add_edge(2, 3, 1);

    fw.run();

    fw.print_distances();

    cout << "\nShortest path 0 -> 3:\n  ";
    auto path = fw.get_path(0, 3);
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) cout << " -> ";
        cout << path[i];
    }
    cout << " (dist: " << fw.shortest_distance(0, 3) << ")\n";

    cout << "\nShortest path 1 -> 3:\n  ";
    path = fw.get_path(1, 3);
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) cout << " -> ";
        cout << path[i];
    }
    cout << " (dist: " << fw.shortest_distance(1, 3) << ")\n";

    cout << "\nNegative cycle? " << boolalpha << fw.has_negative_cycle() << '\n';

    // Transitive closure
    cout << "\nTransitive closure (reachability):\n";
    auto reach = fw.transitive_closure();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j)
            cout << reach[i][j] << ' ';
        cout << '\n';
    }

    // Demo with negative edge
    cout << "\n--- Graph with negative edge ---\n";
    FloydWarshall fw2(3);
    fw2.add_edge(0, 1, 4);
    fw2.add_edge(1, 2, -2);
    fw2.add_edge(0, 2, 3);
    fw2.run();
    fw2.print_distances();
    cout << "Shortest path 0->2: " << fw2.shortest_distance(0, 2) << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Time: O(V^3)\n";
    cout << "Space: O(V^2)\n";
    cout << "Finds shortest paths between ALL pairs\n";

    return 0;
}
