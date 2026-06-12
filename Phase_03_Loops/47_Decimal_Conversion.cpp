/*
 * ====================================================================
 * Program 47: Binary/Octal/Hex to Decimal Conversion
 * Theory:
 *   Conversion from any base to decimal uses the positional system:
 *     value = digit × base^position
 *
 *   Binary to decimal: For each bit d at position p, add d × 2^p
 *   Octal to decimal:  d × 8^p
 *   Hex to decimal:     d × 16^p (d = 0-9, A-F = 10-15)
 *
 *   C++ also provides built-in ways:
 *     std::stoi(str, nullptr, base) for string conversion
 *     std::oct, std::dec, std::hex for I/O
 *
 * Sample Input/Output:
 *   Enter a binary number: 1101
 *   Decimal: 13
 *
 *   Enter an octal number: 17
 *   Decimal: 15
 *
 *   Enter a hex number: 1F
 *   Decimal: 31
 *
 * Time Complexity:  O(d) where d = number of digits
 * Space Complexity: O(1)
 *
 * Common Interview Questions:
 *   Q: How does std::stoi handle invalid input?
 *   A: It throws std::invalid_argument for no conversion and
 *      std::out_of_range for overflow.
 *
 *   Q: How to convert decimal to hex?
 *   A: Divide by 16, collect remainders (0-9, A-F).
 *
 * Common Mistakes:
 *   - Not handling uppercase vs lowercase hex letters
 *   - Invalid digit for base (e.g., '8' in octal)
 *   - Not checking for empty input
 * ====================================================================
 */

#include <iostream>
#include <string>
#include <cctype>

// --- Convert a digit character to its integer value ---
int charToValue(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;  // invalid
}

// --- Convert from any base (2-16) to decimal ---
long long toDecimal(const std::string& str, int base)
{
    long long result{};
    for (char c : str)
    {
        int val = charToValue(c);
        if (val < 0 || val >= base)
        {
            std::cerr << "Invalid digit '" << c << "' for base " << base << '\n';
            return -1;
        }
        result = result * base + val;
    }
    return result;
}

int main()
{
    std::string binary, octal, hex;

    // --- Binary to decimal ---
    std::cout << "Enter a binary number: ";
    std::cin >> binary;
    std::cout << "Decimal: " << toDecimal(binary, 2) << '\n';
    std::cout << "Decimal (std::stoi): " << std::stoi(binary, nullptr, 2) << '\n';

    // --- Octal to decimal ---
    std::cout << "Enter an octal number: ";
    std::cin >> octal;
    std::cout << "Decimal: " << toDecimal(octal, 8) << '\n';
    std::cout << "Decimal (std::oct): " << std::stoi(octal, nullptr, 8) << '\n';

    // --- Hex to decimal ---
    std::cout << "Enter a hex number: ";
    std::cin >> hex;
    std::cout << "Decimal: " << toDecimal(hex, 16) << '\n';
    std::cout << "Decimal (std::hex): " << std::stoi(hex, nullptr, 16) << '\n';

    return 0;
}
