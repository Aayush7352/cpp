/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — Character Frequency Analysis           ║
* ║  Program 95 — Frequency analysis, most/least frequent char ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Count occurrences of each character in a string.
* - Most/least frequent: scan frequency map for max/min count.
* - Tie-breaking: on equal frequency, typically report the character
*   that appears first (or last depending on spec).
*
* SAMPLE I/O:
*   String: "mississippi"
*   Frequency: m=1, i=4, s=4, p=2
*   Most frequent: i and s (tie at 4)
*   Least frequent: m (1)
*
* COMPLEXITY:
*   Time: O(n + k) where k = number of distinct characters (≤ 256).
*   Space: O(k).
*
* INTERVIEW QUESTIONS:
* 1. Q: Most frequent character in O(n)?
*    A: Yes — single pass to count, second pass to find max.
* 2. Q: What if the string is empty?
*    A: No character — handle gracefully.
* 3. Q: How would you handle Unicode?
*    A: Use std::unordered_map<char32_t> with UTF-32 decoding.
*
* COMMON MISTAKES:
* - Using char as index without checking range (should handle all 256
*   values or use unordered_map).
* - Not handling tie-breaking correctly.
* - For non-printable characters, output can be confusing.
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <climits>

int main() {
    std::string s{"mississippi"};

    // ── Frequency count ──────────────────────────────────────
    std::unordered_map<char, int> freq;
    for (char c : s) ++freq[c];

    std::cout << "Frequency:\n";
    for (auto [ch, cnt] : freq)
        std::cout << "  '" << ch << "' → " << cnt << '\n';

    // ── Most frequent ───────────────────────────────────────
    char most = s[0];
    int max_cnt = freq[most];
    for (auto [ch, cnt] : freq) {
        if (cnt > max_cnt) {
            max_cnt = cnt;
            most = ch;
        }
    }
    std::cout << "Most frequent: '" << most << "' (" << max_cnt << " times)\n";

    // ── Least frequent ──────────────────────────────────────
    char least = freq.begin()->first;
    int min_cnt = freq[least];
    for (auto [ch, cnt] : freq) {
        if (cnt < min_cnt) {
            min_cnt = cnt;
            least = ch;
        }
    }
    std::cout << "Least frequent: '" << least << "' (" << min_cnt << " time)\n";

    return 0;
}
