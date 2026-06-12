/*
==================================================================
164. std::count_if
==================================================================
Theory:
- Counts elements satisfying a predicate
- Returns difference_type (usually ptrdiff_t)
- Predicate: callable returning bool (lambda, function pointer,
  functor)
- Linear scan O(n)
- std::count: counts elements equal to a value (uses operator==)
- std::count_if: counts elements where predicate returns true

Other counting algorithms:
- std::count: count of specific value
- std::ranges::count_if (C++20): range-based version
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>

int main() {
    std::cout << "=== std::count_if\n\n";

    // --- Basic ---
    std::cout << "1. Basic count_if (even numbers):\n";
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto nEven = std::count_if(v.begin(), v.end(),
                               [](int x) { return x % 2 == 0; });
    std::cout << "   Even numbers: " << nEven << "\n\n";

    // --- Multiple conditions ---
    std::cout << "2. Count numbers in range [3, 7]:\n";
    auto inRange = std::count_if(v.begin(), v.end(),
                                 [](int x) { return x >= 3 && x <= 7; });
    std::cout << "   Count in [3,7]: " << inRange << "\n\n";

    // --- With strings ---
    std::cout << "3. Count strings by length and content:\n";
    std::vector<std::string> words{"cat", "dog", "elephant", "bat",
                                   "fox", "giraffe"};
    auto longWords = std::count_if(words.begin(), words.end(),
                                   [](const std::string& s) {
                                       return s.length() > 4;
                                   });
    std::cout << "   Words longer than 4: " << longWords << "\n";

    auto startsWithB = std::count_if(words.begin(), words.end(),
                                     [](const std::string& s) {
                                         return !s.empty() && s[0] == 'b';
                                     });
    std::cout << "   Words starting with 'b': " << startsWithB << "\n\n";

    // --- Using function pointer ---
    std::cout << "4. Using function pointer:\n";
    auto isPrime = [](int n) -> bool {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i)
            if (n % i == 0) return false;
        return true;
    };
    auto nPrimes = std::count_if(v.begin(), v.end(), isPrime);
    std::cout << "   Primes in 1..10: " << nPrimes << "\n\n";

    // --- With char/string characteristics ---
    std::cout << "5. Count characters in string:\n";
    std::string text = "Hello World 2024!";
    auto digits = std::count_if(text.begin(), text.end(),
                                [](unsigned char c) { return std::isdigit(c); });
    auto spaces = std::count_if(text.begin(), text.end(),
                                [](unsigned char c) { return std::isspace(c); });
    auto upper = std::count_if(text.begin(), text.end(),
                               [](unsigned char c) { return std::isupper(c); });
    std::cout << "   Text: \"" << text << "\"\n";
    std::cout << "   Digits: " << digits << "\n";
    std::cout << "   Spaces: " << spaces << "\n";
    std::cout << "   Uppercase: " << upper << "\n\n";

    // --- std::count (value equality) ---
    std::cout << "6. std::count (value equality):\n";
    std::vector<int> dup{1, 2, 2, 3, 2, 4, 2, 5};
    auto nTwos = std::count(dup.begin(), dup.end(), 2);
    std::cout << "   Count of 2: " << nTwos << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Basic count_if (even numbers):
   Even numbers: 5

2. Count numbers in range [3, 7]:
   Count in [3,7]: 5

3. Count strings by length and content:
   Words longer than 4: 2
   Words starting with 'b': 1

4. Using function pointer:
   Primes in 1..10: 4

5. Count characters in string:
   Text: "Hello World 2024!"
   Digits: 4
   Spaces: 2
   Uppercase: 2

6. std::count (value equality):
   Count of 2: 4
==================================================================
Complexity:
- Time: O(n)
- Space: O(1)
==================================================================
Interview Questions:
1. count vs count_if?
   -> count: counts elements == given value (uses operator==).
      count_if: counts elements satisfying predicate.

2. What does count_if return?
   -> iterator_traits<InputIterator>::difference_type (signed integer,
      typically ptrdiff_t).

3. Can count_if be used on associative containers?
   -> Yes, for set/map (though set has count() member function
      which is O(log n) vs O(n) for algorithm).
==================================================================
Common Mistakes:
- Forgetting to dereference iterator in predicate
- Using count_if where count would suffice
- Using with associative containers when member count() is faster
- Predicate modifying elements (should be const)
- Not including <algorithm>
==================================================================
*/
