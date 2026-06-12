/*
 * Phase 16 – Data Structures
 * 195. Hash Table
 *
 * Theory:
 * - Hash table maps keys to values using a hash function.
 * - Collision resolution:
 *   1. Chaining: each bucket holds a linked list (or std::list).
 *   2. Open addressing (linear probing): probe next slot on collision.
 * - Load factor = n / m. Rehash when load factor exceeds threshold.
 * - Average-case: O(1) for insert/search/delete, worst-case O(n).
 *
 * Common Mistakes:
 * - Poor hash function causing clustering.
 * - Not resizing (rehashing) leading to high load factor.
 * - In linear probing: not wrapping around, infinite loops.
 * - Deleting with open addressing: need lazy deletion (tombstone).
 *
 * Interview Questions:
 * 1. Design a hash map from scratch.
 * 2. Differences between chaining and open addressing.
 * 3. How does C++ unordered_map work?
 * 4. Implement LRU cache using hash map + doubly linked list.
 * 5. Find first non-repeating character using hash map.
 */

#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <optional>
#include <string>

// ========== Hash Table with Chaining ==========
template<typename Key, typename Value, typename Hash = std::hash<Key>>
class ChainingHashTable {
    struct Bucket {
        std::list<std::pair<Key, Value>> items;
    };

    std::vector<Bucket> buckets;
    size_t num_elements;
    Hash hasher;
    static constexpr double MAX_LOAD_FACTOR = 1.0;

    void rehash() {
        size_t new_capacity = buckets.size() * 2 + 1;
        std::vector<Bucket> new_buckets(new_capacity);

        for (auto& bucket : buckets) {
            for (auto& [key, value] : bucket.items) {
                size_t idx = hasher(key) % new_capacity;
                new_buckets[idx].items.push_back({key, value});
            }
        }
        buckets = std::move(new_buckets);
    }

public:
    explicit ChainingHashTable(size_t initial_capacity = 16)
        : buckets(initial_capacity), num_elements(0) {}

    void insert(const Key& key, const Value& value) {
        if (static_cast<double>(num_elements) / buckets.size() >= MAX_LOAD_FACTOR)
            rehash();

        size_t idx = hasher(key) % buckets.size();
        for (auto& [k, v] : buckets[idx].items) {
            if (k == key) {
                v = value; // update
                return;
            }
        }
        buckets[idx].items.push_back({key, value});
        ++num_elements;
    }

    bool contains(const Key& key) const {
        size_t idx = hasher(key) % buckets.size();
        for (const auto& [k, v] : buckets[idx].items) {
            if (k == key) return true;
        }
        return false;
    }

    std::optional<Value> get(const Key& key) const {
        size_t idx = hasher(key) % buckets.size();
        for (const auto& [k, v] : buckets[idx].items) {
            if (k == key) return v;
        }
        return std::nullopt;
    }

    bool erase(const Key& key) {
        size_t idx = hasher(key) % buckets.size();
        auto& items = buckets[idx].items;
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->first == key) {
                items.erase(it);
                --num_elements;
                return true;
            }
        }
        return false;
    }

    size_t size() const { return num_elements; }
    size_t capacity() const { return buckets.size(); }
    bool empty() const { return num_elements == 0; }

    void display() const {
        for (size_t i = 0; i < buckets.size(); ++i) {
            if (!buckets[i].items.empty()) {
                std::cout << "[" << i << "]: ";
                for (const auto& [k, v] : buckets[i].items) {
                    std::cout << "(" << k << "," << v << ") ";
                }
                std::cout << '\n';
            }
        }
    }
};

