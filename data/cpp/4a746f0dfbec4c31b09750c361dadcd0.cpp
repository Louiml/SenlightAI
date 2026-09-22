Write a C++ function named `nthPrime` that takes a positive integer `n` and returns the `n`-th prime number, where the first prime is 2, the second is 3, and so on. The function must be standalone (no global variables, no external dependencies beyond the standard library), should handle at least `n` up to 1000 (and ideally much larger), and must be efficient enough to avoid naïve repeated primality checks that are too slow. The function should return `unsigned long long` to accommodate large primes. You must also ensure correctness for edge cases like `n = 1` and large `n` where primes exceed typical `int` range.
// The solution benefits from a sieve-based approach to generate primes up to a known bound, then index into the result. However, because the input `n` may be large and unknown, we need a robust strategy. A simple approach: use the Sieve of Eratosthenes to generate primes up to an estimated upper bound based on the prime number theorem, which says the `n`-th prime is approximately `n * log(n)`. A safe bound is `n * (log(n) + log(log(n)))` for `n >= 6`, with a small safety margin. Alternatively, for simplicity and given typical constraints (n ≤ 1000 or even 10^6), we can dynamically extend the sieve: start with a bound, and if the sieve yields fewer than `n` primes, double the bound and rebuild. The sieve itself runs in `O(N log log N)` time and `O(N)` space for `N` being the upper bound, which is efficient for n up to millions. The edge case `n = 1` must return 2, and we should handle small `n` with a default bound (e.g., 15). The function should use `std::vector<bool>` for memory efficiency. The main algorithm: (1) if `n < 1`, return 0 (invalid input, but spec says positive, so we assume valid). (2) Set `limit = 15` if `n < 6`, else `n * (log(n) + log(log(n))) + 10`. (3) While the number of primes found is less than `n`, double `limit` and rebuild sieve. (4) Sieve: create a boolean vector `isPrime` of size `limit+1`, initialize all to true, mark 0 and 1 false, then for `i` from 2 to sqrt(limit), if `isPrime[i]`, mark multiples false. (5) Iterate from 2 to `limit`, collect primes into a vector as we go, stopping once we have `n` primes. Return the last collected prime. Time complexity: O(L log log L) per sieve build, with L being the final limit. Space: O(L). This is more than acceptable.
#include <vector>
#include <cmath>

// Return the n-th prime number, where the 1st prime is 2.
unsigned long long nthPrime(unsigned long long n) {
    if (n == 0) return 0; // invalid input per spec; handle gracefully

    // Estimate an upper bound for the n-th prime.
    unsigned long long limit;
    if (n < 6) {
        limit = 15; // enough for first 6 primes
    } else {
        double n_d = static_cast<double>(n);
        double log_n = std::log(n_d);
        limit = static_cast<unsigned long long>(n_d * (log_n + std::log(log_n))) + 10;
    }

    while (true) {
        // Sieve of Eratosthenes up to limit.
        std::vector<bool> isPrime(limit + 1, true);
        isPrime[0] = isPrime[1] = false;

        unsigned long long sqrtLimit = static_cast<unsigned long long>(std::sqrt(static_cast<double>(limit)));
        for (unsigned long long i = 2; i <= sqrtLimit; ++i) {
            if (isPrime[i]) {
                for (unsigned long long j = i * i; j <= limit; j += i) {
                    isPrime[j] = false;
                }
            }
        }

        // Count primes and find the n-th one.
        unsigned long long count = 0;
        for (unsigned long long i = 2; i <= limit; ++i) {
            if (isPrime[i]) {
                ++count;
                if (count == n) {
                    return i;
                }
            }
        }

        // Not enough primes found; double the limit and retry.
        limit *= 2;
    }
}
#include <cassert>

int main() {
    assert(nthPrime(1) == 2);
    assert(nthPrime(2) == 3);
    assert(nthPrime(3) == 5);
    assert(nthPrime(10) == 29);
    assert(nthPrime(100) == 541);
    assert(nthPrime(1000) == 7919);
    assert(nthPrime(5000) == 48611);
    assert(nthPrime(10000) == 104729);
    assert(nthPrime(100000) == 1299709);
}
