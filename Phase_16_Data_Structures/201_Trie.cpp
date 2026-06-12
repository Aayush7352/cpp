/*
 * Phase 16 – Data Structures
 * 201. Trie (Prefix Tree)
 *
 * Theory:
 * - Trie stores strings in a tree where each node represents a character.
 * - Root is empty; each path from root to leaf spells a word.
 * - Operations: insert, search, prefix search, delete, auto-complete.
 * - Time: O(length of word) for all operations.
 * - Space: O(total characters across all words).
 *
 * Common Mistakes:
 * - Not marking end-of-word flag.
 * - Memory leaks from not deleting child nodes.
 * - Incorrect delete logic (only remove nodes that aren't part of other words).
 * - Case-sensitivity issues.
 *
 * Interview Questions:
 * 1. Implement auto-complete / prefix search.
 * 2. Find all words matching a pattern (with wildcards).
 * 3. Longest common prefix of all strings.
 * 4. Count distinct substrings of a string.
 * 5. Implement a spell checker.
 */

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool is_end;
    int prefix_count; // number of words passing through this node

    TrieNode() : is_end(false), prefix_count(0) {}

    ~TrieNode() {
        for (auto& [ch, child] : children) {
            delete child;
        }
    }
};

class Trie {
    TrieNode* root;

    void collect_words(TrieNode* node, std::string& prefix, std::vector<std::string>& result) const {
        if (node->is_end) result.push_back(prefix);
        for (auto& [ch, child] : node->children) {
            prefix.push_back(ch);
            collect_words(child, prefix, result);
            prefix.pop_back();
        }
    }

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { delete root; }

    void insert(const std::string& word) {
        TrieNode* curr = root;
        for (char ch : word) {
            if (!curr->children.contains(ch))
                curr->children[ch] = new TrieNode();
            curr = curr->children[ch];
            curr->prefix_count++;
        }
        curr->is_end = true;
    }

    bool search(const std::string& word) const {
        TrieNode* curr = root;
        for (char ch : word) {
            if (!curr->children.contains(ch)) return false;
            curr = curr->children[ch];
        }
        return curr->is_end;
    }

    bool starts_with(const std::string& prefix) const {
        TrieNode* curr = root;
        for (char ch : prefix) {
            if (!curr->children.contains(ch)) return false;
            curr = curr->children[ch];
        }
        return true;
    }

    int count_prefix(const std::string& prefix) const {
        TrieNode* curr = root;
        for (char ch : prefix) {
            if (!curr->children.contains(ch)) return 0;
            curr = curr->children[ch];
        }
        return curr->prefix_count;
    }

    bool remove(const std::string& word) {
        // Returns true if word was found and removed
        if (!search(word)) return false;

        auto remove_rec = [](TrieNode* node, const std::string& w, size_t depth, auto&& self) -> bool {
            if (depth == w.size()) {
                if (!node->is_end) return false;
                node->is_end = false;
                return node->children.empty();
            }
            char ch = w[depth];
            auto it = node->children.find(ch);
            if (it == node->children.end()) return false;

            bool should_delete = self(it->second, w, depth + 1, self);
            if (should_delete) {
                delete it->second;
                node->children.erase(it);
                return !node->is_end && node->children.empty();
            }
            return false;
        };

        remove_rec(root, word, 0, remove_rec);
        return true;
    }

    std::vector<std::string> auto_complete(const std::string& prefix) const {
        std::vector<std::string> result;
        TrieNode* curr = root;
        for (char ch : prefix) {
            if (!curr->children.contains(ch)) return result;
            curr = curr->children[ch];
        }
        std::string pref = prefix;
        collect_words(curr, pref, result);
        return result;
    }

    std::vector<std::string> get_all_words() const {
        std::vector<std::string> result;
        std::string prefix;
        collect_words(root, prefix, result);
        return result;
    }

    bool empty() const {
        return root->children.empty() && !root->is_end;
    }
};

int main() {
    using namespace std;

    cout << "=== Trie Demo ===\n\n";

    Trie trie;

    // Insert words
    vector<string> words = {"apple", "app", "apricot", "application", "apt", "bat", "ball", "batman"};
    cout << "Inserting words: ";
    for (const auto& w : words) {
        cout << w << ' ';
        trie.insert(w);
    }
    cout << '\n';

    // Search
    cout << "\nSearch:\n";
    for (const auto& w : {"apple", "app", "apt", "apx", "bat", "cat"}) {
        cout << "  \"" << w << "\": " << (trie.search(w) ? "found" : "not found") << '\n';
    }

    // Prefix search
    cout << "\nPrefix search:\n";
    for (const auto& p : {"ap", "ba", "cat", "app"}) {
        cout << "  \"" << p << "\": " << (trie.starts_with(p) ? "yes" : "no") << '\n';
    }

    // Count prefix
    cout << "\nPrefix count:\n";
    cout << "  \"ap\": " << trie.count_prefix("ap") << '\n';
    cout << "  \"app\": " << trie.count_prefix("app") << '\n';

    // Auto-complete
    cout << "\nAuto-complete for \"ap\":\n  ";
    for (const auto& w : trie.auto_complete("ap")) {
        cout << w << ' ';
    }
    cout << '\n';

    cout << "Auto-complete for \"bat\":\n  ";
    for (const auto& w : trie.auto_complete("bat")) {
        cout << w << ' ';
    }
    cout << '\n';

    // Delete
    cout << "\nDelete \"app\": " << (trie.remove("app") ? "success" : "fail") << '\n';
    cout << "Search \"app\" after delete: " << (trie.search("app") ? "found" : "not found") << '\n';
    cout << "Search \"apple\" after delete: " << (trie.search("apple") ? "found" : "not found") << '\n';
    cout << "Prefix \"ap\" still starts? " << (trie.starts_with("ap") ? "yes" : "no") << '\n';

    // All words
    cout << "\nAll words in trie:\n  ";
    for (const auto& w : trie.get_all_words()) {
        cout << w << ' ';
    }
    cout << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Insert/Search/Delete: O(L) where L = word length\n";
    cout << "Prefix search / auto-complete: O(L + results)\n";
    cout << "Space: O(total characters)\n";

    return 0;
}
