/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — std::string Overview                   ║
* ║  Program 91 — Construction, concatenation, comparison,      ║
* ║               substr, find, rfind, npos                    ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - std::string (C++98, <string>) is a contiguous, dynamically-sized
*   sequence of characters. C++11 guarantees contiguous storage.
* - Construction: from literal, C-string, count+char, iterator pair,
*   initializer list, copy/move.
* - Concatenation: operator+, operator+=, append, push_back.
* - Comparison: operator==, !=, <, <=, >, >= (lexicographical);
*   compare() for partial / case-sensitive comparison.
* - substr(pos, count): O(count) — copies characters.
* - find(str, pos): returns index of first occurrence or npos.
* - rfind(str, pos): returns index of last occurrence or npos.
* - npos: static constexpr std::string::npos = -1 (largest size_t).
*
* SAMPLE I/O:
*   s1 = "Hello", s2 = "World"
*   s1 + " " + s2 = "Hello World"
*   substr(0,5) = "Hello"
*   find("World") = 6
*
* COMPLEXITY:
*   find/rfind: O(n*m) worst-case (no KMP optimisation in standard).
*   substr: O(count).
*
* INTERVIEW QUESTIONS:
* 1. Q: What does npos equal?
*    A: static_cast<std::size_t>(-1). Check with == npos, not >= 0.
* 2. Q: When does `data()` return null-terminated string?
*    A: Since C++11, yes — s.data()[s.size()] == '\0'.
* 3. Q: Difference between size() and capacity()?
*    A: size() is number of characters; capacity() is allocated
*       storage (may be larger).
*
* COMMON MISTAKES:
* - Assuming find returns bool (it returns index, use == npos check).
* - Modifying string while iterating (iterator invalidation).
* - Using substr when string_view would avoid allocation.
*/

#include <iostream>
#include <string>
using namespace std::string_literals;

int main() {
    // ── Construction ─────────────────────────────────────────
    std::string s1{"Hello"};            // from literal
    std::string s2(5, 'W');             // "WWWWW"
    std::string s3 = "World";

    std::cout << "s1 = \"" << s1 << "\", s2 = \"" << s2
              << "\", s3 = \"" << s3 << "\"\n";

    // ── Concatenation ────────────────────────────────────────
    std::string s4 = s1 + " " + s3;     // "Hello World"
    std::cout << "s1 + \" \" + s3 = \"" << s4 << "\"\n";

    s1 += " there";
    std::cout << "s1 after += \" there\" = \"" << s1 << "\"\n";

    // ── Comparison ───────────────────────────────────────────
    std::cout << std::boolalpha;
    std::cout << "\"abc\" == \"abc\": " << ("abc"s == "abc"s) << '\n';
    std::cout << "\"abc\" < \"abd\":  " << ("abc"s < "abd"s) << '\n';

    // ── substr ───────────────────────────────────────────────
    std::string s5 = "Hello World";
    std::cout << "substr(0,5) = \"" << s5.substr(0, 5) << "\"\n";
    std::cout << "substr(6)   = \"" << s5.substr(6) << "\"\n";

    // ── find / rfind / npos ──────────────────────────────────
    auto pos = s5.find("World");
    if (pos != std::string::npos)
        std::cout << "find(\"World\") = " << pos << '\n';

    pos = s5.find('o');
    std::cout << "find('o') = " << pos << '\n';

    pos = s5.rfind('o');
    std::cout << "rfind('o') = " << pos << '\n';

    // Not found
    pos = s5.find("xyz");
    std::cout << "find(\"xyz\") == npos: " << (pos == std::string::npos) << '\n';

    return 0;
}
