/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — Word Count                             ║
* ║  Program 96 — std::stringstream, manual flag-based count   ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Word count: number of whitespace-separated tokens.
* - Using std::stringstream (<sstream>): operator>> extracts
*   whitespace-delimited words automatically.
* - Manual iteration: track a boolean "in_word" flag; toggle when
*   transitioning from whitespace → non-whitespace.
*
* SAMPLE I/O:
*   Input: "The quick brown fox jumps over the lazy dog"
*   Word count (ss): 9
*   Word count (manual): 9
*
* COMPLEXITY:
*   Time: O(n), Space: O(1).
*
* INTERVIEW QUESTIONS:
* 1. Q: What is considered a word?
*    A: Typically any contiguous sequence of non-whitespace characters.
* 2. Q: How to handle multiple spaces, tabs, newlines?
*    A: Both methods handle them correctly. Manual flag counts
*       transitions only on first non-space after space.
* 3. Q: How would you count unique words?
*    A: Use std::unordered_set<std::string> and insert each word.
*
* COMMON MISTAKES:
* - Off-by-one in manual approach: counting words twice for consecutive
*   whitespace.
* - Not handling empty string or leading/trailing spaces.
* - Forgetting to include <sstream> for stringstream.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

std::size_t count_words_ss(const std::string& s) {
    std::istringstream stream(s);
    std::string word;
    std::size_t count = 0;
    while (stream >> word) ++count;
    return count;
}

std::size_t count_words_manual(const std::string& s) {
    std::size_t count = 0;
    bool in_word = false;
    for (char c : s) {
        if (std::isspace(c)) {
            in_word = false;
        } else if (!in_word) {
            ++count;
            in_word = true;
        }
    }
    return count;
}

int main() {
    std::string text{"The quick brown fox jumps over the lazy dog"};
    std::cout << "Input: \"" << text << "\"\n";
    std::cout << "Word count (stringstream): " << count_words_ss(text) << '\n';
    std::cout << "Word count (manual):       " << count_words_manual(text) << '\n';

    // Edge cases
    std::cout << "Empty: \"" << "\" → " << count_words_manual("") << '\n';
    std::cout << "Spaces: \"   \" → " << count_words_manual("   ") << '\n';
    std::cout << "Leading/trailing: \"  hi  \" → "
              << count_words_manual("  hi  ") << '\n';

    return 0;
}
