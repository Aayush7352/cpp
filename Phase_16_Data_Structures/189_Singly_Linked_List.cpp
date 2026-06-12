/*
 * Phase 16 – Data Structures
 * 189. Singly Linked List
 *
 * Theory:
 * - A singly linked list is a linear data structure where each element (node)
 *   contains data and a pointer to the next node.
 * - Head points to the first node; tail->next = nullptr.
 * - Operations: insert/delete at head, tail, given position; search; reverse; traversal.
 * - Time complexities:
 *     Insert at head: O(1)
 *     Insert at tail: O(n) without tail pointer, O(1) with tail pointer
 *     Delete at head: O(1)
 *     Delete at tail: O(n)
 *     Search: O(n)
 *     Reverse: O(n)
 *     Traversal: O(n)
 * - Space complexity: O(n) for n nodes.
 *
 * Common Mistakes:
 * - Losing reference to head/tail pointer.
 * - Not updating tail pointer when inserting/deleting at tail.
 * - Off-by-one errors in position-based operations.
 * - Memory leaks (forgetting to delete nodes in non-RAII contexts).
 *
 * Interview Questions:
 * 1. How do you detect a cycle in a linked list? (Floyd's cycle detection)
 * 2. How do you find the middle element in one pass? (slow/fast pointer)
 * 3. How do you merge two sorted linked lists?
 * 4. How do you remove the nth node from the end?
 * 5. How do you check if a linked list is a palindrome?
 */

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

template<typename T>
struct Node {
    T data;
    Node* next;

    explicit Node(const T& val) : data(val), next(nullptr) {}
};

