/*
==================================================================
135. Comparison Operators (<=>, ==, !=, <, >, <=, >=)
==================================================================
Theory:
- C++20 spaceship operator <=> returning std::strong_ordering
  (or weak/partial)
- Compiler can auto-generate ==, !=, <, >, <=, >= from <=> and ==
- Three-way comparison: (a <=> b) < 0 means a < b
- Use = default for member-wise comparison
*/

#include <iostream>
#include <compare>
#include <string>
#include <algorithm>

class Person {
private:
    std::string m_name;
    int m_age;
    double m_height; // meters

public:
    Person(std::string name, int age, double height)
        : m_name(std::move(name)), m_age(age), m_height(height) {}

    // C++20 defaulted spaceship — lexicographic comparison
    // using all members in declaration order
    auto operator<=>(const Person&) const = default;

    // Need explicit == because <=> doesn't generate == pre-C++20
    // In C++20, defaulted <=> also generates == implicitly
    // But we show explicit for clarity:
    bool operator==(const Person&) const = default;

    const std::string& name()   const noexcept { return m_name; }
    int age()   const noexcept { return m_age; }
    double height() const noexcept { return m_height; }
};

// Example with partial ordering (e.g., case-insensitive string)
struct CaseInsensitiveString {
    std::string s;

    std::weak_ordering operator<=>(const CaseInsensitiveString& rhs) const {
        std::string lower_lhs, lower_rhs;
        lower_lhs.resize(s.size());
        lower_rhs.resize(rhs.s.size());
        std::transform(s.begin(), s.end(), lower_lhs.begin(), ::tolower);
        std::transform(rhs.s.begin(), rhs.s.end(), lower_rhs.begin(), ::tolower);
        return lower_lhs <=> lower_rhs;
    }

    bool operator==(const CaseInsensitiveString& rhs) const {
        return (*this <=> rhs) == 0;
    }
};

int main() {
    std::cout << "=== Comparison Operators (C++20 <=>)\n\n";

    Person alice("Alice", 30, 1.70);
    Person bob("Bob", 25, 1.85);
    Person alice2("Alice", 30, 1.70);

    // All six comparison operators work
    std::cout << "Comparisons:\n";
    std::cout << "alice == alice2 : " << (alice == alice2) << "\n";
    std::cout << "alice != bob    : " << (alice != bob) << "\n";
    std::cout << "alice <  bob    : " << (alice < bob) << "\n";
    std::cout << "alice >  bob    : " << (alice > bob) << "\n";
    std::cout << "alice <= bob    : " << (alice <= bob) << "\n";
    std::cout << "alice >= bob    : " << (alice >= bob) << "\n\n";

    // Three-way comparison result
    auto cmp = alice <=> bob;
    if (cmp < 0)
        std::cout << "alice < bob\n";
    else if (cmp > 0)
        std::cout << "alice > bob\n";
    else
        std::cout << "alice == bob\n";

    // Case-insensitive
    std::cout << "\nCase-insensitive string:\n";
    CaseInsensitiveString cis1{"Hello"};
    CaseInsensitiveString cis2{"hello"};
    std::cout << "\"Hello\" == \"hello\" : " << (cis1 == cis2) << "\n";
    std::cout << "\"ABC\" < \"abc\"     : " << (CaseInsensitiveString{"ABC"} < CaseInsensitiveString{"abc"}) << "\n";

    return 0;
}

/*
==================================================================
Sample I/O:
Comparisons:
alice == alice2 : 1
alice != bob    : 1
alice <  bob    : 1   (lexicographic: "Alice" < "Bob")
alice >  bob    : 0
alice <= bob    : 1
alice >= bob    : 0

alice < bob

Case-insensitive string:
"Hello" == "hello" : 1
"ABC" < "abc"     : 0
==================================================================
Complexity:
- Time: O(n) for string members (lexicographic)
- Space: O(1) (no allocation for <=>)
==================================================================
Interview Questions:
1. What does auto operator<=>(const T&) const = default; generate?
   -> Member-wise lexicographic comparison, returning
      std::strong_ordering (for integral types) or the
      common comparison category.

2. Difference between strong/weak/partial ordering?
   -> strong_ordering: substitutable (a == b implies
      indistinguishable). weak_ordering: equivalence classes.
      partial_ordering: some pairs incomparable (e.g. NaN).

3. Do we still need to define == explicitly?
   -> In C++20, defaulted <=> implicitly declares == as defaulted.
      But if you define <=> manually, you must define == manually.
==================================================================
Common Mistakes:
- Forgetting const qualifier on comparison operators
- Returning bool instead of comparison category
- Assuming defaulted <=> uses custom comparison for members
  (it does member-wise, but if a member has custom <=>, that's used)
- Using <=> where simple equality is sufficient (overkill)
==================================================================
*/
