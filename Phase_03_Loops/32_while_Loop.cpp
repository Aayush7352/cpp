/*
 * ====================================================================
 * Program 32: while Loop
 * Theory:
 *   The while loop repeats as long as its condition is true.
 *   The condition is tested BEFORE each iteration — it may execute
 *   zero times if the condition starts false.
 *
 *   Common patterns:
 *   - Sentinel pattern: loop until a special value (e.g., -1) is entered
 *   - EOF pattern: read input until end-of-file (std::cin >> x)
 *   - Flag-controlled: while(!done) with a bool variable
 *
 * Sample Input/Output:
 *   Enter numbers to sum (0 to stop):
 *   10 20 30 0
 *   Sum: 60
 *
 *   --- Sentinel pattern ---
 *   Enter positive numbers (-1 to quit):
 *   5 8 12 -1
 *   Sum: 25
 *
 * Time Complexity:  O(n) where n is number of inputs
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: When to use while vs for?
 *   A: while when iterations depend on a condition that may change
 *      inside the loop; for when iteration count is known.
 *
 *   Q: Can while cause infinite loops?
 *   A: Yes — if the condition never becomes false, e.g., while(1).
 *
 * Common Mistakes:
 *   - Forgetting to update the condition variable (infinite loop)
 *   - Using assignment (=) instead of equality (==) in condition
 *   - Not initializing the loop variable before the loop
 * ====================================================================
 */

#include <iostream>

int main()
{
    // --- Sentinel pattern: sum until 0 ---
    int num{}, sum{};
    std::cout << "Enter numbers to sum (0 to stop):\n";
    std::cin >> num;
    while (num != 0)
    {
        sum += num;
        std::cin >> num;
    }
    std::cout << "Sum: " << sum << '\n';

    // --- Sentinel pattern with sentinel -1 ---
    int val{}, total{};
    std::cout << "\nEnter positive numbers (-1 to quit):\n";
    while (std::cin >> val && val != -1)
    {
        total += val;
    }
    std::cout << "Sum: " << total << '\n';

    // --- Flag-controlled loop ---
    bool done{false};
    int count{};
    while (!done)
    {
        ++count;
        if (count >= 3) done = true;
    }
    std::cout << "Flag loop ran " << count << " times.\n";

    return 0;
}