// ========== Hash Table with Open Addressing (Linear Probing) ==========
enum class SlotState { EMPTY, OCCUPIED, DELETED };

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class OpenAddressingHashTable {
    struct Slot {
        Key key;
        Value value;
        SlotState state;

        Slot() : state(SlotState::EMPTY) {}
    };

    std::vector<Slot> table;
    size_t num_elements;
    Hash hasher;
    static constexpr double MAX_LOAD_FACTOR = 0.7;

    void rehash() {
        size_t new_capacity = table.size() * 2 + 1;
        std::vector<Slot> new_table(new_capacity);

        for (const auto& slot : table) {
            if (slot.state != SlotState::OCCUPIED) continue;
            size_t idx = hasher(slot.key) % new_capacity;
            while (new_table[idx].state == SlotState::OCCUPIED) {
                idx = (idx + 1) % new_capacity;
            }
            new_table[idx].key = slot.key;
            new_table[idx].value = slot.value;
            new_table[idx].state = SlotState::OCCUPIED;
        }
        table = std::move(new_table);
    }

public:
    explicit OpenAddressingHashTable(size_t initial_capacity = 16)
        : table(initial_capacity), num_elements(0) {}

    void insert(const Key& key, const Value& value) {
        if (static_cast<double>(num_elements) / table.size() >= MAX_LOAD_FACTOR)
            rehash();

        size_t idx = hasher(key) % table.size();
        size_t start = idx;
        while (table[idx].state == SlotState::OCCUPIED) {
            if (table[idx].key == key) {
                table[idx].value = value; // update
                return;
            }
            idx = (idx + 1) % table.size();
            if (idx == start) throw std::runtime_error("Table full");
        }
        table[idx].key = key;
        table[idx].value = value;
        table[idx].state = SlotState::OCCUPIED;
        ++num_elements;
    }

    bool contains(const Key& key) const {
        size_t idx = hasher(key) % table.size();
        size_t start = idx;
        while (table[idx].state != SlotState::EMPTY) {
            if (table[idx].state == SlotState::OCCUPIED && table[idx].key == key)
                return true;
            idx = (idx + 1) % table.size();
            if (idx == start) break;
        }
        return false;
    }

    std::optional<Value> get(const Key& key) const {
        size_t idx = hasher(key) % table.size();
        size_t start = idx;
        while (table[idx].state != SlotState::EMPTY) {
            if (table[idx].state == SlotState::OCCUPIED && table[idx].key == key)
                return table[idx].value;
            idx = (idx + 1) % table.size();
            if (idx == start) break;
        }
        return std::nullopt;
    }

    bool erase(const Key& key) {
        size_t idx = hasher(key) % table.size();
        size_t start = idx;
        while (table[idx].state != SlotState::EMPTY) {
            if (table[idx].state == SlotState::OCCUPIED && table[idx].key == key) {
                table[idx].state = SlotState::DELETED;
                --num_elements;
                return true;
            }
            idx = (idx + 1) % table.size();
            if (idx == start) break;
        }
        return false;
    }

    size_t size() const { return num_elements; }
    size_t capacity() const { return table.size(); }
    bool empty() const { return num_elements == 0; }

    void display() const {
        for (size_t i = 0; i < table.size(); ++i) {
            std::cout << "[" << i << "]: ";
            if (table[i].state == SlotState::OCCUPIED)
                std::cout << "(" << table[i].key << "," << table[i].value << ")";
            else if (table[i].state == SlotState::DELETED)
                std::cout << "(DELETED)";
            else
                std::cout << "(EMPTY)";
            std::cout << '\n';
        }
    }
};

int main() {
    using namespace std;

    cout << "=== Hash Table Demo ===\n\n";

    // ---- Chaining ----
    cout << "--- Chaining Hash Table ---\n";
    ChainingHashTable<string, int> ht;
    ht.insert("Alice", 25);
    ht.insert("Bob", 30);
    ht.insert("Charlie", 35);
    ht.insert("Diana", 28);
    ht.insert("Eve", 22);
    ht.display();

    cout << "\nContains 'Bob'? " << boolalpha << ht.contains("Bob") << '\n';
    cout << "Get 'Charlie': " << ht.get("Charlie").value_or(-1) << '\n';
    cout << "Get 'Nonexistent': " << ht.get("Nonexistent").value_or(-1) << '\n';

    ht.erase("Bob");
    cout << "After erase 'Bob', contains? " << ht.contains("Bob") << '\n';
    cout << "Size: " << ht.size() << '\n';

    // ---- Open Addressing ----
    cout << "\n--- Open Addressing (Linear Probing) ---\n";
    OpenAddressingHashTable<string, int> oa;
    oa.insert("Apple", 10);
    oa.insert("Banana", 20);
    oa.insert("Cherry", 30);
    oa.insert("Date", 40);
    oa.insert("Elderberry", 50);
    oa.display();

    cout << "\nGet 'Banana': " << oa.get("Banana").value_or(-1) << '\n';
    cout << "Erase 'Cherry': " << oa.erase("Cherry") << '\n';
    cout << "Contains 'Cherry' after erase? " << oa.contains("Cherry") << '\n';
    cout << "Size: " << oa.size() << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Average-case insert/search/delete: O(1)\n";
    cout << "Worst-case: O(n)\n";
    cout << "Space: O(m + n) chaining, O(m) open addressing\n";

    return 0;
}
