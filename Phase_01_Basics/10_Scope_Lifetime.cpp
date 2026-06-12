/*
 * ====================================================================
 * Program 10: Scope & Lifetime
 *
 * THEORY:
 * SCOPE (where a name is visible):
 *   Global scope     — outside any function/namespace/class; visible
 *                      everywhere in the translation unit.
 *   Namespace scope  — inside a namespace.
 *   Local/block scope— inside { } braces.
 *   Class scope      — inside a class.
 *
 * LIFETIME (when an object exists):
 *   Static storage duration — exists for the entire program run.
 *     - Global variables, static local variables,
 *       static class members, constinit variables.
 *     - Initialized before main(), destroyed after main().
 *   Thread-local storage duration (thread_local) —
 *     - Each thread gets its own copy.
 *   Automatic storage duration — local variables (stack).
 *     - Created at point of definition, destroyed when scope exits.
 *   Dynamic storage duration — heap (new/delete).
 *     - Explicitly controlled by the programmer.
 *
 * SAMPLE INPUT/OUTPUT:
 *   (no input)
 *   Output:
 *     Global: 100
 *     Static count: 1
 *     Static count: 2
 *     Static count: 3
 *     Local: 42
 *     Dynamic: 99
 *
 * TIME COMPLEXITY:   O(1)
 * SPACE COMPLEXITY:  O(1)
 *
 * COMMON INTERVIEW QUESTIONS:
 *   Q: What is the difference between scope and lifetime?
 *   A: Scope is where a name is visible; lifetime is when an object
 *      exists. A variable may exist but be out of scope (e.g., via
 *      pointers/references).
 *   Q: What is static local variable?
 *   A: A local variable with static storage. It's initialized once
 *      and retains its value across function calls.
 *   Q: What is thread_local?
 *   A: Each thread has its own instance of the variable. Combined
 *      with static → one instance per thread.
 *
 * COMMON MISTAKES:
 *   - Returning a pointer/reference to a local (automatic) variable.
 *   - Forgetting to delete dynamically allocated memory (leak).
 *   - Using a dangling pointer after the object is destroyed.
 *   - Assuming static local variables are initialized every call.
 * ====================================================================
 */

#include <iostream>

// ---- Global variable (static storage duration) ----
int global_var = 100;

// ---- Namespace scope ----
namespace MyScope
{
    int ns_var = 200;
}

void counter_function()
{
    // ---- Static local variable ----
    // Initialized once; value persists across calls.
    static int call_count = 0;
    ++call_count;
    std::cout << "static call_count: " << call_count
              << " (address: " << &call_count << ")\n";

    // ---- Automatic local variable ----
    int local = call_count * 10;
    std::cout << "automatic local:   " << local
              << " (address: " << &local << ")\n";
}

int* dangling_generator()
{
    int local = 42;
    return &local;  // BAD: local destroyed when function returns
}

int main()
{
    std::cout << "=== Global scope ===\n";
    std::cout << "global_var: " << global_var << '\n';

    std::cout << "\n=== Namespace scope ===\n";
    std::cout << "MyScope::ns_var: " << MyScope::ns_var << '\n';

    // ---- Block scope ----
    std::cout << "\n=== Block scope ===\n";
    {
        int block_var = 50;
        std::cout << "Inside block: block_var = " << block_var << '\n';
        // block_var dies here
    }
    // std::cout << block_var;  // Error: out of scope

    // ---- Static local persistence ----
    std::cout << "\n=== Static local vs automatic ===\n";
    counter_function();  // count = 1
    counter_function();  // count = 2
    counter_function();  // count = 3

    // ---- Dynamic allocation ----
    std::cout << "\n=== Dynamic storage ===\n";
    int* dyn = new int(99);
    std::cout << "Dynamic value: " << *dyn << '\n';
    delete dyn;  // must release

    // ---- Dangling pointer (BAD) ----
    std::cout << "\n=== Dangling pointer (common mistake) ===\n";
    int* dangling = dangling_generator();
    // *dangling; // UB — local was destroyed
    std::cout << "Dangling pointer address: " << dangling
              << " (DO NOT DEREFERENCE)\n";

    // ---- thread_local example (C++11) ----
    // (Conceptual only — full demo requires <thread>)
    std::cout << "\n=== thread_local (C++11) ===\n";
    std::cout << "Declared as: thread_local int tls = 0;\n";
    std::cout << "Each thread has its own copy.\n";

    // ---- Summary of storage durations ----
    std::cout << "\n=== Summary ===\n";
    std::cout << "1. Static:    Lives entire program (globals, statics)\n";
    std::cout << "2. Automatic: Lives until scope exit (locals)\n";
    std::cout << "3. Dynamic:   Lives until delete (new/delete)\n";
    std::cout << "4. Thread:    Lives for thread duration (thread_local)\n";

    return 0;
}
