/*
==================================================================
157. std::stable_sort
==================================================================
Theory:
- Stable sorting: equivalent elements preserve original order
- Based on MergeSort (or TimSort variant)
- Requires O(n) additional memory (or falls back to O(n log n))
- Maintains relative order of equal elements
- Slightly slower than std::sort due to stability requirement
- Useful when sorting by multiple keys sequentially
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <array>

struct Employee {
    std::string name;
    int department;
    int id;

    void display() const {
        std::cout << "   " << name << " (dept:" << department
                  << ", id:" << id << ")\n";
    }
};

int main() {
    std::cout << "=== std::stable_sort\n\n";

    // --- Stability demo ---
    std::cout << "1. Stability: sort by department, then by id (stable)\n";

    std::vector<Employee> employees = {
        {"Alice",   2, 101},
        {"Bob",     1, 102},
        {"Charlie", 2, 100},
        {"David",   1, 103},
        {"Eve",     2, 99}
    };

    std::cout << "   Original order:\n";
    for (const auto& e : employees) e.display();

    // First sort by id (secondary key)
    std::stable_sort(employees.begin(), employees.end(),
        [](const Employee& a, const Employee& b) {
            return a.id < b.id;
        });
    std::cout << "\n   After sort by id:\n";
    for (const auto& e : employees) e.display();

    // Then sort by department (primary key) — stable preserves id order
    std::stable_sort(employees.begin(), employees.end(),
        [](const Employee& a, const Employee& b) {
            return a.department < b.department;
        });
    std::cout << "\n   After stable sort by department (id order preserved):\n";
    for (const auto& e : employees) e.display();

    // --- Compare with std::sort (not stable) ---
    std::cout << "\n2. std::sort (NOT stable, same operation):\n";
    std::vector<Employee> employees2 = {
        {"Alice",   2, 101},
        {"Bob",     1, 102},
        {"Charlie", 2, 100},
        {"David",   1, 103},
        {"Eve",     2, 99}
    };

    std::sort(employees2.begin(), employees2.end(),
        [](const Employee& a, const Employee& b) {
            return a.id < b.id;
        });
    std::sort(employees2.begin(), employees2.end(),
        [](const Employee& a, const Employee& b) {
            return a.department < b.department;
        });
    std::cout << "   std::sort may reorder within departments:\n";
    for (const auto& e : employees2) e.display();

    // --- Stable sort on primitive arrays ---
    std::cout << "\n3. Stable sort on array:\n";
    std::array<int, 8> arr{3, 1, 4, 1, 5, 9, 2, 6};
    std::stable_sort(arr.begin(), arr.end());
    std::cout << "   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Stability: sort by department, then by id (stable)
   Original order:
   Alice (dept:2, id:101)
   Bob (dept:1, id:102)
   Charlie (dept:2, id:100)
   David (dept:1, id:103)
   Eve (dept:2, id:99)

   After sort by id:
   Eve (dept:2, id:99)
   Charlie (dept:2, id:100)
   Alice (dept:2, id:101)
   Bob (dept:1, id:102)
   David (dept:1, id:103)

   After stable sort by department (id order preserved):
   Bob (dept:1, id:102)
   David (dept:1, id:103)
   Eve (dept:2, id:99)
   Charlie (dept:2, id:100)
   Alice (dept:2, id:101)

2. std::sort (NOT stable, same operation):
   std::sort may reorder within departments:
   Bob (dept:1, id:102)
   David (dept:1, id:103)
   Charlie (dept:2, id:100)  <- Eve and Alice order may vary
   Eve (dept:2, id:99)
   Alice (dept:2, id:101)

3. Stable sort on array:
   1 1 2 3 4 5 6 9
==================================================================
Complexity:
- Best case: O(n log n)
- Worst case: O(n (log n)^2) if memory allocation fails
- Extra memory: O(n) for merge
==================================================================
Interview Questions:
1. When to use stable_sort over sort?
   -> When relative order of equal elements matters,
      e.g., sorting by multiple criteria sequentially.

2. How does stable_sort achieve stability?
   -> Uses merge sort: equal elements from left half always
      come before right half.

3. Why is stable_sort slower than sort?
   -> Extra memory allocation + merge overhead. QuickSort
      is generally faster in practice.
==================================================================
Common Mistakes:
- Using sort instead of stable_sort when order of equals matters
- Expecting O(n) memory allocation always succeeds
- Using stable_sort where O(n log n) time is unacceptable
- Not understanding the memory tradeoff
==================================================================
*/
