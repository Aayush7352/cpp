/*
 * ====================================================================
 * Program 46: Decimal to Binary Conversion
 * Theory:
 *   Decimal to binary: Repeatedly divide by 2, collecting remainders
 *   in reverse order.
 *
 *   Example: 13
 *     13 ÷ 2 = 6 r1  ↑
 *      6 ÷ 2 = 3 r0  |
 *      3 ÷ 2 = 1 r1  |
 *      1 ÷ 2 = 0 r1  |
 *     Binary: 1101
 *
 *   C++20 provides std::bitset for easy binary representation.
 *   std::format (C++20) also supports {:b} format specifier (but not
 *   fully supported by all compilers yet).
 *
 * Sample Input/Output:
 *   Enter a decimal number: 13
 *   Binary (manual): 1101
 *   Binary (bitset):  00000000000000000000000000001101
 *   Binary (trimmed): 1101
 *
 * Time Complexity:  O(log n)
 * Space Complexity: O(log n) for the binary string
 *
 * Common Interview Questions:
 *   Q: How to convert negative numbers to binary?
 *   A: Use two's complement representation. std::bitset handles this
 *      via the underlying unsigned representation.
 *
 *   Q: How to convert binary to decimal?
 *   A: For each bit, result = result * 2 + bit.
 *
 * Common Mistakes:
 *   - Reversing the remainders in wrong order (need reverse)
 *   - Not handling n = 0 (binary = 0, not empty)
 *   - Off-by-one in bitset size
 * ====================================================================
 */

#include <iostream>
#include <bitset>
#include <string>
#include <algorithm>

// --- Manual decimal to binary ---
std::string toBinaryManual(int n)
{
    if (n == 0) return "0";

    unsigned int num = static_cast<unsigned int>(n);
    std::string result;

    while (num > 0)
    {
        result.push_back((num & 1) ? '1' : '0');
        num >>= 1;
    }

    std::reverse(result.begin(), result.end());
    return result;
}

int main()
{
    int num{};
    std::cout << "Enter a decimal number: ";
    std::cin >> num;

    // --- Manual conversion ---
    std::string binManual = toBinaryManual(num);
    std::cout << "Binary (manual): " << binManual << '\n';

    // --- Using std::bitset ---
    std::bitset<32> bits(static_cast<unsigned int>(num));
    std::cout << "Binary (bitset):  " << bits << '\n';

    // --- Trim leading zeros from bitset ---
    std::string bitStr = bits.to_string();
    auto firstOne = bitStr.find('1');
    if (firstOne != std::string::npos)
    {
        std::cout << "Binary (trimmed): " << bitStr.substr(firstOne) << '\n';
    }
    else
    {
        std::cout << "Binary (trimmed): 0\n";
    }

    return 0;
}
