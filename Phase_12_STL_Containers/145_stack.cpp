/*
==================================================================
145. std::stack (Container Adapter)
==================================================================
Theory:
- Container adapter: wraps underlying container (default deque)
- LIFO (Last In, First Out) operations
- push, pop, top, empty, size
- No iteration (no begin/end)
- Can use vector or list as underlying container
- Used for: undo/redo, expression evaluation, DFS, call stack
*/

#include <iostream>
#include <stack>
#include <vector>
#include <deque>
#include <string>

int main() {
    std::cout << "=== std::stack (Container Adapter)\n\n";

    // --- Default (deque) ---
    std::stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << "1. Basic LIFO operations:\n";
    std::cout << "   Top: " << s.top() << "\n";
    std::cout << "   Size: " << s.size() << "\n";
    s.pop();
    std::cout << "   Pop -> Top: " << s.top() << "\n";
    s.pop();
    std::cout << "   Pop -> Top: " << s.top() << "\n";
    std::cout << "   Empty: " << s.empty() << "\n";
    s.pop();
    std::cout << "   Empty after all pops: " << s.empty() << "\n\n";

    // --- Stack with std::vector as underlying ---
    std::cout << "2. Stack backed by vector:\n";
    std::stack<int, std::vector<int>> vecStack;
    for (int i = 0; i < 5; ++i)
        vecStack.push(i * 10);
    std::cout << "   ";
    while (!vecStack.empty()) {
        std::cout << vecStack.top() << " ";
        vecStack.pop();
    }
    std::cout << "\n\n";

    // --- Practical: Parentheses matching ---
    std::cout << "3. Practical: Balanced parentheses\n";
    auto isBalanced = [](const std::string& expr) -> bool {
        std::stack<char> st;
        for (char c : expr) {
            if (c == '(' || c == '{' || c == '[')
                st.push(c);
            else if (c == ')' || c == '}' || c == ']') {
                if (st.empty()) return false;
                char top = st.top();
                if ((c == ')' && top != '(') ||
                    (c == '}' && top != '{') ||
                    (c == ']' && top != '['))
                    return false;
                st.pop();
            }
        }
        return st.empty();
    };

    std::string tests[] = {"()", "({[]})", "({[})", "(((", ""};
    for (const auto& t : tests)
        std::cout << "   \"" << t << "\" -> "
                  << (isBalanced(t) ? "balanced" : "unbalanced") << "\n";

    // --- Min stack (two-stack approach) ---
    std::cout << "\n4. Min stack (track minimum):\n";
    std::stack<int> mainStack, minStack;
    auto push = [&](int val) {
        mainStack.push(val);
        if (minStack.empty() || val <= minStack.top())
            minStack.push(val);
    };
    auto pop = [&]() {
        if (mainStack.top() == minStack.top())
            minStack.pop();
        mainStack.pop();
    };
    push(5); push(3); push(7); push(3);
    std::cout << "   After pushes 5,3,7,3\n";
    std::cout << "   Min: " << minStack.top() << "\n";
    pop();
    std::cout << "   After pop, Min: " << minStack.top() << "\n";
    pop();
    std::cout << "   After pop, Min: " << minStack.top() << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic LIFO operations:
   Top: 30
   Size: 3
   Pop -> Top: 20
   Pop -> Top: 10
   Empty: 0
   Empty after all pops: 1

2. Stack backed by vector:
   40 30 20 10 0

3. Practical: Balanced parentheses
   "()" -> balanced
   "({[]})" -> balanced
   "({[})" -> unbalanced
   "(((" -> unbalanced
   "" -> balanced

4. Min stack (track minimum):
   After pushes 5,3,7,3
   Min: 3
   After pop, Min: 3
   After pop, Min: 3
==================================================================
Complexity:
- push, pop, top: O(1)
- Space: O(n)
==================================================================
Interview Questions:
1. Why is stack called an adapter?
   -> It adapts an existing container (deque, vector, list)
      with a restricted LIFO interface.

2. Can we iterate over a stack?
   -> No direct iteration. Must pop elements to access them.
      Or use underlying container if accessible (but not standard).

3. How to implement a min stack?
   -> Use two stacks: one for values, one for minimums.
      Each push: if new <= min.top(), push to min stack.
==================================================================
Common Mistakes:
- Calling top() on empty stack (undefined behavior)
- Forgetting that pop() doesn't return the top element
- Expecting iteration support
- Mixing up stack (LIFO) with queue (FIFO)
==================================================================
*/
