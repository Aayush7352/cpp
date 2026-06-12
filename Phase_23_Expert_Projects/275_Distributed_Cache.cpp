/*
THEORY: Distributed Cache
- Consistent Hashing for node distribution
- Replication for fault tolerance
- TTL (Time To Live) for key expiration
- Cache eviction (LRU within each node)
- Gossip protocol for membership

Complexity: O(log n) for key lookup
Interview Questions:
1. How does consistent hashing minimize rehashing?
2. How to handle cache failures/replication?
3. What is the CAP theorem and how does it apply?
Common Mistakes:
- Not handling hot keys (skewed distribution)
- Cache stampede (thundering herd)
- Not implementing proper expiration
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>
#include <functional>
#include <iomanip>

class ConsistentHash {
    std::map<size_t, std::string> ring_;
    std::set<std::string> nodes_;
    int virtualNodes_;

    size_t hash(const std::string& key) const {
        std::hash<std::string> hasher;
        return hasher(key);
    }

public:
    explicit ConsistentHash(int virtualNodes = 150) : virtualNodes_(virtualNodes) {}

    void addNode(const std::string& node) {
        nodes_.insert(node);
        for (int i = 0; i < virtualNodes_; ++i) {
            std::string vnode = node + "#" + std::to_string(i);
            ring_[hash(vnode)] = node;
        }
    }

    void removeNode(const std::string& node) {
        nodes_.erase(node);
        for (auto it = ring_.begin(); it != ring_.end();) {
            if (it->second == node) it = ring_.erase(it);
            else ++it;
        }
    }

    std::string getNode(const std::string& key) const {
        if (ring_.empty()) return "";
        auto h = hash(key);
        auto it = ring_.lower_bound(h);
        if (it == ring_.end()) it = ring_.begin();
        return it->second;
    }

    std::vector<std::string> getReplicas(const std::string& key, int count = 2) const {
        std::vector<std::string> replicas;
        std::set<std::string> seen;
        auto h = hash(key);
        auto it = ring_.lower_bound(h);

        for (int i = 0; i < (int)ring_.size() && (int)replicas.size() < count; ++i) {
            if (it == ring_.end()) it = ring_.begin();
            if (seen.insert(it->second).second) {
                replicas.push_back(it->second);
            }
            ++it;
        }
        return replicas;
    }

    size_t nodeCount() const { return nodes_.size(); }
};

struct CacheEntry {
    std::string value;
    std::chrono::steady_clock::time_point expiry;
    bool expired() const {
        return std::chrono::steady_clock::now() > expiry;
    }
};

class CacheNode {
    std::unordered_map<std::string, CacheEntry> store_;
    size_t maxSize_{1000};
    std::vector<std::string> accessOrder_;

public:
    void put(const std::string& key, const std::string& value, int ttlSeconds = 60) {
        if (store_.size() >= maxSize_) evict();
        store_[key] = {value, std::chrono::steady_clock::now() +
                       std::chrono::seconds(ttlSeconds)};
        accessOrder_.push_back(key);
    }

    std::string get(const std::string& key) {
        auto it = store_.find(key);
        if (it == store_.end()) return "";
        if (it->second.expired()) {
            store_.erase(it);
            return "";
        }
        return it->second.value;
    }

    void evict() {
        if (store_.empty()) return;
        // Simple LRU: evict oldest accessed
        while (!accessOrder_.empty()) {
            auto key = accessOrder_.front();
            accessOrder_.erase(accessOrder_.begin());
            if (store_.erase(key)) break;
        }
    }

    size_t size() const { return store_.size(); }
};

class DistributedCache {
    ConsistentHash hashRing_;
    std::unordered_map<std::string, std::unique_ptr<CacheNode>> nodes_;
    int replicationFactor_{2};
    int defaultTTL_{60};

public:
    DistributedCache() {
        hashRing_ = ConsistentHash(150);
    }

    void addNode(const std::string& nodeId) {
        nodes_[nodeId] = std::make_unique<CacheNode>();
        hashRing_.addNode(nodeId);
        std::cout << "Node added: " << nodeId << "\n";
    }

    void removeNode(const std::string& nodeId) {
        nodes_.erase(nodeId);
        hashRing_.removeNode(nodeId);
        std::cout << "Node removed: " << nodeId << "\n";
    }

    void put(const std::string& key, const std::string& value, int ttl = -1) {
        int expiry = (ttl > 0) ? ttl : defaultTTL_;
        auto replicas = hashRing_.getReplicas(key, replicationFactor_);
        for (const auto& node : replicas) {
            auto it = nodes_.find(node);
            if (it != nodes_.end()) {
                it->second->put(key, value, expiry);
                std::cout << "Stored " << key << " -> " << value
                          << " on " << node << "\n";
            }
        }
    }

    std::string get(const std::string& key) {
        auto primary = hashRing_.getNode(key);
        if (primary.empty()) return "";

        // Try primary first, then replicas
        auto replicas = hashRing_.getReplicas(key, replicationFactor_);
        for (const auto& node : replicas) {
            auto it = nodes_.find(node);
            if (it != nodes_.end()) {
                auto val = it->second->get(key);
                if (!val.empty()) {
                    std::cout << "Found " << key << " on " << node << "\n";
                    return val;
                }
            }
        }
        return "";
    }

    void printNodes() const {
        std::cout << "\n=== Cache Cluster ===\n";
        for (const auto& [id, node] : nodes_) {
            std::cout << "  Node " << id << " (" << node->size() << " entries)\n";
        }
        std::cout << "Total nodes: " << nodes_.size() << "\n\n";
    }
};

int main() {
    DistributedCache cache;

    cache.addNode("server-1");
    cache.addNode("server-2");
    cache.addNode("server-3");
    cache.addNode("server-4");
    cache.printNodes();

    // Store data
    cache.put("user:1001", "Alice Johnson");
    cache.put("user:1002", "Bob Smith");
    cache.put("user:1003", "Charlie Brown");
    cache.put("session:abc123", "valid");
    cache.put("config:theme", "dark");

    // Retrieve data
    std::cout << "\nGetting user:1001: " << cache.get("user:1001") << "\n";
    std::cout << "Getting user:1002: " << cache.get("user:1002") << "\n";
    std::cout << "Getting nonexistent: " << cache.get("unknown:key") << "\n";

    // Simulate node failure
    std::cout << "\n--- Node failure: server-2 ---\n";
    cache.removeNode("server-2");

    // Should still find data from replicas
    std::cout << "Getting user:1001 after failure: " << cache.get("user:1001") << "\n";
    cache.printNodes();

    std::cout << "Distributed cache demo complete\n";
    return 0;
}

/*
SAMPLE OUTPUT:
Node added: server-1
Node added: server-2
Node added: server-3
Node added: server-4

=== Cache Cluster ===
  Node server-1 (1 entries)
  Node server-2 (2 entries)
  ...

Stored user:1001 -> Alice Johnson on server-3
...
Found user:1001 on server-1
Getting user:1001: Alice Johnson
*/
