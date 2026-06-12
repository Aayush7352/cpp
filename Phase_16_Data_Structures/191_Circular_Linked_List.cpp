/*
 * Phase 16 – Data Structures
 * 191. Circular Linked List
 *
 * Theory:
 * - A circular singly linked list where the last node's next points to head.
 * - No null pointers in a well-formed circular list.
 * - Traversal must detect when we've returned to head to avoid infinite loops.
 * - Josephus problem: n people stand in a circle, every k-th is eliminated.
 *   Circular linked list is a natural fit: simulate elimination in O(n*k).
 *
 * Common Mistakes:
 * - Infinite loops during traversal (no null sentinel).
 * - Not handling single-node circular list correctly.
 * - Losing the head pointer when inserting/deleting.
 *
 * Interview Questions:
 * 1. Solve Josephus problem with O(n) using recurrence: J(n,k) = (J(n-1,k)+k)%n.
 * 2. Detect if a linked list is circular.
 * 3. Split a circular list into two halves.
 * 4. Insert into a sorted circular linked list.
 */

#include <iostream>
#include <stdexcept>

template<typename T>
struct CNode {
    T data;
    CNode* next;
    explicit CNode(const T& val) : data(val), next(nullptr) {}
};

template<typename T>
class CircularLinkedList {
    CNode<T>* head;
    CNode<T>* tail;
    size_t count;

public:
    CircularLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~CircularLinkedList() { clear(); }

    bool empty() const { return count == 0; }
    size_t size() const { return count; }

    void push_back(const T& val) {
        CNode<T>* new_node = new CNode<T>(val);
        if (!head) {
            head = tail = new_node;
            tail->next = head;
        } else {
            tail->next = new_node;
            tail = new_node;
            tail->next = head;
        }
        ++count;
    }

    void push_front(const T& val) {
        CNode<T>* new_node = new CNode<T>(val);
        if (!head) {
            head = tail = new_node;
            tail->next = head;
        } else {
            new_node->next = head;
            head = new_node;
            tail->next = head;
        }
        ++count;
    }

    void pop_front() {
        if (!head) throw std::underflow_error("List is empty");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            CNode<T>* temp = head;
            head = head->next;
            tail->next = head;
            delete temp;
        }
        --count;
    }

    void delete_at(size_t position) {
        if (position >= count) throw std::out_of_range("Position out of range");
        if (position == 0) { pop_front(); return; }

        CNode<T>* curr = head;
        for (size_t i = 0; i < position - 1; ++i) curr = curr->next;
        CNode<T>* temp = curr->next;
        curr->next = temp->next;
        if (temp == tail) tail = curr;
        delete temp;
        --count;
    }

    void traverse(int max_nodes = 20) const {
        if (!head) { std::cout << "(empty)\n"; return; }
        CNode<T>* curr = head;
        int visited = 0;
        do {
            std::cout << curr->data;
            curr = curr->next;
            if (curr != head && visited < max_nodes - 1) std::cout << " -> ";
            ++visited;
        } while (curr != head && visited < max_nodes);
        if (visited == max_nodes) std::cout << " ...";
        std::cout << " (back to head)\n";
    }

    // Josephus problem: eliminate every k-th person, return survivor
    // 0-indexed position
    T josephus(int k) {
        if (empty()) throw std::underflow_error("Empty list");
        if (k <= 0) throw std::invalid_argument("k must be positive");

        // Build circular list 0..n-1
        CircularLinkedList<int> circle;
        for (size_t i = 0; i < count; ++i) {
            // We'll use the data values stored
        }
        // Actually, simplify: recreate with values from our list
        // Or just use a simpler approach: create fresh list 0..count-1
        // Let's just simulate with the current list
        if (count == 1) return head->data;

        // Convert to vector for Josephus simulation
        // We'll implement in-place using circular list traversal
        CNode<T>* curr = head;
        CNode<T>* prev = tail;

        while (count > 1) {
            // Move (k-1) steps forward
            for (int step = 1; step < k; ++step) {
                prev = curr;
                curr = curr->next;
            }
            // Eliminate curr
            CNode<T>* elim = curr;
            if (elim == head) head = head->next;
            if (elim == tail) tail = prev;
            prev->next = curr->next;
            curr = curr->next;
            delete elim;
            --count;
        }
        head = tail = curr;
        tail->next = head;
        return curr->data;
    }

    void clear() {
        if (!head) return;
        CNode<T>* curr = head;
        do {
            CNode<T>* next = curr->next;
            delete curr;
            curr = next;
        } while (curr != head);
        head = tail = nullptr;
        count = 0;
    }

    std::vector<T> to_vector() const {
        std::vector<T> v;
        if (!head) return v;
        v.reserve(count);
        CNode<T>* curr = head;
        do {
            v.push_back(curr->data);
            curr = curr->next;
        } while (curr != head);
        return v;
    }
};

// Recurrence-based Josephus solution: O(n)
int josephus_recurrence(int n, int k) {
    if (n == 1) return 0;
    return (josephus_recurrence(n - 1, k) + k) % n;
}

int main() {
    using namespace std;

    cout << "=== Circular Linked List Demo ===\n\n";

    CircularLinkedList<int> cll;
    for (int i = 1; i <= 7; ++i) cll.push_back(i);
    cout << "Circular list (1..7):\n";
    cll.traverse();
    cout << "Size: " << cll.size() << '\n';

    cout << "\nJosephus problem (k=3), survivor: " << cll.josephus(3) << '\n';

    // Re-demonstrate Josephus recurrence
    cout << "\nJosephus recurrence (n=7, k=3): survivor = "
         << josephus_recurrence(7, 3) + 1 << " (1-indexed)\n";

    // Rebuild for more demos
    CircularLinkedList<int> list2;
    for (int i = 10; i <= 50; i += 10) list2.push_back(i);
    cout << "\nList (10,20,30,40,50):\n";
    list2.traverse();

    cout << "Pushing front 5\n";
    list2.push_front(5);
    list2.traverse();

    cout << "Popping front\n";
    list2.pop_front();
    list2.traverse();

    cout << "Deleting at position 2\n";
    list2.delete_at(2);
    list2.traverse();

    cout << "\n--- Complexity ---\n";
    cout << "Insert/delete at head: O(1)\n";
    cout << "Insert/delete at tail: O(1) (with tail ptr)\n";
    cout << "Josephus simulation: O(n*k)\n";
    cout << "Josephus recurrence: O(n)\n";

    return 0;
}
