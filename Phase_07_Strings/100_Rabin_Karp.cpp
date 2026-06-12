/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — Rabin-Karp Algorithm                   ║
* ║  Program 100 — Rolling hash, collision handling            ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Rabin-Karp uses a rolling hash to find pattern in text.
*   hash("abc") = a * B² + b * B¹ + c * B⁰  (mod M).
* - Rolling update: remove leftmost char, add new rightmost char:
*   new_hash = (old_hash - a * B^(m-1)) * B + new_char  (mod M).
* - When hash matches, verify character-by-character (handles
*   collisions).
* - Base B (e.g. 256) and Mod M (e.g. 10⁹+7) should be chosen to
*   minimise collisions.
*
* SAMPLE I/O:
*   Text:    "ABABABDABACDABABCABAB"
*   Pattern: "ABABCABAB"
*   Rabin-Karp: found at index 10
*
* COMPLEXITY:
*   Time: O(n + m) average, O(n*m) worst (all hashes collide).
*   Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: Why use modulo arithmetic for hash?
*    A: Keeps hash values in a bounded range, avoiding overflow.
* 2. Q: How to choose B and M?
*    A: B should be > alphabet size. M should be a large prime
*       (e.g. 1e9+7, 1e9+9) to reduce collisions.
* 3. Q: What happens on hash collision?
*    A: Do a direct character comparison to confirm. Worst-case
*       every hash collides → O(n*m).
*
* COMMON MISTAKES:
* - Integer overflow when computing hash (use long long or modular
*   multiplication carefully).
* - Forgetting to pre-compute B^(m-1) for rolling update.
* - Negative hash values after subtraction — add M before % M.
*/

#include <iostream>
#include <string>
#include <vector>

using ll = long long;

constexpr ll B = 256;
constexpr ll M = 1'000'000'007;

std::vector<int> rabin_karp(const std::string& text, const std::string& pattern) {
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    std::vector<int> matches;
    if (m == 0 || m > n) return matches;

    // Precompute B^(m-1) mod M
    ll pow = 1;
    for (int i = 0; i < m - 1; ++i)
        pow = (pow * B) % M;

    // Compute pattern hash and first window hash
    ll pat_hash = 0, win_hash = 0;
    for (int i = 0; i < m; ++i) {
        pat_hash = (pat_hash * B + pattern[i]) % M;
        win_hash = (win_hash * B + text[i]) % M;
    }

    // Slide over text
    for (int i = 0; i <= n - m; ++i) {
        if (pat_hash == win_hash) {
            // Verify character-by-character (collision check)
            bool match = true;
            for (int j = 0; j < m; ++j)
                if (text[i + j] != pattern[j]) { match = false; break; }
            if (match) matches.push_back(i);
        }

        // Rolling hash: remove left, add right
        if (i < n - m) {
            win_hash = (win_hash - text[i] * pow % M + M) % M;
            win_hash = (win_hash * B + text[i + m]) % M;
        }
    }
    return matches;
}

int main() {
    std::string text{"ABABABDABACDABABCABAB"};
    std::string pattern{"ABABCABAB"};

    std::cout << "Text:    \"" << text << "\"\n";
    std::cout << "Pattern: \"" << pattern << "\"\n";

    auto matches = rabin_karp(text, pattern);
    if (matches.empty()) {
        std::cout << "Rabin-Karp: not found\n";
    } else {
        std::cout << "Rabin-Karp: found at indices: ";
        for (int idx : matches) std::cout << idx << ' ';
        std::cout << '\n';
    }

    // ── Collision example (illustrative) ─────────────────────
    // In practice with M=1e9+7 and B=256, collisions are rare.
    std::cout << "\nCollision handling: direct compare on hash match.\n";
    std::cout << "Base = " << B << ", Mod = " << M << '\n';

    return 0;
}
