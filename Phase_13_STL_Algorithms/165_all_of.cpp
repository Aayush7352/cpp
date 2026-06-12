/*
==================================================================
165. std::all_of
==================================================================
Theory:
- Returns true if predicate returns true for ALL elements
- Short-circuit: stops at first false
- Returns true for empty range (vacuous truth)
- Part of C++11 "of" family: all_of, any_of, none_of
- Predicate must not modify elements
- Useful for validation checks on containers
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <numeric>

int main() {
    std::cout << "=== std::all_of\n\n";

    // --- Basic ---
    std::cout << "1. Basic all_of:\n";
    std::vector<int> v1{2, 4, 6, 8, 10};
    std::vector<int> v2{2, 4, 5, 8, 10};

    auto allEven = [](int x) { return x % 2 == 0; };
    std::cout << "   All even in v1: " << std::all_of(v1.begin(), v1.end(), allEven) << "\n";
    std::cout << "   All even in v2: " << std::all_of(v2.begin(), v2.end(), allEven) << "\n\n";

    // --- Empty range (always true) ---
    std::cout << "2. Empty range (vacuous truth):\n";
    std::vector<int> empty;
    std::cout << "   all_of on empty: "
              << std::all_of(empty.begin(), empty.end(),
                             [](int) { return false; })
              << "\n\n";

    // --- String validation ---
    std::cout << "3. String validation:\n";
    std::string s1 = "HELLO";
    std::string s2 = "Hello";
    std::string s3 = "12345";

    auto isUpper = [](unsigned char c) { return std::isupper(c); };
    auto isDigit = [](unsigned char c) { return std::isdigit(c); };

    std::cout << "   All upper in \"" << s1 << "\": "
              << std::all_of(s1.begin(), s1.end(), isUpper) << "\n";
    std::cout << "   All upper in \"" << s2 << "\": "
              << std::all_of(s2.begin(), s2.end(), isUpper) << "\n";
    std::cout << "   All digits in \"" << s3 << "\": "
              << std::all_of(s3.begin(), s3.end(), isDigit) << "\n\n";

    // --- Vector of strings ---
    std::cout << "4. All strings non-empty:\n";
    std::vector<std::string> words{"apple", "banana", "", "date"};
    auto allNonEmpty = std::all_of(words.begin(), words.end(),
                                   [](const std::string& s) { return !s.empty(); });
    std::cout << "   All non-empty: " << allNonEmpty << "\n\n";

    // --- With custom objects ---
    std::cout << "5. All values positive:\n";
    std::vector<int> data{-1, 2, 3};
    bool allPositive = std::all_of(data.begin(), data.end(),
                                   [](int x) { return x > 0; });
    std::cout << "   Data: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n   All positive: " << allPositive << "\n\n";

    // --- Short-circuit demonstration ---
    std::cout << "6. Short-circuit behavior:\n";
    std::vector<int> vals{1, 2, 3, 999, 5};
    int count = 0;
    bool result = std::all_of(vals.begin(), vals.end(),
                              [&count](int x) {
                                  ++count;
                                  return x < 100;
                              });
    std::cout << "   Checked " << count << " elements (stopped at 999)\n";
    std::cout << "   Result: " << result << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic all_of:
   All even in v1: 1
   All even in v2: 0

2. Empty range (vacuous truth):
   all_of on empty: 1

3. String validation:
   All upper in "HELLO": 1
   All upper in "Hello": 0
   All digits in "12345": 1

4. All strings non-empty:
   All non-empty: 0

5. All values positive:
   Data: -1 2 3
   All positive: 0

6. Short-circuit behavior:
   Checked 4 elements (stopped at 999)
   Result: 0
==================================================================
Complexity:
- Time: O(n) worst case, O(k) where k = first failure (short-circuit)
- Space: O(1)
==================================================================
Interview Questions:
1. Why does all_of return true for empty ranges?
   -> Vacuous truth: "for all elements, P(x) holds" is
      trivially true when there are no elements.

2. How to check if all elements are equal?
   -> all_of(v.begin()+1, v.end(), [&](int x){ return x == v.front(); });
      Or use std::adjacent_find with std::not_equal_to.

3. Can short-circuit depend on order?
   -> Yes, evaluation order is sequential (left to right).
      For parallel version, use std::all_of with execution policy.
==================================================================
Common Mistakes:
- Not expecting true for empty range (vacuous truth)
- Modifying elements in predicate (undefined behavior)
- Forgetting that predicate must return bool (convertible)
- Using where any_of or none_of is more appropriate
- Assuming short-circuit with parallel execution policies
==================================================================
*/
