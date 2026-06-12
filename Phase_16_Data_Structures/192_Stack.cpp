/*
 * Phase 16 – Data Structures
 * 192. Stack
 *
 * Theory:
 * - LIFO (Last-In, First-Out) data structure.
 * - Push (insert top), Pop (remove top), Top (peek), isEmpty.
 * - Implementations:
 *   1. Array (fixed size, fast, simple)
 *   2. Vector (dynamic, STL-like)
 *   3. Linked list (dynamic, no wasted space, O(1) push/pop)
 * - Applications: function call stack, expression evaluation, undo/redo,
 *   DFS, bracket matching.
 *
 * Common Mistakes:
 * - Stack overflow (pushing beyond capacity in array-based).
 * - Stack underflow (popping from empty stack).
 * - Not handling LIFO semantics correctly.
 *
 * Interview Questions:
 * 1. Implement a stack that supports getMin() in O(1).
 * 2. Implement two stacks in one array.
 * 3. Implement a stack using two queues.
 * 4. Check for balanced parentheses using stack.
 * 5. Evaluate postfix expression using stack.
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>

// ---------- Array-based stack (fixed capacity) ----------
template<typename T, size_t Capacity = 100>
class ArrayStack {
    T data[Capacity];
    int top_idx; // -1 when empty

public:
    ArrayStack() : top_idx(-1) {}

    void push(const T& val) {
        if (top_idx >= static_cast<int>(Capacity) - 1)
            throw std::overflow_error("Stack overflow");
        data[++top_idx] = val;
    }

    void pop() {
        if (empty()) throw std::underflow_error("Stack underflow");
        --top_idx;
    }

    T& top() {
        if (empty()) throw std::underflow_error("Stack empty");
        return data[top_idx];
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("Stack empty");
        return data[top_idx];
    }

    bool empty() const { return top_idx == -1; }
    size_t size() const { return static_cast<size_t>(top_idx + 1); }
    size_t capacity() const { return Capacity; }
};

// ---------- Vector-based stack (dynamic) ----------
template<typename T>
class VectorStack {
    std::vector<T> data;

public:
    void push(const T& val) { data.push_back(val); }

    void pop() {
        if (data.empty()) throw std::underflow_error("Stack underflow");
        data.pop_back();
    }

    T& top() {
        if (data.empty()) throw std::underflow_error("Stack empty");
        return data.back();
    }

    const T& top() const {
        if (data.empty()) throw std::underflow_error("Stack empty");
        return data.back();
    }

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
};

// ---------- Linked-list-based stack ----------
template<typename T>
struct SNode {
    T data;
    SNode* next;
    explicit SNode(const T& val) : data(val), next(nullptr) {}
};

template<typename T>
class LinkedStack {
    SNode<T>* top_node;
    size_t count;

public:
    LinkedStack() : top_node(nullptr), count(0) {}
    ~LinkedStack() { while (top_node) { SNode<T>* temp = top_node; top_node = top_node->next; delete temp; } }

    void push(const T& val) {
        SNode<T>* new_node = new SNode<T>(val);
        new_node->next = top_node;
        top_node = new_node;
        ++count;
    }

    void pop() {
        if (!top_node) throw std::underflow_error("Stack underflow");
        SNode<T>* temp = top_node;
        top_node = top_node->next;
        delete temp;
        --count;
    }

    T& top() {
        if (!top_node) throw std::underflow_error("Stack empty");
        return top_node->data;
    }

    const T& top() const {
        if (!top_node) throw std::underflow_error("Stack empty");
        return top_node->data;
    }

    bool empty() const { return top_node == nullptr; }
    size_t size() const { return count; }
};

// ---------- Helper: balanced parentheses ----------
bool are_parentheses_balanced(const std::string& expr) {
    VectorStack<char> st;
    for (char ch : expr) {
        if (ch == '(' || ch == '{' || ch == '[') {
            st.push(ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (st.empty()) return false;
            char top = st.top(); st.pop();
            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '['))
                return false;
        }
    }
    return st.empty();
}

// ---------- Helper: postfix evaluation ----------
int evaluate_postfix(const std::string& expr) {
    VectorStack<int> st;
    for (char ch : expr) {
        if (std::isdigit(ch)) {
            st.push(ch - '0');
        } else {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            switch (ch) {
                case '+': st.push(a + b); break;
                case '-': st.push(a - b); break;
                case '*': st.push(a * b); break;
                case '/': st.push(a / b); break;
                default: throw std::invalid_argument("Unknown operator");
            }
        }
    }
    return st.top();
}

int main() {
    using namespace std;

    cout << "=== Stack Demo (3 implementations) ===\n\n";

    // ---- Array Stack ----
    cout << "--- ArrayStack ---\n";
    ArrayStack<int, 10> arr_st;
    arr_st.push(10); arr_st.push(20); arr_st.push(30);
    cout << "Top: " << arr_st.top() << ", Size: " << arr_st.size() << '\n';
    arr_st.pop();
    cout << "After pop, top: " << arr_st.top() << '\n';
    arr_st.pop(); arr_st.pop();
    cout << "Empty? " << boolalpha << arr_st.empty() << '\n';

    // ---- Vector Stack ----
    cout << "\n--- VectorStack ---\n";
    VectorStack<int> vec_st;
    for (int i = 1; i <= 5; ++i) vec_st.push(i * 10);
    while (!vec_st.empty()) {
        cout << vec_st.top() << ' ';
        vec_st.pop();
    }
    cout << "(LIFO order)\n";

    // ---- Linked Stack ----
    cout << "\n--- LinkedStack ---\n";
    LinkedStack<int> lk_st;
    lk_st.push(100); lk_st.push(200); lk_st.push(300);
    cout << "Top: " << lk_st.top() << ", Size: " << lk_st.size() << '\n';

    // ---- Applications ----
    cout << "\n--- Applications ---\n";

    string expr1 = "({[]})";
    string expr2 = "({[})";
    cout << "Balanced '({[]})': " << are_parentheses_balanced(expr1) << '\n';
    cout << "Balanced '({[})': " << are_parentheses_balanced(expr2) << '\n';

    string postfix = "23*54*+";
    cout << "Postfix '23*54*+' = " << evaluate_postfix(postfix) << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Push: O(1) (amortized for vector)\n";
    cout << "Pop: O(1)\n";
    cout << "Top: O(1)\n";
    cout << "Space: O(n)\n";

    return 0;
}
