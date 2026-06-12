/*
==================================================================
169. std::remove_if with Erase-Remove Idiom
==================================================================
Theory:
- std::remove_if does NOT erase elements — it moves "kept"
  elements to the front and returns new logical end
- The erase-remove idiom: container.erase(
    std::remove_if(...), container.end())
- remove_if: stable (preserves order of kept elements)
- Also: std::remove (value), std::unique (consecutive duplicates)
- Works with vector, deque, list (list has remove/remove_if member)
- After remove_if, elements from new end to old end are
  in unspecified state
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <list>

int main() {
    std::cout << "=== remove_if with Erase-Remove Idiom\n\n";

    // --- Basic remove_if + erase ---
    std::cout << "1. Remove all even numbers:\n";
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto newEnd = std::remove_if(v.begin(), v.end(),
                                  [](int x) { return x % 2 == 0; });

    std::cout << "   After remove_if (before erase):\n";
    std::cout << "   size=" << v.size() << " | ";
    for (int x : v) std::cout << x << " ";
    std::cout << " (last " << std::distance(newEnd, v.end())
              << " are garbage)\n";

    v.erase(newEnd, v.end());
    std::cout << "   After erase: ";
    for (int x : v) std::cout << x << " ";
    std::cout << " (size=" << v.size() << ")\n\n";

    // --- remove (by value) ---
    std::cout << "2. std::remove (by value):\n";
    std::vector<int> v2{1, 2, 3, 2, 4, 2, 5};
    v2.erase(std::remove(v2.begin(), v2.end(), 2), v2.end());
    std::cout << "   Remove all 2s: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Lambda with predicate ---
    std::cout << "3. Remove strings containing 'a':\n";
    std::vector<std::string> words{"apple", "banana", "cherry", "date", "fig"};
    words.erase(
        std::remove_if(words.begin(), words.end(),
                        [](const std::string& s) {
                            return s.find('a') != std::string::npos;
                        }),
        words.end());
    std::cout << "   Without 'a': ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n\n";

    // --- Erase-remove with custom objects ---
    std::cout << "4. Remove elements matching condition:\n";
    struct Product {
        std::string name;
        double price;
    };
    std::vector<Product> products{{"Laptop", 999.99}, {"Mouse", 25.50},
                                   {"Keyboard", 75.00}, {"Monitor", 299.99}};
    products.erase(
        std::remove_if(products.begin(), products.end(),
                        [](const Product& p) { return p.price > 100; }),
        products.end());
    std::cout << "   Products under $100:\n";
    for (const auto& p : products)
        std::cout << "   " << p.name << " ($" << p.price << ")\n";
    std::cout << "\n";

    // --- unique ---
    std::cout << "5. std::unique (remove consecutive duplicates):\n";
    std::vector<int> dup{1, 1, 2, 2, 2, 3, 1, 1, 4};
    dup.erase(std::unique(dup.begin(), dup.end()), dup.end());
    std::cout << "   After unique: ";
    for (int x : dup) std::cout << x << " ";
    std::cout << "\n\n";

    // --- List uses member functions ---
    std::cout << "6. List has member remove_if (faster):\n";
    std::list<int> lst{1, 2, 3, 4, 5, 6};
    lst.remove_if([](int x) { return x % 3 == 0; });
    std::cout << "   List after remove_if (multiples of 3): ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Remove all even numbers:
   After remove_if (before erase):
   size=10 | 1 3 5 7 9 6 7 8 9 10  (last 5 are garbage)
   After erase: 1 3 5 7 9  (size=5)

2. std::remove (by value):
   Remove all 2s: 1 3 4 5

3. Remove strings containing 'a':
   Without 'a': cherry fig

4. Remove elements matching condition:
   Products under $100:
   Mouse ($25.5)
   Keyboard ($75)

5. std::unique (remove consecutive duplicates):
   After unique: 1 2 3 1 4

6. List has member remove_if (faster):
   List after remove_if (multiples of 3): 1 2 4 5
==================================================================
Complexity:
- remove_if: O(n)
- erase (vector): O(n) for elements after removal point
- unique: O(n)
- remove_if (list member): O(n)
==================================================================
Interview Questions:
1. Why doesn't remove_if actually remove?
   -> It's designed to work with all container types. Only the
      container knows how to deallocate. Algorithm moves elements,
      container erases.

2. What happens to elements after new end?
   -> They're in valid but unspecified state (moved-from).

3. Erase-remove vs list::remove_if?
   -> For list, member remove_if is O(n) and actually removes.
      For vector, algorithm + erase is needed.
==================================================================
Common Mistakes:
- Forgetting to call erase after remove_if (logical size unchanged)
- Using remove_if on associative containers (use member erase)
- Using unique before sorting (only removes consecutive duplicates)
- Capturing by reference in lambda and modifying it
- Not understanding remove_if returns iterator to new end
==================================================================
*/
