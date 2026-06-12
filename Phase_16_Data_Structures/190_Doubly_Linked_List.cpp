/*
 * Phase 16 – Data Structures
 * 190. Doubly Linked List
 *
 * Theory:
 * - Each node has data, a prev pointer, and a next pointer.
 * - Allows forward and backward traversal.
 * - Insert/delete operations are O(1) given a pointer to the node (if head/tail known).
 * - Space: O(n) with extra pointer per node compared to singly linked list.
 *
 * Common Mistakes:
 * - Forgetting to update both prev and next pointers.
 * - Dangling pointers when splicing nodes.
 * - Not updating tail pointer on operations affecting the end.
 *
 * Interview Questions:
 * 1. How does a doubly linked list differ from singly in terms of operations?
 * 2. Implement LRU Cache using doubly linked list + hash map.
 * 3. Reverse a doubly linked list.
 * 4. Why is deletion at tail O(1) in doubly but O(n) in singly?
 */

#include <iostream>
#include <stdexcept>
#include <vector>

template<typename T>
struct DNode {
    T data;
    DNode* prev;
    DNode* next;

    explicit DNode(const T& val) : data(val), prev(nullptr), next(nullptr) {}
};

template<typename T>
class DoublyLinkedList {
    DNode<T>* head;
    DNode<T>* tail;
    size_t count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~DoublyLinkedList() { clear(); }

    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        DNode<T>* curr = other.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = other.tail = nullptr;
        other.count = 0;
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head; tail = other.tail; count = other.count;
            other.head = other.tail = nullptr; other.count = 0;
        }
        return *this;
    }

    void swap(DoublyLinkedList& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(count, other.count);
    }

    void push_front(const T& val) {
        DNode<T>* new_node = new DNode<T>(val);
        if (!head) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        ++count;
    }

    void push_back(const T& val) {
        DNode<T>* new_node = new DNode<T>(val);
        if (!tail) {
            head = tail = new_node;
        } else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        ++count;
    }

    void insert_at(const T& val, size_t position) {
        if (position > count) throw std::out_of_range("Position out of range");
        if (position == 0) { push_front(val); return; }
        if (position == count) { push_back(val); return; }

        DNode<T>* curr = head;
        for (size_t i = 0; i < position; ++i) curr = curr->next;

        DNode<T>* new_node = new DNode<T>(val);
        new_node->prev = curr->prev;
        new_node->next = curr;
        curr->prev->next = new_node;
        curr->prev = new_node;
        ++count;
    }

    void pop_front() {
        if (!head) throw std::underflow_error("List is empty");
        DNode<T>* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        --count;
    }

    void pop_back() {
        if (!tail) throw std::underflow_error("List is empty");
        DNode<T>* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        --count;
    }

    void delete_at(size_t position) {
        if (position >= count) throw std::out_of_range("Position out of range");
        if (position == 0) { pop_front(); return; }
        if (position == count - 1) { pop_back(); return; }

        DNode<T>* curr = head;
        for (size_t i = 0; i < position; ++i) curr = curr->next;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr;
        --count;
    }

    void forward_traverse() const {
        DNode<T>* curr = head;
        while (curr) {
            std::cout << curr->data;
            if (curr->next) std::cout << " <-> ";
            curr = curr->next;
        }
        std::cout << '\n';
    }

    void backward_traverse() const {
        DNode<T>* curr = tail;
        while (curr) {
            std::cout << curr->data;
            if (curr->prev) std::cout << " <-> ";
            curr = curr->prev;
        }
        std::cout << '\n';
    }

    void reverse() {
        if (!head) return;
        DNode<T>* curr = head;
        DNode<T>* temp = nullptr;
        tail = head;
        while (curr) {
            temp = curr->prev;
            curr->prev = curr->next;
            curr->next = temp;
            curr = curr->prev;
        }
        if (temp) head = temp->prev;
    }

    std::optional<size_t> search(const T& val) const {
        DNode<T>* curr = head;
        size_t pos = 0;
        while (curr) {
            if (curr->data == val) return pos;
            curr = curr->next;
            ++pos;
        }
        return std::nullopt;
    }

    [[nodiscard]] size_t size() const { return count; }
    [[nodiscard]] bool empty() const { return count == 0; }
    [[nodiscard]] T front() const { if (!head) throw std::underflow_error("empty"); return head->data; }
    [[nodiscard]] T back() const { if (!tail) throw std::underflow_error("empty"); return tail->data; }

    void clear() {
        DNode<T>* curr = head;
        while (curr) {
            DNode<T>* next = curr->next;
            delete curr;
            curr = next;
        }
        head = tail = nullptr;
        count = 0;
    }

    std::vector<T> to_vector_forward() const {
        std::vector<T> v; v.reserve(count);
        DNode<T>* curr = head;
        while (curr) { v.push_back(curr->data); curr = curr->next; }
        return v;
    }

    std::vector<T> to_vector_backward() const {
        std::vector<T> v; v.reserve(count);
        DNode<T>* curr = tail;
        while (curr) { v.push_back(curr->data); curr = curr->prev; }
        return v;
    }
};

int main() {
    using namespace std;

    DoublyLinkedList<int> dll;

    cout << "=== Doubly Linked List Demo ===\n\n";

    cout << "Pushing back 1,2,3\n";
    dll.push_back(1); dll.push_back(2); dll.push_back(3);
    cout << "Forward:  "; dll.forward_traverse();
    cout << "Backward: "; dll.backward_traverse();

    cout << "\nPushing front 0\n";
    dll.push_front(0);
    cout << "Forward: "; dll.forward_traverse();

    cout << "Inserting 99 at position 2\n";
    dll.insert_at(99, 2);
    cout << "Forward: "; dll.forward_traverse();

    cout << "\nSearching for 99: ";
    auto p = dll.search(99);
    if (p) cout << "found at " << *p << '\n';

    cout << "\nDeleting at position 2\n";
    dll.delete_at(2);
    cout << "Forward: "; dll.forward_traverse();

    cout << "\nPopping front and back\n";
    dll.pop_front(); dll.pop_back();
    cout << "Forward: "; dll.forward_traverse();

    cout << "\nReversing\n";
    dll.push_back(10); dll.push_back(20);
    cout << "Before: "; dll.forward_traverse();
    dll.reverse();
    cout << "After:  "; dll.forward_traverse();
    cout << "Backward: "; dll.backward_traverse();

    cout << "\nSize: " << dll.size() << '\n';

    cout << "\n--- Complexity ---\n";
    cout << "Insert head/tail: O(1)\n";
    cout << "Delete head/tail: O(1)\n";
    cout << "Insert/delete at pos: O(n)\n";
    cout << "Search: O(n)\n";

    return 0;
}
