/*
==================================================================
163. std::transform
==================================================================
Theory:
- Unary transform: apply function to each element, write to output
- Binary transform: combine elements from two ranges element-wise
- Output can be same as input (in-place)
- Lambda, function pointer, or functor
- Result iterator determines output destination
- Often chained with back_inserter
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <iterator> // back_inserter
#include <cmath>

int main() {
    std::cout << "=== std::transform\n\n";

    // --- Unary transform (in-place) ---
    std::cout << "1. Unary transform (square in-place):\n";
    std::vector<int> v{1, 2, 3, 4, 5};
    std::transform(v.begin(), v.end(), v.begin(),
                   [](int x) { return x * x; });
    std::cout << "   Squared: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Unary transform (new container) ---
    std::cout << "2. Unary to new container:\n";
    std::vector<int> orig{1, 2, 3, 4, 5};
    std::vector<int> squared;
    squared.reserve(orig.size());
    std::transform(orig.begin(), orig.end(),
                   std::back_inserter(squared),
                   [](int x) { return x * x; });
    std::cout << "   Original: ";
    for (int x : orig) std::cout << x << " ";
    std::cout << "\n   Squared:  ";
    for (int x : squared) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Binary transform ---
    std::cout << "3. Binary transform (element-wise sum):\n";
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<int> b{10, 20, 30, 40, 50};
    std::vector<int> result;
    result.reserve(a.size());
    std::transform(a.begin(), a.end(), b.begin(),
                   std::back_inserter(result),
                   std::plus<int>());
    std::cout << "   A:  ";
    for (int x : a) std::cout << x << " ";
    std::cout << "\n   B:  ";
    for (int x : b) std::cout << x << " ";
    std::cout << "\n   Sum:";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n\n";

    // --- String transformation ---
    std::cout << "4. String to uppercase:\n";
    std::string s = "Hello, World!";
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    std::cout << "   Uppercase: \"" << s << "\"\n\n";

    // --- Lambda with capture ---
    std::cout << "5. Lambda with capture (add offset):\n";
    int offset = 100;
    std::transform(v.begin(), v.end(), v.begin(),
                   [offset](int x) { return x + offset; });
    std::cout << "   With offset " << offset << ": ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n\n";

    // --- Function pointer ---
    std::cout << "6. Using function pointer:\n";
    double arr[] = {1.0, 2.0, 3.0, 4.0};
    std::transform(arr, arr + 4, arr, std::sqrt);
    std::cout << "   Sqrt: ";
    for (double d : arr) std::cout << d << " ";
    std::cout << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Unary transform (square in-place):
   Squared: 1 4 9 16 25

2. Unary to new container:
   Original: 1 2 3 4 5
   Squared:  1 4 9 16 25

3. Binary transform (element-wise sum):
   A:  1 2 3 4 5
   B:  10 20 30 40 50
   Sum: 11 22 33 44 55

4. String to uppercase:
   Uppercase: "HELLO, WORLD!"

5. Lambda with capture (add offset):
   With offset 100: 101 104 109 116 125

6. Using function pointer:
   Sqrt: 1 1.41421 1.73205 2
==================================================================
Complexity:
- Time: O(n) for any transform
- Space: O(1) in-place, O(n) for new container
==================================================================
Interview Questions:
1. transform vs for_each?
   -> transform: assigns result to output range (functional).
      for_each: applies function for side effects, returns function.

2. Can transform be parallelized?
   -> Not directly (C++17). For parallel transform, use
      std::for_each with execution policy, or ranges::views::transform.

3. Binary transform with different sized ranges?
   -> Uses min(len1, len2). Shorter range determines iterations.
==================================================================
Common Mistakes:
- Forgetting to ensure output container has enough space
  (use back_inserter or resize)
- Using transform where for_each is more appropriate
- Not including <iterator> for back_inserter
- Unary vs binary confusion (wrong arity lambda)
- Overwriting source when destination overlaps incorrectly
==================================================================
*/