template<typename T>
class SinglyLinkedList {
    Node<T>* head;
    Node<T>* tail;
    size_t count;

public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    // Copy constructor
    SinglyLinkedList(const SinglyLinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node<T>* curr = other.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    // Move constructor
    SinglyLinkedList(SinglyLinkedList&& other) noexcept
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this != &other) {
            SinglyLinkedList temp(other);
            swap(temp);
        }
        return *this;
    }

    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            count = other.count;
            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;
        }
        return *this;
    }

    void swap(SinglyLinkedList& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(count, other.count);
    }

    // --- Insert operations ---

    void push_front(const T& val) {
        Node<T>* new_node = new Node<T>(val);
        new_node->next = head;
        head = new_node;
        if (!tail) tail = head;
        ++count;
    }

    void push_back(const T& val) {
        Node<T>* new_node = new Node<T>(val);
        if (!tail) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++count;
    }

    void insert_at(const T& val, size_t position) {
        if (position > count) {
            throw std::out_of_range("Position out of range");
        }
        if (position == 0) {
            push_front(val);
            return;
        }
        if (position == count) {
            push_back(val);
            return;
        }

        Node<T>* curr = head;
        for (size_t i = 0; i < position - 1; ++i) {
            curr = curr->next;
        }
        Node<T>* new_node = new Node<T>(val);
        new_node->next = curr->next;
        curr->next = new_node;
        ++count;
    }

    // --- Delete operations ---

    void pop_front() {
        if (!head) throw std::underflow_error("List is empty");
        Node<T>* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        --count;
    }

    void pop_back() {
        if (!head) throw std::underflow_error("List is empty");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node<T>* curr = head;
            while (curr->next != tail) {
                curr = curr->next;
            }
            delete tail;
            tail = curr;
            tail->next = nullptr;
        }
        --count;
    }

    void delete_at(size_t position) {
        if (position >= count) {
            throw std::out_of_range("Position out of range");
        }
        if (position == 0) {
            pop_front();
            return;
        }
        Node<T>* curr = head;
        for (size_t i = 0; i < position - 1; ++i) {
            curr = curr->next;
        }
        Node<T>* temp = curr->next;
        curr->next = temp->next;
        if (!curr->next) tail = curr;
        delete temp;
        --count;
    }

    // --- Search ---

    std::optional<size_t> search(const T& val) const {
        Node<T>* curr = head;
        size_t pos = 0;
        while (curr) {
            if (curr->data == val) return pos;
            curr = curr->next;
            ++pos;
        }
        return std::nullopt;
    }

    // --- Reverse ---

    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* curr = head;
        tail = head;
        while (curr) {
            Node<T>* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    // --- Traversal ---

    void traverse() const {
        Node<T>* curr = head;
        while (curr) {
            std::cout << curr->data;
            if (curr->next) std::cout << " -> ";
            curr = curr->next;
        }
        std::cout << '\n';
    }

    [[nodiscard]] std::vector<T> to_vector() const {
        std::vector<T> result;
        result.reserve(count);
        Node<T>* curr = head;
        while (curr) {
            result.push_back(curr->data);
            curr = curr->next;
        }
        return result;
    }

    [[nodiscard]] size_t size() const { return count; }
    [[nodiscard]] bool empty() const { return count == 0; }
    [[nodiscard]] T front() const {
        if (!head) throw std::underflow_error("List is empty");
        return head->data;
    }
    [[nodiscard]] T back() const {
        if (!tail) throw std::underflow_error("List is empty");
        return tail->data;
    }

    void clear() {
        Node<T>* curr = head;
        while (curr) {
            Node<T>* next = curr->next;
            delete curr;
            curr = next;
        }
        head = tail = nullptr;
        count = 0;
    }

    // Iterator support (minimal forward iterator)
    class Iterator {
        Node<T>* ptr;
    public:
        explicit Iterator(Node<T>* p) : ptr(p) {}
        T& operator*() { return ptr->data; }
        T* operator->() { return &ptr->data; }
        Iterator& operator++() { ptr = ptr->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ptr = ptr->next; return tmp; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

// --- Sample I/O ---
int main() {
    using namespace std;

    SinglyLinkedList<int> list;

    cout << "=== Singly Linked List Demo ===\n\n";

    // Insert operations
    cout << "Pushing back 10, 20, 30\n";
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    cout << "List: "; list.traverse();

    cout << "Pushing front 5\n";
    list.push_front(5);
    cout << "List: "; list.traverse();

    cout << "Inserting 15 at position 2\n";
    list.insert_at(15, 2);
    cout << "List: "; list.traverse();

    cout << "Front: " << list.front() << ", Back: " << list.back() << ", Size: " << list.size() << '\n';

    // Search
    cout << "\nSearching for 15: ";
    auto pos = list.search(15);
    if (pos) cout << "found at position " << *pos << '\n';
    else cout << "not found\n";

    cout << "Searching for 99: ";
    pos = list.search(99);
    if (pos) cout << "found at position " << *pos << '\n';
    else cout << "not found\n";

    // Delete operations
    cout << "\nPopping front\n";
    list.pop_front();
    cout << "List: "; list.traverse();

    cout << "Popping back\n";
    list.pop_back();
    cout << "List: "; list.traverse();

    cout << "Deleting at position 1\n";
    list.delete_at(1);
    cout << "List: "; list.traverse();

    // Reverse
    cout << "\nReversing list\n";
    list.push_back(40);
    list.push_back(50);
    cout << "Before: "; list.traverse();
    list.reverse();
    cout << "After:  "; list.traverse();

    // Iterator traversal
    cout << "\nIterator traversal: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        cout << *it << ' ';
    }
    cout << '\n';

    // Clear
    cout << "\nClearing list\n";
    list.clear();
    cout << "Size after clear: " << list.size() << '\n';

    // Complexity summary
    cout << "\n--- Complexity Summary ---\n";
    cout << "Insert head: O(1)\n";
    cout << "Insert tail: O(1) (with tail ptr) / O(n) (without)\n";
    cout << "Insert at pos: O(n)\n";
    cout << "Delete head: O(1)\n";
    cout << "Delete tail: O(n)\n";
    cout << "Search: O(n)\n";
    cout << "Reverse: O(n)\n";
    cout << "Traversal: O(n)\n";

    return 0;
}
