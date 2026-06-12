/*
 * Phase 16 – Data Structures
 * 198. AVL Tree
 *
 * Theory:
 * - Self-balancing BST where |balance factor| ≤ 1 for every node.
 * - Balance factor = height(left) - height(right).
 * - Rotations:
 *   - LL (Right rotation): when inserted into left subtree of left child.
 *   - RR (Left rotation): inserted into right subtree of right child.
 *   - LR (Left-Right): left rotation on left child, then right rotation on node.
 *   - RL (Right-Left): right rotation on right child, then left rotation on node.
 * - Insert/Delete: O(log n). After insertion/deletion, update heights and rebalance.
 *
 * Common Mistakes:
 * - Not updating heights after rotations.
 * - Wrong balance factor signs.
 * - Forgetting to rebalance after deletion.
 * - Infinite loop due to incorrect rotation logic.
 *
 * Interview Questions:
 * 1. Compare AVL vs Red-Black trees.
 * 2. Implement all four rotations.
 * 3. Why is AVL more strictly balanced than Red-Black?
 * 4. Show the tree after a sequence of insertions/deletions.
 */

#include <iostream>
#include <algorithm>
#include <queue>

template<typename T>
struct AVLNode {
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;

    explicit AVLNode(const T& val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

template<typename T>
class AVLTree {
    AVLNode<T>* root;

    int get_height(AVLNode<T>* node) const {
        return node ? node->height : 0;
    }

    int get_balance(AVLNode<T>* node) const {
        return node ? get_height(node->left) - get_height(node->right) : 0;
    }

    void update_height(AVLNode<T>* node) {
        if (node)
            node->height = 1 + std::max(get_height(node->left), get_height(node->right));
    }

    // Right rotation (LL case)
    AVLNode<T>* rotate_right(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        update_height(y);
        update_height(x);

        return x;
    }

    // Left rotation (RR case)
    AVLNode<T>* rotate_left(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        update_height(x);
        update_height(y);

        return y;
    }

    AVLNode<T>* insert_rec(AVLNode<T>* node, const T& val) {
        if (!node) return new AVLNode<T>(val);

        if (val < node->data)
            node->left = insert_rec(node->left, val);
        else if (val > node->data)
            node->right = insert_rec(node->right, val);
        else
            return node; // no duplicates

        update_height(node);

        int bal = get_balance(node);

        // LL case
        if (bal > 1 && val < node->left->data)
            return rotate_right(node);

        // RR case
        if (bal < -1 && val > node->right->data)
            return rotate_left(node);

        // LR case
        if (bal > 1 && val > node->left->data) {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }

        // RL case
        if (bal < -1 && val < node->right->data) {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }

        return node;
    }

    AVLNode<T>* find_min(AVLNode<T>* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    AVLNode<T>* delete_rec(AVLNode<T>* node, const T& val) {
        if (!node) return nullptr;

        if (val < node->data) {
            node->left = delete_rec(node->left, val);
        } else if (val > node->data) {
            node->right = delete_rec(node->right, val);
        } else {
            if (!node->left || !node->right) {
                AVLNode<T>* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                AVLNode<T>* successor = find_min(node->right);
                node->data = successor->data;
                node->right = delete_rec(node->right, successor->data);
            }
        }

        if (!node) return nullptr;

        update_height(node);

        int bal = get_balance(node);

        // LL
        if (bal > 1 && get_balance(node->left) >= 0)
            return rotate_right(node);

        // LR
        if (bal > 1 && get_balance(node->left) < 0) {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }

        // RR
        if (bal < -1 && get_balance(node->right) <= 0)
            return rotate_left(node);

        // RL
        if (bal < -1 && get_balance(node->right) > 0) {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }

        return node;
    }

    void inorder_rec(AVLNode<T>* node) const {
        if (!node) return;
        inorder_rec(node->left);
        std::cout << node->data << ' ';
        inorder_rec(node->right);
    }

    void clear_rec(AVLNode<T>* node) {
        if (!node) return;
        clear_rec(node->left);
        clear_rec(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { clear_rec(root); }

    void insert(const T& val) {
        root = insert_rec(root, val);
    }

    void remove(const T& val) {
        root = delete_rec(root, val);
    }

    bool search(const T& val) const {
        AVLNode<T>* curr = root;
        while (curr) {
            if (val == curr->data) return true;
            if (val < curr->data) curr = curr->left;
            else curr = curr->right;
        }
        return false;
    }

    void inorder() const {
        std::cout << "Inorder: ";
        inorder_rec(root);
        std::cout << '\n';
    }

    void level_order() const {
        std::cout << "Level-order: ";
        if (!root) return;
        std::queue<AVLNode<T>*> q;
        q.push(root);
        while (!q.empty()) {
            AVLNode<T>* curr = q.front(); q.pop();
            std::cout << curr->data << "(" << curr->height << ") ";
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        std::cout << '\n';
    }
};

int main() {
    using namespace std;

    cout << "=== AVL Tree Demo ===\n\n";

    AVLTree<int> avl;

    // Insertions that would cause various rotations
    cout << "Inserting: 10, 20, 30, 40, 50, 25\n";
    for (int v : {10, 20, 30, 40, 50, 25}) {
        avl.insert(v);
        cout << "After inserting " << v << ": ";
        avl.inorder();
    }

    cout << "\nLevel order (value with height):\n";
    avl.level_order();

    cout << "\nSearch 25: " << (avl.search(25) ? "found" : "not found") << '\n';
    cout << "Search 99: " << (avl.search(99) ? "found" : "not found") << '\n';

    cout << "\nDeleting 40\n";
    avl.remove(40);
    avl.inorder();
    avl.level_order();

    cout << "\nDeleting 20\n";
    avl.remove(20);
    avl.inorder();
    avl.level_order();

    cout << "\nDeleting 10\n";
    avl.remove(10);
    avl.inorder();
    avl.level_order();

    // Demonstrate balanced nature
    cout << "\nInserting 1..15 to show balanced tree\n";
    AVLTree<int> avl2;
    for (int i = 1; i <= 15; ++i) avl2.insert(i);
    cout << "Inorder: ";
    avl2.inorder();
    cout << "Level order (value+height): ";
    avl2.level_order();

    cout << "\n--- Complexity ---\n";
    cout << "Insert/Delete/Search: O(log n) guaranteed\n";
    cout << "Rotations: O(1)\n";
    cout << "Space: O(n)\n";

    return 0;
}
