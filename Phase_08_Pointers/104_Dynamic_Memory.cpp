/*
THEORY: Dynamic Memory

C++ memory management operators:
- new: Allocates a single object, returns typed pointer, calls constructor.
- delete: Destroys object and deallocates memory.
- new[]: Allocates array of objects, calls default constructor for each.
- delete[]: Destroys array elements and deallocates (must match new[]).

Placement new: Construct object at pre-allocated address.
  new (ptr) Type(args);   — no allocation, just construction.

Nothrow new: Returns nullptr on failure instead of throwing std::bad_alloc.
  new (std::nothrow) Type;

Always match: new with delete, new[] with delete[]. Mismatch = UB.
Prefer RAII wrappers (smart pointers, containers) over raw new/delete.
*/

#include <iostream>
#include <new>       // placement new, nothrow
#include <cstdlib>   // malloc, free

struct Point {
    int x, y;
    Point(int xv, int yv) : x{xv}, y{yv} { std::cout << "Point(" << x << "," << y << ")\n"; }
    ~Point() { std::cout << "~Point(" << x << "," << y << ")\n"; }
    Point() : Point(0,0) {}
};

int main() {
    // 1. Single object new/delete
    std::cout << "--- new/delete single ---\n";
    int* p{new int{42}};
    std::cout << "*p = " << *p << '\n';
    delete p;

    // 2. Array new[]/delete[]
    std::cout << "\n--- new[]/delete[] array ---\n";
    int* arr{new int[5]{10, 20, 30, 40, 50}};
    for (int i{0}; i < 5; ++i) {
        std::cout << "arr[" << i << "] = " << arr[i] << '\n';
    }
    delete[] arr;

    // 3. Object new/delete
    std::cout << "\n--- object new/delete ---\n";
    Point* pp{new Point(3, 4)};
    delete pp;

    // 4. Object array new[]/delete[]
    std::cout << "\n--- object array new[]/delete[] ---\n";
    Point* parr{new Point[2]{{1,2}, {3,4}}};
    delete[] parr;

    // 5. Placement new
    std::cout << "\n--- placement new ---\n";
    alignas(Point) char buffer[sizeof(Point)];
    Point* placed{new (buffer) Point(5, 6)};
    placed->~Point();  // must manually call destructor
    // no delete — buffer is stack memory

    // 6. Nothrow new
    std::cout << "\n--- nothrow new ---\n";
    int* big{new (std::nothrow) int[1'000'000]};
    if (!big) {
        std::cout << "Allocation failed (returned nullptr)\n";
    } else {
        std::cout << "Allocation succeeded\n";
        delete[] big;
    }

    // 7. Mixing new and free (DANGER — demo only)
    // int* bad{new int{7}};
    // free(bad);    // UB: new must pair with delete, not free

    return 0;
}

/*
SAMPLE I/O:
--- new/delete single ---
*p = 42

--- new[]/delete[] array ---
arr[0] = 10
arr[1] = 20
arr[2] = 30
arr[3] = 40
arr[4] = 50

--- object new/delete ---
Point(3,4)
~Point(3,4)

--- object array new[]/delete[] ---
Point(1,2)
Point(3,4)
~Point(3,4)
~Point(1,2)

--- placement new ---
Point(5,6)
~Point(5,6)

--- nothrow new ---
Allocation succeeded

COMPLEXITY:
- new/delete: O(1) amortized (heap allocator dependent)
- new[]/delete[]: O(N) for objects (N constructors/destructors called)

INTERVIEW QUESTIONS:
1. Difference between new and malloc()?
   new calls constructor, is type-safe, throws on failure.
   malloc() returns void*, no constructor, returns NULL on failure.
   delete calls destructor; free() does not.

2. Why must new[] be paired with delete[]?
   new[] stores the array size in the allocation header. delete[] knows to
   call destructors for all elements. Using delete on new[]'d memory = UB.

3. What is placement new used for?
   Reusing memory (e.g., custom allocators, embedded systems, object pools).
   No deallocation — only destruction.

COMMON MISTAKES:
- Forgetting delete/delete[] (memory leak)
- Using delete instead of delete[] on array (UB)
- Double delete (UB)
- Using placement new on not-enough-aligned memory (UB)
- Mixing malloc/free with new/delete
- Not calling destructor explicitly for placement new objects
*/
