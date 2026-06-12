/*
 * ====================================================================
 * Program 4: Input / Output
 *
 * THEORY:
 *   std::cin   — Standard input (keyboard)
 *   std::cout  — Standard output (console)
 *   std::cerr  — Standard error (unbuffered)
 *   std::clog  — Standard log (buffered)
 *
 * FORMATTED I/O:
 *   std::setprecision(n)   — digits after decimal for floating
 *   std::fixed             — fixed-point notation
 *   std::scientific        — scientific notation
 *   std::boolalpha         — prints true/false instead of 1/0
 *   std::hex / std::dec / std::oct — integer base
 *   std::setw(n)           — field width
 *   std::left / std::right — alignment
 *   std::setfill(c)        — fill character
 *
 * INPUT HANDLING:
 *   std::getline(stream, string) — reads entire line (including spaces)
 *   cin.ignore(n, delim)         — ignores characters up to delimiter
 *   cin.clear()                  — clears error flags
 *   cin.fail()                   — checks if previous read failed
 *
 * SAMPLE INPUT/OUTPUT:
 *   Input:
 *     Enter name: Alice
 *     Enter age: 25
 *     Enter pi: 3.14159
 *   Output:
 *     Name: Alice, Age: 25
 *     Pi formatted: 3.142
 *     Hex 25: 19
 *     Octal 25: 31
 *
 * TIME COMPLEXITY:   O(1) per I/O operation (amortized)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: cin vs getline() — what's the difference?
 *   A: cin >> stops at whitespace; getline() reads until newline.
 *      Mixing them leaves '\n' in the buffer — use cin.ignore().
 *   Q: Why use cerr vs cout?
 *   A: cerr is unbuffered and not tied to cout — useful for errors
 *      that must appear immediately.
 *
 * COMMON MISTAKES:
 *   - Not clearing the input buffer after cin >> before getline().
 *   - Forgetting #include <iomanip> for manipulators.
 *   - Assuming cin >> reads an entire line.
 *   - Not checking cin.fail() after numeric input.
 * ====================================================================
 */

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

int main()
{
    // ---- Basic cout ----
    std::cout << "=== Basic cout ===\n";
    std::cout << "Hello, world!\n";

    // ---- cerr and clog ----
    std::cerr << "This is an error message (cerr)\n";
    std::clog << "This is a log message (clog)\n";

    // ---- cin with error handling ----
    std::cout << "\n=== cin with error checking ===\n";
    int age{};
    std::cout << "Enter an integer (age): ";
    std::cin >> age;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Setting age to 0.\n";
        age = 0;
    }

    // ---- Ignore leftover newline before getline ----
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // ---- getline ----
    std::cout << "Enter your full name: ";
    std::string name;
    std::getline(std::cin, name);

    // ---- Formatted output ----
    std::cout << "\n=== Formatted output ===\n";
    double pi = 3.14159265358979;

    std::cout << "Default:          " << pi << '\n';
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Fixed (3 digits): " << pi << '\n';
    std::cout << std::scientific;
    std::cout << "Scientific:       " << pi << '\n';

    // ---- Integer formatting ----
    int num = 255;
    std::cout << "\n=== Integer formatting ===\n";
    std::cout << "Decimal:  " << num << '\n';
    std::cout << std::hex << "Hex:      " << num << '\n';
    std::cout << std::oct << "Octal:    " << num << '\n';
    std::cout << std::dec;

    // ---- Boolean ----
    std::cout << "\n=== Boolean formatting ===\n";
    bool flag = true;
    std::cout << std::boolalpha << "boolalpha: " << flag << '\n';
    std::cout << std::noboolalpha << "noboolalpha: " << flag << '\n';

    // ---- Width, fill, alignment ----
    std::cout << "\n=== Width & align ===\n";
    std::cout << std::setw(10) << std::left << "Left" << "|\n";
    std::cout << std::setw(10) << std::right << "Right" << "|\n";
    std::cout << std::setfill('-') << std::setw(10) << "fill"
              << std::setfill(' ') << "|\n";

    // ---- Summary ----
    std::cout << "\n=== Summary ===\n";
    std::cout << "Name: " << name << ", Age: " << age << '\n';

    return 0;
}
