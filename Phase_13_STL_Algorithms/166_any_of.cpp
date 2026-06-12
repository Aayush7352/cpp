/*
==================================================================
166. std::any_of
==================================================================
Theory:
- Returns true if predicate returns true for AT LEAST ONE element
- Short-circuit: stops at first true
- Returns false for empty range
- Part of C++11 "of" family
- Useful for: checking if any element satisfies condition
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <map>

int main() {
    std::cout << "=== std::any_of\n\n";

    // --- Basic ---
    std::cout << "1. Basic any_of (negative numbers):\n";
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{1, -2, 3, 4, 5};

    auto isNegative = [](int x) { return x < 0; };
    std::cout << "   Any negative in v1: "
              << std::any_of(v1.begin(), v1.end(), isNegative) << "\n";
    std::cout << "   Any negative in v2: "
              << std::any_of(v2.begin(), v2.end(), isNegative) << "\n\n";

    // --- Empty range ---
    std::cout << "2. Empty range (false):\n";
    std::vector<int> empty;
    std::cout << "   any_of on empty: "
              << std::any_of(empty.begin(), empty.end(),
                              [](int) { return true; })
              << "\n\n";

    // --- String check ---
    std::cout << "3. Check if string has lowercase:\n";
    std::string s1 = "HELLO";
    std::string s2 = "Hello";

    auto isLower = [](unsigned char c) { return std::islower(c); };
    std::cout << "   Any lower in \"" << s1 << "\": "
              << std::any_of(s1.begin(), s1.end(), isLower) << "\n";
    std::cout << "   Any lower in \"" << s2 << "\": "
              << std::any_of(s2.begin(), s2.end(), isLower) << "\n\n";

    // --- Map check ---
    std::cout << "4. Map: any value > 100:\n";
    std::map<std::string, int> scores{{"Alice", 95}, {"Bob", 105}, {"Charlie", 87}};
    bool hasHigh = std::any_of(scores.begin(), scores.end(),
                                [](const auto& pair) { return pair.second > 100; });
    std::cout << "   Any score > 100: " << hasHigh << "\n\n";

    // --- Short-circuit ---
    std::cout << "5. Short-circuit demonstration:\n";
    std::vector<int> vals{10, 20, 999, 30, 40};
    int count = 0;
    bool found = std::any_of(vals.begin(), vals.end(),
                              [&count](int x) {
                                  ++count;
                                  return x > 100;
                              });
    std::cout << "   Checked " << count << " elements (stopped at 999)\n";
    std::cout << "   Found > 100: " << found << "\n\n";

    // --- Practical: validation ---
    std::cout << "6. Practical validation:\n";
    auto hasSpecialChar = [](const std::string& s) {
        return std::any_of(s.begin(), s.end(), [](unsigned char c) {
            return !std::isalnum(c) && c != ' ';
        });
    };
    std::cout << "   \"hello123\" has special: " << hasSpecialChar("hello123") << "\n";
    std::cout << "   \"hello!@#\" has special: " << hasSpecialChar("hello!@#") << "\n";

    // --- Algorithm composition ---
    std::cout << "\n7. Compose: any_of and is_partitioned:\n";
    std::vector<int> mixed{-5, -3, 0, 2, 7};
    bool anyPositive = std::any_of(mixed.begin(), mixed.end(),
                                    [](int x) { return x > 0; });
    bool allNonZero = std::none_of(mixed.begin(), mixed.end(),
                                    [](int x) { return x == 0; });
    std::cout << "   Any positive: " << anyPositive
              << ", All non-zero: " << allNonZero << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic any_of (negative numbers):
   Any negative in v1: 0
   Any negative in v2: 1

2. Empty range (false):
   any_of on empty: 0

3. Check if string has lowercase:
   Any lower in "HELLO": 0
   Any lower in "Hello": 1

4. Map: any value > 100:
   Any score > 100: 1

5. Short-circuit demonstration:
   Checked 3 elements (stopped at 999)
   Found > 100: 1

6. Practical validation:
   "hello123" has special: 0
   "hello!@#" has special: 1

7. Compose: any_of and is_partitioned:
   Any positive: 1, All non-zero: 0
==================================================================
Complexity:
- Time: O(n) worst case, O(k) where k = first true
- Space: O(1)
==================================================================
Interview Questions:
1. any_of vs all_of vs none_of?
   -> any_of: at least one true. all_of: all true.
      none_of: none true (inverse of any_of).

2. When does any_of return false?
   -> When no element satisfies predicate, or range is empty.

3. De Morgan's laws with "of" family:
   -> !any_of == none_of (inverse of predicate).
      !all_of == any_of (inverse of predicate).
==================================================================
Common Mistakes:
- Expecting true from empty range (returns false)
- Using any_of when none_of is clearer (inverting predicate)
- Forgetting to include <algorithm>
- Not leveraging short-circuit behavior for performance
- Modifying elements in predicate
==================================================================
*/
