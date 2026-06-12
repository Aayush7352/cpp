/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — Anagram Check                          ║
* ║  Program 94 — Sort-and-compare, frequency map, multiset    ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Anagram: two strings containing the same characters with same
*   frequencies (case-insensitive, spaces ignored optionally).
* - Methods:
*   1. Sort both and compare — O(n log n) time, O(1) space (in-place).
*   2. Frequency map (unordered_map / array) — O(n) time, O(k) space.
*   3. std::multiset — O(n log n), less common.
*
* SAMPLE I/O:
*   "listen" and "silent" → anagram
*   "hello"  and "world"  → not anagram
*   "Dormitory" and "dirty room" → anagram (ignoring spaces, case)
*
* COMPLEXITY:
*   Sort: O(n log n), Map: O(n), Multiset: O(n log n).
*
* INTERVIEW QUESTIONS:
* 1. Q: Which method is fastest for very long strings?
*    A: Frequency array (if alphabet is small, e.g. 26 letters) —
*       O(n), minimal constant.
* 2. Q: How to handle Unicode anagrams?
*    A: Normalise (NFC/NFD) first, then compare code points or
*       use ICU collation.
* 3. Q: What if strings have different lengths?
*    A: They cannot be anagrams — early exit.
*
* COMMON MISTAKES:
* - Not converting to same case before comparing.
* - Forgetting to handle spaces (unless specified).
* - Using array index but not accounting for all 256 extended ASCII
*   or Unicode.
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <cctype>

// ── Method 1: Sort and compare ────────────────────────────────
bool is_anagram_sort(std::string a, std::string b) {
    if (a.size() != b.size()) return false;
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

// ── Method 2: Frequency map (array for lowercase English) ────
bool is_anagram_freq(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    int count[26]{};
    for (char c : a) ++count[std::tolower(c) - 'a'];
    for (char c : b) --count[std::tolower(c) - 'a'];
    for (int v : count) if (v != 0) return false;
    return true;
}

// ── Method 3: Multiset ────────────────────────────────────────
bool is_anagram_multiset(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    std::multiset<char> ms(a.begin(), a.end());
    for (char c : b) {
        auto it = ms.find(c);
        if (it == ms.end()) return false;
        ms.erase(it);
    }
    return ms.empty();
}

int main() {
    std::string s1{"listen"}, s2{"silent"};
    std::string s3{"hello"}, s4{"world"};

    std::cout << std::boolalpha;
    std::cout << "sort:     \"" << s1 << "\" vs \"" << s2 << "\": "
              << is_anagram_sort(s1, s2) << '\n';
    std::cout << "freq:     \"" << s1 << "\" vs \"" << s2 << "\": "
              << is_anagram_freq(s1, s2) << '\n';
    std::cout << "multiset: \"" << s1 << "\" vs \"" << s2 << "\": "
              << is_anagram_multiset(s1, s2) << '\n';
    std::cout << "sort:     \"" << s3 << "\" vs \"" << s4 << "\": "
              << is_anagram_sort(s3, s4) << '\n';

    return 0;
}
