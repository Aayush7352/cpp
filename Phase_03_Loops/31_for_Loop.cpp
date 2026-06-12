/*
 * ====================================================================
 * Program 31: for Loop
 * Theory:
 *   The for loop has three parts: initialization, condition, update.
 *   Syntax: for(init; condition; update) { body }
 *
 *   Variations:
 *   - Multiple counters: for(int i=0, j=10; i<j; ++i, --j)
 *   - Comma operator: allows multiple expressions in init/update
 *   - Empty sections: for(;;) is an infinite loop
 *   - Range-based for (C++11): for(auto& elem : container)
 *
 * Sample Input/Output:
 *   Forward: 1 2 3 4 5
 *   Backward: 5 4 3 2 1
 *   Multiple counters: i=0 j=10  i=1 j=9  i=2 j=8  i=3 j=7  i=4 j=6
 *
 * Time Complexity:  O(n)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Can you have multiple variables in for loop?
 *   A: Yes, using the comma operator: for(int i=0, j=0; ...)
 *
 *   Q: What is the difference between for and while?
 *   A: for is preferred when the number of iterations is known;
 *      while is better when waiting for a condition.
 *
 * Common Mistakes:
 *   - Off-by-one errors (using <= instead of <)
 *   - Modifying loop counter inside the body unexpectedly
 *   - Using comma as separator vs operator (comma operator vs function args)
 * ====================================================================
 */

#include <iostream>
#include <vector>

int main()
{
    // --- Basic forward loop ---
    std::cout << "Forward: ";
    for (int i{1}; i <= 5; ++i)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // --- Backward loop ---
    std::cout << "Backward: ";
    for (int i{5}; i >= 1; --i)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // --- Multiple counters with comma operator ---
    std::cout << "Multiple counters: ";
    for (int i{0}, j{10}; i < j; ++i, --j)
    {
        std::cout << "i=" << i << " j=" << j << "  ";
    }
    std::cout << '\n';

    // --- Range-based for (C++20) ---
    std::vector<int> vec{10, 20, 30, 40, 50};
    std::cout << "Range-based: ";
    for (int v : vec)
    {
        std::cout << v << ' ';
    }
    std::cout << '\n';

    // --- Infinite loop with break ---
    int count{};
    std::cout << "Break on 3: ";
    for (;;)
    {
        if (count >= 3) break;
        std::cout << count << ' ';
        ++count;
    }
    std::cout << '\n';

    return 0;
}
