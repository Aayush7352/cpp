/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — Substring Search                       ║
* ║  Program 98 — Naive O(n*m), std::string::find              ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Naive substring search: slide pattern over text, compare
*   character by character. O(n*m) worst-case.
* - std::string::find uses an unspecified algorithm (often a hybrid
*   like Two-Way or Boyer-Moore in practice, but worst-case can
*   still be O(n*m) in the standard — quality of implementation
*   dependent).
*
* SAMPLE I/O:
*   Text:    "ABABABDABACDABABCABAB"
*   Pattern: "ABABCABAB"
*   Naive:   found at index 10
*   find:    found at index 10
*
* COMPLEXITY:
*   Naive: O(n*m).
*   find:  O(n*m) worst case, often faster in practice.
*
* INTERVIEW QUESTIONS:
* 1. Q: Worst case for naive search?
*    A: Text "AAAA…AAAB", pattern "AAAB" — many false starts.
* 2. Q: How does find differ from KMP?
*    A: KMP guarantees O(n+m) using LPS table; find may be
*       simpler but slower in worst case.
* 3. Q: Can find handle empty pattern?
*    A: Yes — returns 0 (empty string matches at position 0).
*
* COMMON MISTAKES:
* - Off-by-one in loop: j should go up to m, not m-1.
* - Not resetting j correctly on mismatch.
* - Confusing npos with a valid index.
*/

#include <iostream>
#include <string>

// Naive O(n*m) substring search — returns index or -1
int naive_search(const std::string& text, const std::string& pattern) {
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0) return 0;

    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) ++j;
        if (j == m) return i;
    }
    return -1;
}

int main() {
    std::string text{"ABABABDABACDABABCABAB"};
    std::string pattern{"ABABCABAB"};

    // ── Naive search ─────────────────────────────────────────
    int idx = naive_search(text, pattern);
    std::cout << "Text:    \"" << text << "\"\n";
    std::cout << "Pattern: \"" << pattern << "\"\n";
    if (idx != -1)
        std::cout << "Naive:   found at index " << idx << '\n';
    else
        std::cout << "Naive:   not found\n";

    // ── std::string::find ──────────────────────────────────
    auto pos = text.find(pattern);
    if (pos != std::string::npos)
        std::cout << "find:    found at index " << pos << '\n';
    else
        std::cout << "find:    not found\n";

    // ── Not found case ──────────────────────────────────────
    std::cout << "\nSearch for \"XYZ\": ";
    pos = text.find("XYZ");
    std::cout << (pos == std::string::npos ? "not found" : "found") << '\n';

    return 0;
}
