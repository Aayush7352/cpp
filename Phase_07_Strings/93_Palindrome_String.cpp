/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — Palindrome Check                       ║
* ║  Program 93 — Case-sensitive, case-insensitive,            ║
* ║               ignoring non-alphanumeric                    ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Palindrome: string reads the same forward and backward.
* - Case-sensitive: direct character comparison.
* - Case-insensitive: use std::tolower or std::toupper.
* - Ignoring non-alphanumeric: use std::isalnum to skip spaces,
*   punctuation, etc.
*
* SAMPLE I/O:
*   "racecar" → palindrome (case-sensitive)
*   "RaceCar" → not palindrome (case-sensitive)
*   "RaceCar" → palindrome (case-insensitive)
*   "A man, a plan, a canal: Panama" → palindrome (ignoring non-alnum)
*
* COMPLEXITY:
*   Time: O(n), Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: What about empty string and single character?
*    A: Both are trivially palindromes.
* 2. Q: How would you handle Unicode palindromes?
*    A: Use ICU collation for case-folding and grapheme clustering.
* 3. Q: Can you check palindrome without modifying the string?
*    A: Yes — two-pointer with index-based access (shown below).
*
* COMMON MISTAKES:
* - Forgetting to convert both characters when comparing case-insensitively.
* - Off-by-one: right should start at s.size() - 1 (not size()).
* - Not handling empty string (left < right condition handles it).
*/

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

bool is_palindrome_case_sensitive(const std::string& s) {
    std::size_t l = 0, r = s.size();
    while (l < r)
        if (s[l++] != s[--r]) return false;
    return true;
}

bool is_palindrome_case_insensitive(const std::string& s) {
    std::size_t l = 0, r = s.size();
    while (l < r)
        if (std::tolower(s[l++]) != std::tolower(s[--r])) return false;
    return true;
}

bool is_palindrome_alnum(const std::string& s) {
    std::size_t l = 0, r = s.size();
    while (l < r) {
        if (!std::isalnum(s[l])) { ++l; continue; }
        if (!std::isalnum(s[r])) { --r; continue; }
        if (std::tolower(s[l++]) != std::tolower(s[--r])) return false;
    }
    return true;
}

int main() {
    std::string a{"racecar"};
    std::string b{"RaceCar"};
    std::string c{"A man, a plan, a canal: Panama"};

    std::cout << std::boolalpha;
    std::cout << "\"" << a << "\" (case-sensitive): "
              << is_palindrome_case_sensitive(a) << '\n';
    std::cout << "\"" << b << "\" (case-sensitive): "
              << is_palindrome_case_sensitive(b) << '\n';
    std::cout << "\"" << b << "\" (case-insensitive): "
              << is_palindrome_case_insensitive(b) << '\n';
    std::cout << "\"" << c << "\" (alnum only): "
              << is_palindrome_alnum(c) << '\n';

    return 0;
}
