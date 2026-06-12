/*
 * Phase 16 – Data Structures
 * 197. Binary Search Tree (BST)
 *
 * Theory:
 * - For every node: left subtree < node < right subtree.
 * - Operations: insert O(h), search O(h), delete O(h).
 * - Delete cases:
 *   1. Leaf: just remove.
 *   2. One child: replace with child.
 *   3. Two children: replace with inorder successor (or predecessor).
 * - Inorder traversal of BST gives sorted order.
 *
 * Common Mistakes:
 * - Not updating parent pointers during deletion.
 * - Choosing wrong inorder successor (deepest left of right subtree).
 * - Not handling empty tree.
 * - Degenerate BST (skewed) → O(n) operations.
 *
 * Interview Questions:
 * 1. Validate BST (check if tree is BST).
 * 2. Find inorder predecessor/successor in BST.
 * 3. Find LCA in BST.
 * 4. Convert BST to sorted doubly linked list.
 * 5. Find kth smallest element in BST.
 */

#include <iostream>
#include <stack>
#include <optional>

template<typename T>
struct BSTNode {
    T data;
    BSTNode* left;
    BSTNode* right;

    explicit BSTNode(const T& val) : data(val), left(nullptr), right(nullptr) {}
};

template<typename T>
class BST {
    BSTNode<T>* root;

    BSTNode<T>* insert_rec(BSTNode<T>* node, const T& val) {
        if (!node) return new BSTNode<T>(val);
        if (val < node->data)
            node->left = insert_rec(node->left, val);
        else if (val > node->data)
            node->right = insert_rec(node->right, val);
        // if equal, do nothing (no duplicates)
        return node;
    }

    BSTNode<T>* search_rec(BSTNode<T>* node, const T& val) const {
        if (!node || node->data == val) return node;
        if (val < node->data) return search_rec(node->left, val);
        return search_rec(node->right, val);
    }

    BSTNode<T>* delete_rec(BSTNode<T>* node, const T& val) {
        if (!node) return nullptr;

        if (val < node->data) {
            node->left = delete_rec(node->left, val);
        } else if (val > node->data) {
            node->right = delete_rec(node->right, val);
        } else {
            // Case 1: leaf
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
            // Case 2: one child
            if (!node->left) {
                BSTNode<T>* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                BSTNode<T>* temp = node->left;
                delete node;
                return temp;
            }
            // Case 3: two children – replace with inorder successor
            BSTNode<T>* successor = find_min(node->right);
            node->data = successor->data;
            node->right = delete_rec(node->right, successor->data);
        }
        return node;
    }

    BSTNode<T>* find_min(BSTNode<T>* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    BSTNode<T>* find_max(BSTNode<T>* node) const {
        while (node && node->right) node = node->right;
        return node;
    }

    void inorder_rec(BSTNode<T>* node) const {
        if (!node) return;
        inorder_rec(node->left);
        std::cout << node->data << ' ';
        inorder_rec(node->right);
    }

    void clear_rec(BSTNode<T>* node) {
        if (!node) return;
        clear_rec(node->left);
        clear_rec(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { clear_rec(root); }

    void insert(const T& val) {
        root = insert_rec(root, val);
    }

    bool search(const T& val) const {
        return search_rec(root, val) != nullptr;
    }

    void remove(const T& val) {
        root = delete_rec(root, val);
    }

    void inorder() const {
        std::cout << "Inorder: ";
        inorder_rec(root);
        std::cout << '\n';
    }

    // Inorder successor
    std::optional<T> inorder_successor(const T& val) const {
        BSTNode<T>* target = search_rec(root, val);
        if (!target) return std::nullopt;

        // If right subtree exists, successor is min of right subtree
        if (target->right) {
            return find_min(target->right)->data;
        }

        // Otherwise, walk from root tracking the last left-turn
        BSTNode<T>* successor = nullptr;
        BSTNode<T>* curr = root;
        while (curr) {
            if (val < curr->data) {
                successor = curr;
                curr = curr->left;
            } else if (val > curr->data) {
                curr = curr->right;
            } else {
                break;
            }
        }
        if (successor) return successor->data;
        return std::nullopt;
    }

    // Inorder predecessor
    std::optional<T> inorder_predecessor(const T& val) const {
        BSTNode<T>* target = search_rec(root, val);
        if (!target) return std::nullopt;

        if (target->left) {
            return find_max(target->left)->data;
        }

        BSTNode<T>* predecessor = nullptr;
        BSTNode<T>* curr = root;
        while (curr) {
            if (val > curr->data) {
                predecessor = curr;
                curr = curr->right;
            } else if (val < curr->data) {
                curr = curr->left;
            } else {
                break;
            }
        }
        if (predecessor) return predecessor->data;
        return std::nullopt;
    }

    // Validate BST
    bool is_valid_bst() const {
        auto validate = [](BSTNode<T>* node, auto&& self, T* min, T* max) -> bool {
            if (!node) return true;
            if (min && node->data <= *min) return false;
            if (max && node->data >= *max) return false;
            return self(node->left, self, min, &node->data) &&
                   self(node->right, self, &node->data, max);
        };
        return validate(root, validate, nullptr, nullptr);
    }

    // Kth smallest (1-indexed)
    std::optional<T> kth_smallest(int k) const {
        std::stack<BSTNode<T>*> st;
        BSTNode<T>* curr = root;
        int count = 0;
        while (curr || !st.empty()) {
            while (curr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top(); st.pop();
            ++count;
            if (count == k) return curr->data;
            curr = curr->right;
        }
        return std::nullopt;
    }
};

int main() {
    using namespace std;

    cout << "=== BST Demo ===\n\n";

    BST<int> bst;

    // Insert values
    vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 25, 55, 75};
    for (int v : values) bst.insert(v);

    cout << "Inserted: ";
    for (int v : values) cout << v << ' ';
    cout << '\n';
    bst.inorder();

    // Search
    cout << "\nSearch 40: " << (bst.search(40) ? "found" : "not found") << '\n';
    cout << "Search 99: " << (bst.search(99) ? "found" : "not found") << '\n';

    // Inorder successor/predecessor
    cout << "\nInorder successor of 40: ";
    auto s = bst.inorder_successor(40);
    if (s) cout << *s << '\n';

    cout << "Inorder predecessor of 40: ";
    auto p = bst.inorder_predecessor(40);
    if (p) cout << *p << '\n';

    cout << "Inorder successor of 80: ";
    s = bst.inorder_successor(80);
    if (s) cout << *s << '\n';

    cout << "Inorder predecessor of 10: ";
    p = bst.inorder_predecessor(10);
    if (p) cout << *p << '\n';

    // Delete cases
    cout << "\nDelete leaf 10\n";
    bst.remove(10);
    bst.inorder();

    cout << "Delete node with one child (20)\n";
    bst.remove(20);
    bst.inorder();

    cout << "Delete node with two children (50)\n";
    bst.remove(50);
    bst.inorder();

    // Kth smallest
    cout << "\n2nd smallest: ";
    auto k = bst.kth_smallest(2);
    if (k) cout << *k << '\n';

    cout << "4th smallest: ";
    k = bst.kth_smallest(4);
    if (k) cout << *k << '\n';

    // Validate BST
    cout << "\nIs valid BST? " << boolalpha << bst.is_valid_bst() << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Insert/Search/Delete: O(h) = O(log n) average, O(n) worst\n";
    cout << "Inorder traversal: O(n)\n";
    cout << "Inorder successor/predecessor: O(h)\n";
    cout << "Kth smallest: O(n)\n";

    return 0;
}
