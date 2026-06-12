/*
==================================================================
146. std::queue (Container Adapter)
==================================================================
Theory:
- Container adapter (default deque)
- FIFO (First In, First Out) operations
- push (back), pop (front), front, back, empty, size
- No iteration (no begin/end)
- Used for: BFS, task scheduling, buffering, print spooling
- Alternative underlying: list (not vector — no pop_front)
*/

#include <iostream>
#include <queue>
#include <list>
#include <string>

int main() {
    std::cout << "=== std::queue (Container Adapter)\n\n";

    // --- Default (deque) ---
    std::queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    std::cout << "1. Basic FIFO operations:\n";
    std::cout << "   Front: " << q.front() << ", Back: " << q.back() << "\n";
    std::cout << "   Size: " << q.size() << "\n";
    q.pop();
    std::cout << "   After pop -> Front: " << q.front() << "\n";
    q.pop();
    std::cout << "   After pop -> Front: " << q.front() << "\n";
    std::cout << "   Empty: " << q.empty() << "\n";
    q.pop();
    std::cout << "   Empty after all pops: " << q.empty() << "\n\n";

    // --- Queue with list as underlying ---
    std::cout << "2. Queue backed by list:\n";
    std::queue<int, std::list<int>> listQueue;
    for (int i = 1; i <= 5; ++i)
        listQueue.push(i * 100);
    std::cout << "   ";
    while (!listQueue.empty()) {
        std::cout << listQueue.front() << " ";
        listQueue.pop();
    }
    std::cout << "\n\n";

    // --- Practical: BFS (Binary Tree Level Order) ---
    std::cout << "3. Practical: BFS simulation\n";
    struct Node {
        std::string name;
        std::vector<Node*> children;
    };
    Node root{"A", {}};
    Node b{"B", {}}, c{"C", {}}, d{"D", {}};
    Node e{"E", {}}, f{"F", {}};
    root.children = {&b, &c, &d};
    b.children = {&e, &f};

    std::queue<Node*> bfs;
    bfs.push(&root);
    std::cout << "   BFS order: ";
    while (!bfs.empty()) {
        Node* curr = bfs.front();
        bfs.pop();
        std::cout << curr->name << " ";
        for (Node* child : curr->children)
            bfs.push(child);
    }
    std::cout << "\n\n";

    // --- Circular buffer using queue ---
    std::cout << "4. Fixed-size buffer (queue):\n";
    std::queue<int> buffer;
    const size_t MAX = 3;
    auto enqueue = [&](int val) {
        buffer.push(val);
        if (buffer.size() > MAX) buffer.pop();
    };
    enqueue(1); enqueue(2); enqueue(3);
    std::cout << "   After enqueues 1,2,3: ";
    std::queue<int> show = buffer;
    while (!show.empty()) { std::cout << show.front() << " "; show.pop(); }
    enqueue(4);
    std::cout << "\n   After enqueue 4:     ";
    show = buffer;
    while (!show.empty()) { std::cout << show.front() << " "; show.pop(); }
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic FIFO operations:
   Front: 10, Back: 30
   Size: 3
   After pop -> Front: 20
   After pop -> Front: 30
   Empty: 0
   Empty after all pops: 1

2. Queue backed by list:
   100 200 300 400 500

3. Practical: BFS simulation
   BFS order: A B C D E F

4. Fixed-size buffer (queue):
   After enqueues 1,2,3: 1 2 3
   After enqueue 4:     2 3 4
==================================================================
Complexity:
- push: O(1)
- pop: O(1)
- front/back: O(1)
==================================================================
Interview Questions:
1. Why can't vector be underlying for queue?
   -> vector lacks pop_front(). deque and list have both
      push_front/pop_front.

2. std::queue vs std::deque?
   -> queue is an adapter with restricted FIFO interface.
      deque is a full container with random access.

3. How to clear a queue quickly?
   -> Assign empty: q = std::queue<int>{}; Or swap: { std::queue<int> empty; std::swap(q, empty); }
==================================================================
Common Mistakes:
- Calling front/back on empty queue
- Expecting iteration support
- Using pop() and forgetting it discards (doesn't return element)
- Confusing push (back) with front insert
==================================================================
*/
