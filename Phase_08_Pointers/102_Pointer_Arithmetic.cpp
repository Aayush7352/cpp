/*
THEORY: Pointer Arithmetic

Pointer arithmetic works on typed pointers (not void*). Operations:
- ptr++ / ++ptr: Advance to next element (size of T bytes)
- ptr-- / --ptr: Move to previous element
- ptr + n / ptr - n: Advance/rewind by n elements
- ptr1 - ptr2: Difference in number of elements (ptrdiff_t)
- Relational ops: <, >, <=, >= for pointers in same array

Arithmetic is scaled by sizeof(T). Only valid within array bounds (UB otherwise).
size_t: unsigned result of sizeof, used for indexing.
ptrdiff_t: signed difference between pointers.
*/

#include <iostream>
#include <cstddef>   // ptrdiff_t, size_t
#include <iterator>  // std::size (C++17)

int main() {
    int arr[]{10, 20, 30, 40, 50};
    constexpr size_t N{std::size(arr)};

    // 1. Traversal with pointer arithmetic
    int* ptr{arr};                               // points to arr[0]

    std::cout << "Array traversal with ptr++:\n";
    for (size_t i{0}; i < N; ++i) {
        std::cout << "arr[" << i << "] = " << *ptr << " at " << ptr << '\n';
        ++ptr;                                   // advance to next element
    }

    // 2. ptr + n
    ptr = arr;                                   // reset
    int* fifth{ptr + 4};
    std::cout << "\n*(ptr + 4) = " << *fifth << '\n';

    // 3. Subtraction
    ptrdiff_t diff{fifth - arr};
    std::cout << "fifth - arr = " << diff << " elements\n";

    // 4. ptr--
    int* last{arr + N - 1};
    std::cout << "\nReverse traversal:\n";
    while (last >= arr) {
        std::cout << *last << ' ';
        --last;
    }
    std::cout << '\n';

    // 5. size_t / ptrdiff_t
    std::cout << "\nsizeof(arr) = " << sizeof(arr) << '\n';
    std::cout << "Number of elements (size_t): " << N << '\n';
    std::cout << "ptrdiff_t of last - first: " << (arr + N - 1) - arr << '\n';

    // 6. Comparison
    int* begin{arr};
    int* end{arr + N};
    std::cout << "\nComparison: begin < end is " << (begin < end ? "true" : "false") << '\n';

    // 7. char* arithmetic (sizeof(char) == 1)
    char msg[]{"ABC"};
    char* cp{msg};
    std::cout << "\nChar pointer arithmetic:\n";
    while (*cp) {
        std::cout << *cp << " (addr " << (void*)cp << ")\n";
        ++cp;
    }

    return 0;
}

/*
SAMPLE I/O:
Array traversal with ptr++:
arr[0] = 10 at 0x16f82b510
arr[1] = 20 at 0x16f82b514
arr[2] = 30 at 0x16f82b518
arr[3] = 40 at 0x16f82b51c
arr[4] = 50 at 0x16f82b520

*(ptr + 4) = 50
fifth - arr = 4 elements

Reverse traversal:
50 40 30 20 10

sizeof(arr) = 20
Number of elements (size_t): 5
ptrdiff_t of last - first: 4

Comparison: begin < end is true

Char pointer arithmetic:
A (addr 0x16f82b507)
B (addr 0x16f82b508)
C (addr 0x16f82b509)

COMPLEXITY:
- All operations: O(1)
- Traversal: O(N) time, O(1) space

INTERVIEW QUESTIONS:
1. Can you do pointer arithmetic on void*?
   No — void* has no size information. Must cast to char* or typed pointer first.
   (GCC/Clang extensions allow byte arithmetic on void* as non-standard extension.)

2. What is the difference between arr and &arr?
   arr decays to int* pointing to arr[0]. &arr is int(*)[5] (pointer to array of 5 ints).
   arr + 1 advances by sizeof(int). &arr + 1 advances by sizeof(arr).

3. What is ptrdiff_t?
   Signed integer type defined in <cstddef>. Holds result of pointer subtraction.
   Guaranteed to be large enough for any valid pointer difference.

COMMON MISTAKES:
- Going out of bounds: arr[5] is UB for int arr[5]
- Applying arithmetic on pointers to unrelated objects (UB)
- Using int or unsigned instead of ptrdiff_t for difference
- Forgetting that sizeof returns size_t (unsigned)
- Assuming char* arithmetic adds 1 byte (it does, correct for char)
*/
