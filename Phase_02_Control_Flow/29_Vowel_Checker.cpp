/*
 * ====================================================================
 * Program 29: Vowel/Consonant Checker
 * Theory:
 *   Vowels in English: a, e, i, o, u (and their uppercase variants).
 *   We use switch with fall-through to handle both upper and lower
 *   cases with minimal code.
 *
 *   Fall-through: When a case block lacks a break, execution continues
 *   into the next case. Grouping multiple case labels before a single
 *   body is a common and clean use of intentional fall-through.
 *
 * Sample Input/Output:
 *   Enter a character: a
 *   'a' is a vowel.
 *
 *   Enter a character: Z
 *   'Z' is a consonant.
 *
 *   Enter a character: 5
 *   '5' is not a letter.
 *
 * Time Complexity:  O(1)
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: Why does switch work with char?
 *   A: char is an integral type in C++ (small integer). Characters
 *      map to ASCII values, so switch works naturally.
 *
 *   Q: How to handle 'y' as sometimes a vowel?
 *   A: English is inconsistent. Most CS problems treat 'y' as a
 *      consonant unless specified otherwise.
 *
 * Common Mistakes:
 *   - Forgetting break after a case with body (causing fall-through)
 *   - Not accounting for both uppercase and lowercase
 *   - Assuming non-letter characters are consonants
 * ====================================================================
 */

#include <iostream>
#include <cctype>

int main()
{
    char ch{};
    std::cout << "Enter a character: ";
    std::cin >> ch;

    if (!std::isalpha(ch))
    {
        std::cout << "'" << ch << "' is not a letter.\n";
        return 0;
    }

    switch (ch)
    {
        case 'a': case 'A':
        case 'e': case 'E':
        case 'i': case 'I':
        case 'o': case 'O':
        case 'u': case 'U':
            std::cout << "'" << ch << "' is a vowel.\n";
            break;
        default:
            std::cout << "'" << ch << "' is a consonant.\n";
            break;
    }

    return 0;
}
