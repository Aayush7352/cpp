/*
* ╔══════════════════════════════════════════════════════════════╗
* ║  PHASE 7: STRINGS — String Compression (Run-Length Encoding)║
* ║  Program 97 — RLE compression, std::string compression     ║
* ╚══════════════════════════════════════════════════════════════╝
*
* THEORY:
* - Run-length encoding (RLE): replace consecutive repeated
*   characters with the character and its count.
*   e.g. "aaabbcddd" → "a3b2c1d3".
* - If count is 1, often just the character (no digit). Common
*   variants differ; here we always include count.
* - std::string compression: use to_string for count.
*
* SAMPLE I/O:
*   Input:  "aaabbcddd"
*   Output: "a3b2c1d3"
*   Input:  "abcd"
*   Output: "a1b1c1d1"
*
* COMPLEXITY:
*   Time: O(n) — single pass.
*   Space: O(n) for output string (could be longer than input).
*
* INTERVIEW QUESTIONS:
* 1. Q: When is RLE useful?
*    A: When data contains long runs (e.g. simple graphics, fax
*       transmission). Poor for random data (doubles size).
* 2. Q: What is the worst case for RLE?
*    A: No repeats — "abc" → "a1b1c1" which is 2× the input.
* 3. Q: How to decompress RLE?
*    A: Read char, read count, append char count times.
*
* COMMON MISTAKES:
* - Off-by-one: last run not appended after loop.
* - Using char for count that exceeds 9 (use multi-digit encoding
*   or limit to 9+ with special marker).
* - Not accounting for count = 1 (variant specific).
*/

#include <iostream>
#include <string>

std::string compress(const std::string& s) {
    if (s.empty()) return {};

    std::string result;
    char current = s[0];
    int count = 1;

    for (std::size_t i = 1; i < s.size(); ++i) {
        if (s[i] == current) {
            ++count;
        } else {
            result += current + std::to_string(count);
            current = s[i];
            count = 1;
        }
    }
    result += current + std::to_string(count);   // last run
    return result;
}

std::string decompress(const std::string& s) {
    std::string result;
    for (std::size_t i = 0; i < s.size(); i += 2) {
        char ch = s[i];
        int count = s[i + 1] - '0';   // single-digit count
        result.append(count, ch);
    }
    return result;
}

int main() {
    std::string input{"aaabbcddd"};
    std::string compressed = compress(input);
    std::cout << "Input:    \"" << input << "\"\n";
    std::cout << "Compressed: \"" << compressed << "\"\n";
    std::cout << "Decompressed: \"" << decompress(compressed) << "\"\n";

    std::string input2{"abcd"};
    std::cout << "\nInput:    \"" << input2 << "\"\n";
    std::cout << "Compressed: \"" << compress(input2) << "\"\n";

    return 0;
}
