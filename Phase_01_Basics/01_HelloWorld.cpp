/*
 * ====================================================================
 * Program 1: Hello World — The First C++ Program
 *
 * THEORY:
 * - A C++ program starts executing from the `main()` function.
 * - `#include <iostream>` brings in the standard input/output library.
 * - `int main()` is the entry point. It returns 0 to indicate success.
 * - `std::cout` (character output) prints text to the console.
 * - `<<` is the stream insertion operator.
 * - `\n` is a newline character; `std::endl` flushes the buffer.
 *
 * COMPILATION PROCESS (4 stages):
 *   1. Preprocessing: Handles #include, #define, macros → .i file
 *   2. Compilation:  Translates C++ to assembly → .s file
 *   3. Assembly:     Converts assembly to machine code → .o file
 *   4. Linking:      Links object files + libraries → executable
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -o hello 01_HelloWorld.cpp
 * Run:     ./hello
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output:
 *     Hello, World!
 *     Welcome to C++20 Programming!
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: What does `#include <iostream>` do?
 *   A: It inserts the contents of the iostream header file, which
 *      declares std::cout, std::cin, std::cerr, etc.
 *   Q: What is the difference between std::endl and '\n'?
 *   A: '\n' just inserts a newline; std::endl also flushes the output
 *      buffer, which is slightly slower.
 *   Q: What does `int main()` return?
 *   A: An exit status. 0 means success, nonzero indicates an error.
 *
 * COMMON MISTAKES:
 *   - Forgetting `#include <iostream>` — compiler error.
 *   - Writing `main` instead of `int main` — invalid.
 *   - Missing semicolon at the end of the cout statement.
 *   - Using `cout` without `std::` prefix (unless `using namespace std;`).
 *   - Forgetting the return statement (allowed in C++20 but bad practice).
 * ====================================================================
 */

#include <iostream>

int main()
{
    std::cout << "Hello, World!" << '\n';
    std::cout << "Welcome to C++20 Programming!" << std::endl;

    return 0;
}
