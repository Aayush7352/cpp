/*
==================================================================
147. std::priority_queue
==================================================================:
Theory:
- Container adapter (default vector)
- Max-heap: largest element at top by default
- push, pop (remove top), top (read top), empty, size
- Custom comparator: use std::greater<T> for min-heap
- Based on std::make_heap, push_heap, pop_heap
- No iteration support
*/

#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <string>

struct Task {
    int priority;
    std::string description;

    bool operator<(const Task& other) const {
        // Higher priority = higher in max-heap
        return priority < other.priority;
    }
};

int main() {
    std::cout << "=== std::priority_queue\n\n";

    // --- Default max-heap ---
    std::cout << "1. Default max-heap:\n";
    std::priority_queue<int> pq;
    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    std::cout << "   ";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << " (largest first)\n\n";

    // --- Min-heap ---
    std::cout << "2. Min-heap (using greater):\n";
    std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;
    minPQ.push(30);
    minPQ.push(10);
    minPQ.push(50);
    minPQ.push(20);
    std::cout << "   ";
    while (!minPQ.empty()) {
        std::cout << minPQ.top() << " ";
        minPQ.pop();
    }
    std::cout << " (smallest first)\n\n";

    // --- Custom comparator (lambda) ---
    std::cout << "3. Custom comparator (lambda):\n";
    auto cmp = [](int a, int b) { return a > b; }; // min-heap
    std::priority_queue<int, std::vector<int>, decltype(cmp)> lambdaPQ(cmp);
    lambdaPQ.push(7); lambdaPQ.push(3); lambdaPQ.push(9);
    std::cout << "   ";
    while (!lambdaPQ.empty()) {
        std::cout << lambdaPQ.top() << " ";
        lambdaPQ.pop();
    }
    std::cout << "\n\n";

    // --- Custom class with priority ---
    std::cout << "4. Custom Task priority queue:\n";
    std::priority_queue<Task> taskQueue;
    taskQueue.push({3, "Low: Clean desk"});
    taskQueue.push({5, "Medium: Write report"});
    taskQueue.push({10, "Critical: Fix bug"});
    taskQueue.push({7, "High: Review PR"});

    while (!taskQueue.empty()) {
        const auto& t = taskQueue.top();
        std::cout << "   [P" << t.priority << "] " << t.description << "\n";
        taskQueue.pop();
    }

    // --- Kth largest element ---
    std::cout << "\n5. Kth largest (using min-heap of size K):\n";
    auto kthLargest = [](std::vector<int> nums, int k) -> int {
        std::priority_queue<int, std::vector<int>, std::greater<int>> minH;
        for (int n : nums) {
            minH.push(n);
            if (static_cast<int>(minH.size()) > k)
                minH.pop();
        }
        return minH.top();
    };
    std::vector<int> data{3, 2, 1, 5, 6, 4};
    std::cout << "   Data: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n   3rd largest: " << kthLargest(data, 3) << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Default max-heap:
   50 30 20 10  (largest first)

2. Min-heap (using greater):
   10 20 30 50  (smallest first)

3. Custom comparator (lambda):
   3 7 9

4. Custom Task priority queue:
   [10] Critical: Fix bug
   [7] High: Review PR
   [5] Medium: Write report
   [3] Low: Clean desk

5. Kth largest (using min-heap of size K):
   Data: 3 2 1 5 6 4
   3rd largest: 4
==================================================================
Complexity:
- push: O(log n)
- pop: O(log n)
- top: O(1)
==================================================================
Interview Questions:
1. What underlying container does priority_queue use?
   -> Default is vector. Any random-access container with
      front, push_back, pop_back works (e.g., deque).

2. How to get both min and max?
   -> Two heaps (min + max), or use std::multiset.

3. How to iterate priority_queue?
   -> Not directly. Copy to vector and sort, or pop all.
==================================================================
Common Mistakes:
- Using default comparator when min-heap needed
- Forgetting to pass custom comparator as constructor arg
  (for lambda comparators, must pass instance)
- Calling top() on empty priority_queue
- Expecting stable ordering (heap is not sorted array)
==================================================================
*/
