/*
==================================================================
155. std::tuple
==================================================================
Theory:
- Heterogeneous fixed-size collection (2+ elements)
- std::get<index> or std::get<type> to access elements
- std::tie: unpack tuple into existing variables
- Structured binding: auto [a,b,c] = tuple
- std::apply: call function with tuple elements as args (C++17)
- Comparison: lexicographic element-by-element
- tuple_cat: concatenate tuples
*/

#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

// Function to demonstrate std::apply
int add(int a, double b, std::string_view) {
    return a + static_cast<int>(b);
}

int main() {
    std::cout << "=== std::tuple\n\n";

    // --- Construction ---
    std::cout << "1. Construction:\n";
    std::tuple<int, double, std::string> t1(1, 2.5, "hello");
    auto t2 = std::make_tuple(2, 3.14, "world");
    std::tuple t3{3, 4.2f, "ctad"}; // CTAD (C++17)
    std::tuple t4 = t1; // copy

    std::cout << "   t1: " << std::get<0>(t1) << ", "
              << std::get<1>(t1) << ", " << std::get<2>(t1) << "\n\n";

    // --- std::get ---
    std::cout << "2. std::get by index and type:\n";
    std::cout << "   t1[0] = " << std::get<0>(t1) << "\n";
    std::cout << "   t1[1] = " << std::get<1>(t1) << "\n";
    std::cout << "   t1[int] = " << std::get<int>(t1) << " (by type)\n\n";

    // --- Structured binding ---
    std::cout << "3. Structured binding:\n";
    auto [id, score, label] = t1;
    std::cout << "   id=" << id << ", score=" << score
              << ", label=" << label << "\n\n";

    // --- std::tie ---
    std::cout << "4. std::tie (unpack into existing vars):\n";
    int x; double y; std::string z;
    std::tie(x, y, z) = t1;
    std::cout << "   x=" << x << ", y=" << y << ", z=" << z << "\n\n";

    // --- std::ignore ---
    std::cout << "5. std::ignore:\n";
    std::tie(x, std::ignore, z) = t1;
    std::cout << "   x=" << x << ", z=" << z << " (y ignored)\n\n";

    // --- std::apply ---
    std::cout << "6. std::apply (C++17):\n";
    auto result = std::apply(add, t1);
    std::cout << "   apply(add, t1) = " << result << "\n\n";

    // --- tuple_cat ---
    std::cout << "7. tuple_cat:\n";
    auto cat = std::tuple_cat(t1, t2);
    std::cout << "   tuple_cat size = " << std::tuple_size_v<decltype(cat)>
              << "\n   Elements: ";
    std::apply([](auto&&... args) {
        ((std::cout << args << " "), ...);
    }, cat);
    std::cout << "\n\n";

    // --- Comparison ---
    std::cout << "8. Comparison:\n";
    std::tuple a{1, 2.0, "a"};
    std::tuple b{1, 2.0, "b"};
    std::tuple c{1, 3.0, "a"};
    std::cout << "   (1,2,a) < (1,2,b): " << (a < b) << "\n";
    std::cout << "   (1,2,a) < (1,3,a): " << (a < c) << "\n";
    std::cout << "   (1,2,a) == (1,2,a): " << (a == a) << "\n\n";

    // --- Return multiple values ---
    std::cout << "9. Return multiple values:\n";
    auto statistics = [](const std::vector<int>& v) {
        int sum = 0;
        for (int n : v) sum += n;
        double avg = v.empty() ? 0.0 : static_cast<double>(sum) / v.size();
        auto [min, max] = std::minmax_element(v.begin(), v.end());
        return std::tuple(sum, avg, *min, *max);
    };
    std::vector<int> data{4, 2, 9, 1, 7};
    auto [sum, avg, minVal, maxVal] = statistics(data);
    std::cout << "   Data: 4 2 9 1 7\n";
    std::cout << "   Sum=" << sum << ", Avg=" << avg
              << ", Min=" << minVal << ", Max=" << maxVal << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
1. Construction:
   t1: 1, 2.5, hello

2. std::get by index and type:
   t1[0] = 1
   t1[1] = 2.5
   t1[int] = 1 (by type)

3. Structured binding:
   id=1, score=2.5, label=hello

4. std::tie (unpack into existing vars):
   x=1, y=2.5, z=hello

5. std::ignore:
   x=1, z=hello (y ignored)

6. std::apply (C++17):
   apply(add, t1) = 3

7. tuple_cat:
   tuple_cat size = 6
   Elements: 1 2.5 hello 2 3.14 world

8. Comparison:
   (1,2,a) < (1,2,b): 1
   (1,2,a) < (1,3,a): 1
   (1,2,a) == (1,2,a): 1

9. Return multiple values:
   Data: 4 2 9 1 7
   Sum=23, Avg=4.6, Min=1, Max=9
==================================================================
Complexity:
- Construction: O(1) (or O(k) for string elements)
- get: O(1)
- tie: O(1)
- apply: O(1) (compiler inlines)
- Comparison: O(k) where k = number of elements
==================================================================
Interview Questions:
1. tuple vs struct?
   -> tuple: generic, no naming, good for temporary grouping.
      struct: named fields, better readability for permanent types.

2. How to iterate over tuple elements?
   -> Use std::apply with variadic lambda, or template
      metaprogramming (fold expressions). No runtime loop.

3. When does std::get<type> compile error?
   -> When multiple elements have the same type.
==================================================================
Common Mistakes:
- Using runtime index with std::get (must be compile-time)
- Forgetting #include <tuple>
- Using tie without pre-declared variables
- std::get<type> ambiguous when duplicate types exist
- Modifying tuple element through get<T> when T is reference
==================================================================
*/
