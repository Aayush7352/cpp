/*
==================================================================
167. std::none_of
==================================================================
Theory:
- Returns true if predicate returns false for ALL elements
- Short-circuit: stops at first true
- Returns true for empty range (vacuous truth)
- Logical inverse of any_of with same predicate
- Part of C++11 "of" family (all_of, any_of, none_of)
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <set>

int main() {
    std::cout << "=== std::none_of\n\n";

    // --- Basic ---
    std::cout << "1. Basic none_of (zero check):\n";
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{1, 0, 3, 4, 5};

    auto isZero = [](int x) { return x == 0; };
    std::cout << "   No zero in v1: "
              << std::none_of(v1.begin(), v1.end(), isZero) << "\n";
    std::cout << "   No zero in v2: "
              << std::none_of(v2.begin(), v2.end(), isZero) << "\n\n";

    // --- Empty range ---
    std::cout << "2. Empty range (vacuous truth):\n";
    std::vector<int> empty;
    std::cout << "   none_of on empty: "
              << std::none_of(empty.begin(), empty.end(),
                              [](int) { return true; })
              << "\n\n";

    // --- String check ---
    std::cout << "3. Validate string content:\n";
    auto noDigits = [](const std::string& s) {
        return std::none_of(s.begin(), s.end(),
                            [](unsigned char c) { return std::isdigit(c); });
    };
    std::cout << "   \"hello\" has no digits: " << noDigits("hello") << "\n";
    std::cout << "   \"abc123\" has no digits: " << noDigits("abc123") << "\n\n";

    // --- Set check ---
    std::cout << "4. Set: no even numbers:\n";
    std::set<int> s{1, 3, 5, 7, 9};
    bool noEven = std::none_of(s.begin(), s.end(),
                                [](int x) { return x % 2 == 0; });
    std::cout << "   No evens: " << noEven << "\n\n";

    // --- Short-circuit ---
    std::cout << "5. Short-circuit demonstration:\n";
    std::vector<int> vals{10, 20, 999, 30, 40};
    int count = 0;
    bool nonNegative = std::none_of(vals.begin(), vals.end(),
                                    [&count](int x) {
                                        ++count;
                                        return x < 0;
                                    });
    std::cout << "   Checked " << count << " elements (all non-negative)\n";
    std::cout << "   All non-negative: " << nonNegative << "\n\n";

    // --- Practical: Validation ---
    std::cout << "6. Practical: check for duplicates:\n";
    std::vector<int> data{1, 2, 3, 4, 5};
    std::set<int> seen;
    bool noDuplicates = std::none_of(data.begin(), data.end(),
                                      [&seen](int x) {
                                          if (seen.count(x)) return true;
                                          seen.insert(x);
                                          return false;
                                      });
    std::cout << "   Data: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n   No duplicates: " << noDuplicates << "\n\n";

    // --- De Morgan relationships ---
    std::cout << "7. De Morgan relationship:\n";
    std::vector<int> nums{-1, 2, 3};
    auto posPred = [](int x) { return x > 0; };

    bool notAnyPos = !std::any_of(nums.begin(), nums.end(), posPred);
    bool nonePos = std::none_of(nums.begin(), nums.end(), posPred);
    bool allNeg = std::all_of(nums.begin(), nums.end(),
                              [](int x) { return x <= 0; });

    std::cout << "   !any_of(positive) == none_of(positive): "
              << (notAnyPos == nonePos) << "\n";
    std::cout << "   none_of(positive) == all_of(!positive): "
              << (nonePos == allNeg) << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic none_of (zero check):
   No zero in v1: 1
   No zero in v2: 0

2. Empty range (vacuous truth):
   none_of on empty: 1

3. Validate string content:
   "hello" has no digits: 1
   "abc123" has no digits: 0

4. Set: no even numbers:
   No evens: 1

5. Short-circuit demonstration:
   Checked 5 elements (all non-negative)
   All non-negative: 1

6. Practical: check for duplicates:
   Data: 1 2 3 4 5
   No duplicates: 1

7. De Morgan relationship:
   !any_of(positive) == none_of(positive): 1
   none_of(positive) == all_of(!positive): 1
==================================================================
Complexity:
- Time: O(n) worst case, O(k) where k = first true
- Space: O(1) (or O(n) if storing state like duplicates check)
==================================================================
Interview Questions:
1. De Morgan's law: none_of(p) == !any_of(p)?
   -> Yes. And all_of(p) == none_of(!p).

2. What does none_of return for empty range?
   -> True (vacuous truth — no element fails the predicate).

3. none_of vs all_of with negated predicate?
   -> none_of(begin, end, pred) == all_of(begin, end, !pred)
==================================================================
Common Mistakes:
- Confusing none_of with all_of (forgot to invert predicate)
- Using none_of when all_of with !pred would be clearer
- Expecting false for empty range (returns true)
- Modifying elements in predicate
- Not taking advantage of short-circuit evaluation
==================================================================
*/
