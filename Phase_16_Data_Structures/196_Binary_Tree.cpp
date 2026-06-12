/*
 * Phase 16 – Data Structures
 * 196. Binary Tree
 *
 * Theory:
 * - Each node has at most two children: left and right.
 * - Binary tree ≠ BST; no ordering constraint.
 * - Traversals:
 *   - Inorder (LNR): left, root, right
 *   - Preorder (NLR): root, left, right
 *   - Postorder (LRN): left, right, root
 *   - Level-order (BFS): level by level using queue
 * - Properties: height, count of nodes, diameter, balanced check.
 *
 * Common Mistakes:
 * - Confusing the order of traversal types.
 * - Not handling null child pointers (segfault).
 * - Mixing recursive depth with large trees (stack overflow).
 *
 * Interview Questions:
 * 1. Find the height/depth of a tree.
 * 2. Check if a tree is balanced (height difference ≤ 1).
 * 3. Find the diameter of a tree (longest path).
 * 4. Print all root-to-leaf paths.
 * 5. Build tree from inorder + preorder traversals.
 */

#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <cmath>

template<typename T>
struct BTNode {
    T data;
    BTNode* left;
    BTNode* right;

    explicit BTNode(const T& val) : data(val), left(nullptr), right(nullptr) {}
};

template<typename T>
class BinaryTree {
    BTNode<T>* root;

    // --- recursive helpers ---
    void inorder_rec(BTNode<T>* node) const {
        if (!node) return;
        inorder_rec(node->left);
        std::cout << node->data << ' ';
        inorder_rec(node->right);
    }

    void preorder_rec(BTNode<T>* node) const {
        if (!node) return;
        std::cout << node->data << ' ';
        preorder_rec(node->left);
        preorder_rec(node->right);
    }

    void postorder_rec(BTNode<T>* node) const {
        if (!node) return;
        postorder_rec(node->left);
        postorder_rec(node->right);
        std::cout << node->data << ' ';
    }

    int height_rec(BTNode<T>* node) const {
        if (!node) return -1; // height of leaf = 0 if we return 0; using -1 convention
        return 1 + std::max(height_rec(node->left), height_rec(node->right));
    }

    int count_nodes_rec(BTNode<T>* node) const {
        if (!node) return 0;
        return 1 + count_nodes_rec(node->left) + count_nodes_rec(node->right);
    }

    void clear_rec(BTNode<T>* node) {
        if (!node) return;
        clear_rec(node->left);
        clear_rec(node->right);
        delete node;
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { clear_rec(root); }

    BTNode<T>* get_root() const { return root; }

    // Build a sample tree for demonstration
    void build_sample() {
        //        1
        //       / \
        //      2   3
        //     / \   \
        //    4   5   6
        root = new BTNode<T>(1);
        root->left = new BTNode<T>(2);
        root->right = new BTNode<T>(3);
        root->left->left = new BTNode<T>(4);
        root->left->right = new BTNode<T>(5);
        root->right->right = new BTNode<T>(6);
    }

    // --- Recursive traversals ---
    void inorder() const {
        std::cout << "Inorder (recursive):   ";
        inorder_rec(root);
        std::cout << '\n';
    }

    void preorder() const {
        std::cout << "Preorder (recursive):  ";
        preorder_rec(root);
        std::cout << '\n';
    }

    void postorder() const {
        std::cout << "Postorder (recursive): ";
        postorder_rec(root);
        std::cout << '\n';
    }

    // --- Iterative traversals ---
    void inorder_iterative() const {
        std::cout << "Inorder (iterative):   ";
        std::stack<BTNode<T>*> st;
        BTNode<T>* curr = root;
        while (curr || !st.empty()) {
            while (curr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top(); st.pop();
            std::cout << curr->data << ' ';
            curr = curr->right;
        }
        std::cout << '\n';
    }

    void preorder_iterative() const {
        std::cout << "Preorder (iterative):  ";
        if (!root) return;
        std::stack<BTNode<T>*> st;
        st.push(root);
        while (!st.empty()) {
            BTNode<T>* curr = st.top(); st.pop();
            std::cout << curr->data << ' ';
            if (curr->right) st.push(curr->right);
            if (curr->left) st.push(curr->left);
        }
        std::cout << '\n';
    }

    void postorder_iterative() const {
        std::cout << "Postorder (iterative): ";
        if (!root) return;
        std::stack<BTNode<T>*> st1, st2;
        st1.push(root);
        while (!st1.empty()) {
            BTNode<T>* curr = st1.top(); st1.pop();
            st2.push(curr);
            if (curr->left) st1.push(curr->left);
            if (curr->right) st1.push(curr->right);
        }
        while (!st2.empty()) {
            std::cout << st2.top()->data << ' ';
            st2.pop();
        }
        std::cout << '\n';
    }

    // --- Level-order (BFS) ---
    void level_order() const {
        std::cout << "Level-order:           ";
        if (!root) return;
        std::queue<BTNode<T>*> q;
        q.push(root);
        while (!q.empty()) {
            BTNode<T>* curr = q.front(); q.pop();
            std::cout << curr->data << ' ';
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        std::cout << '\n';
    }

    // --- Properties ---
    int height() const {
        int h = height_rec(root);
        std::cout << "Height: " << h << '\n';
        return h;
    }

    int count_nodes() const {
        int cnt = count_nodes_rec(root);
        std::cout << "Node count: " << cnt << '\n';
        return cnt;
    }

    // Check if tree is balanced
    bool is_balanced() const {
        auto check = [](BTNode<T>* node, auto&& self) -> std::pair<bool, int> {
            if (!node) return {true, -1};
            auto [left_bal, left_h] = self(node->left, self);
            auto [right_bal, right_h] = self(node->right, self);
            bool bal = left_bal && right_bal && std::abs(left_h - right_h) <= 1;
            return {bal, 1 + std::max(left_h, right_h)};
        };
        bool balanced = check(root, check).first;
        std::cout << "Is balanced? " << (balanced ? "Yes" : "No") << '\n';
        return balanced;
    }

    // Diameter (longest path between any two nodes)
    int diameter() const {
        auto compute = [](BTNode<T>* node, auto&& self) -> std::pair<int, int> {
            if (!node) return {0, -1};
            auto [left_d, left_h] = self(node->left, self);
            auto [right_d, right_h] = self(node->right, self);
            int height = 1 + std::max(left_h, right_h);
            int diameter = std::max({left_d, right_d, left_h + right_h + 2});
            return {diameter, height};
        };
        int d = compute(root, compute).first;
        std::cout << "Diameter: " << d << '\n';
        return d;
    }
};

int main() {
    using namespace std;

    cout << "=== Binary Tree Demo ===\n\n";

    BinaryTree<int> tree;
    tree.build_sample();

    // Traversals
    tree.inorder();
    tree.preorder();
    tree.postorder();
    cout << '\n';
    tree.inorder_iterative();
    tree.preorder_iterative();
    tree.postorder_iterative();
    tree.level_order();

    cout << '\n';
    tree.height();
    tree.count_nodes();
    tree.is_balanced();
    tree.diameter();

    cout << "\n--- Complexity ---\n";
    cout << "All traversals: O(n)\n";
    cout << "Height: O(n)\n";
    cout << "Count nodes: O(n)\n";
    cout << "Balanced check: O(n)\n";
    cout << "Diameter: O(n)\n";
    cout << "Space (recursion): O(h) stack, worst-case O(n)\n";

    return 0;
}
