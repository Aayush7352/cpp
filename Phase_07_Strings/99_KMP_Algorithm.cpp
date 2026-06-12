/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — KMP Algorithm (Knuth-Morris-Pratt)     ║
* ║  Program 99 — LPS array construction, O(n+m) search        ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - KMP preprocesses the pattern to build an LPS (Longest Proper
*   Prefix which is also Suffix) array.
* - When a mismatch occurs at position j, LPS[j-1] gives the next
*   character in the pattern to compare without backtracking in text.
* - Key insight: text pointer never goes back → O(n+m) guaranteed.
*
* LPS construction:
*   lps[0] = 0; len tracks length of previous longest prefix-suffix.
*   If pattern[i] == pattern[len], increment len, set lps[i] = len.
*   Else if len != 0, len = lps[len-1]; else lps[i] = 0.
*
* SAMPLE I/O:
*   Text:    "ABABDABACDABABCABAB"
*   Pattern: "ABABCABAB"
*   KMP:     found at index 10
*
* COMPLEXITY:
*   Time: O(n + m) — both LPS build and search are linear.
*   Space: O(m) for LPS array.
*
* INTERVIEW QUESTIONS:
* 1. Q: Why is KMP better than naive?
*    A: It never re-compares characters that have already been
*       matched — avoids O(n*m) worst-case.
* 2. Q: What does LPS[i] represent?
*    A: The length of the longest proper prefix of pattern[0..i]
*       that is also a suffix of pattern[0..i].
* 3. Q: Can KMP handle multiple pattern searches?
*    A: Yes — build LPS once, then scan text. For multiple patterns,
*       consider Aho-Corasick automaton.
*
* COMMON MISTAKES:
* - Off-by-one in LPS construction (compare pattern[i] vs pattern[len]).
* - Forgetting that LPS[0] = 0 always.
* - Infinite loop in LPS while (len > 0 && mismatch) block.
*/

#include <iostream>
#include <string>
#include <vector>

std::vector<int> build_lps(const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> lps(m, 0);
    int len = 0;   // length of previous longest prefix-suffix
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            ++len;
            lps[i] = len;
            ++i;
        } else {
            if (len != 0) {
                len = lps[len - 1];   // fallback
            } else {
                lps[i] = 0;
                ++i;
            }
        }
    }
    return lps;
}

std::vector<int> kmp_search(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    if (pattern.empty()) return matches;

    auto lps = build_lps(pattern);
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    int i = 0, j = 0;

    while (i < n) {
        if (text[i] == pattern[j]) {
            ++i; ++j;
            if (j == m) {
                matches.push_back(i - j);
                j = lps[j - 1];
            }
        } else {
            if (j != 0) j = lps[j - 1];
            else ++i;
        }
    }
    return matches;
}

int main() {
    std::string text{"ABABDABACDABABCABAB"};
    std::string pattern{"ABABCABAB"};

    std::cout << "Text:    \"" << text << "\"\n";
    std::cout << "Pattern: \"" << pattern << "\"\n";

    auto matches = kmp_search(text, pattern);
    if (matches.empty()) {
        std::cout << "KMP: not found\n";
    } else {
        std::cout << "KMP: found at indices: ";
        for (int idx : matches) std::cout << idx << ' ';
        std::cout << '\n';
    }

    // Show LPS
    auto lps = build_lps(pattern);
    std::cout << "LPS array: ";
    for (int v : lps) std::cout << v << ' ';
    std::cout << '\n';

    return 0;
}
