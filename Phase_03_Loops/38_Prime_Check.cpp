/*
 * ====================================================================
 * Program 38: Prime Check
 * Theory:
 *   A prime number is a natural number > 1 with exactly two factors:
 *   1 and itself.
 *
 *   sqrt optimization: If n has a factor > sqrt(n), the complementary
 *   factor must be < sqrt(n). So we only need to check up to sqrt(n).
 *
 *   Sieve of Eratosthenes:
 *     Efficiently find all primes up to N.
 *     - Create boolean array of size N+1, mark all as true
 *     - Start from 2, mark multiples as false
 *     - Remaining true positions are primes
 *
 * Sample Input/Output:
 *   Enter a number: 29
 *   29 is prime.
 *
 *   Enter a number: 1
 *   1 is not prime.
 *
 *   Enter limit for sieve: 30
 *   Primes up to 30: 2 3 5 7 11 13 17 19 23 29
 *
 * Time Complexity:
 *   IsPrime: O(√n)
 *   Sieve:   O(n log log n)
 * Space Complexity:
 *   IsPrime: O(1)
 *   Sieve:   O(n)
 *
 * Common Interview Questions:
 *   Q: Why check only up to sqrt(n)?
 *   A: If n = a×b and a > sqrt(n), then b < sqrt(n). So a factor
 *      exists ≤ sqrt(n) if any factor exists.
 *
 *   Q: What is the Sieve of Eratosthenes complexity?
 *   A: O(n log log n) time, O(n) space.
 *
 * Common Mistakes:
 *   - Treating 1 as prime (it's not)
 *   - Not handling even numbers efficiently
 *   - Checking up to n instead of sqrt(n)
 * ====================================================================
 */

#include <iostream>
#include <cmath>
#include <vector>

// --- IsPrime with sqrt optimization ---
bool isPrime(int n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    int limit = static_cast<int>(std::sqrt(n));
    for (int i{3}; i <= limit; i += 2)
    {
        if (n % i == 0) return false;
    }
    return true;
}

// --- Sieve of Eratosthenes ---
std::vector<int> sieve(int limit)
{
    std::vector<bool> isPrime(limit + 1, true);
    std::vector<int> primes;

    if (limit < 2) return primes;

    isPrime[0] = isPrime[1] = false;

    for (int i{2}; i * i <= limit; ++i)
    {
        if (isPrime[i])
        {
            for (int j{i * i}; j <= limit; j += i)
            {
                isPrime[j] = false;
            }
        }
    }

    for (int i{2}; i <= limit; ++i)
    {
        if (isPrime[i]) primes.push_back(i);
    }
    return primes;
}

int main()
{
    int n{};
    std::cout << "Enter a number: ";
    std::cin >> n;

    if (isPrime(n))
        std::cout << n << " is prime.\n";
    else
        std::cout << n << " is not prime.\n";

    // --- Sieve demo ---
    int limit{};
    std::cout << "Enter limit for sieve: ";
    std::cin >> limit;

    auto primes = sieve(limit);
    std::cout << "Primes up to " << limit << ": ";
    for (int p : primes) std::cout << p << ' ';
    std::cout << '\n';

    return 0;
}
