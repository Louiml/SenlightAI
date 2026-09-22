Write a C++ function named `countPrimesUpTo` that takes a single positive integer `n` and returns the number of prime numbers in the inclusive range `[2, n]`. The function must use an efficient sieve-based algorithm (e.g., the Sieve of Eratosthenes) rather than trial division for each number. The input `n` can be as large as 10^7, and the function must handle the edge cases where `n` is 0 or 1 (both should return 0). The function should be const-correct and must not use global variables.

#include <cassert>

int main() {
    // Edge cases: 0 and 1 have no primes.
    assert(countPrimesUpTo(0) == 0);
    assert(countPrimesUpTo(1) == 0);
    // Small values.
    assert(countPrimesUpTo(2) == 1);   // {2}
    assert(countPrimesUpTo(3) == 2);   // {2,3}
    assert(countPrimesUpTo(10) == 4);  // {2,3,5,7}
    assert(countPrimesUpTo(11) == 5);  // {2,3,5,7,11}
    // Known values for a few moderate n.
    assert(countPrimesUpTo(100) == 25);
    assert(countPrimesUpTo(1000) == 168);
    assert(countPrimesUpTo(10000) == 1229);
    // Test a large value (e.g., 1e6) to ensure performance and correctness.
    assert(countPrimesUpTo(1000000) == 78498);
    return 0;
}

#include <vector>

// Return the number of primes in the inclusive range [2, n].
// Uses the Sieve of Eratosthenes. Efficient for n up to 10^7.
int countPrimesUpTo(int n) {
    if (n < 2) return 0;

    // Mark all numbers as potentially prime initially.
    std::vector<bool> isPrime(n + 1, true);
    // 0 and 1 are not prime.
    isPrime[0] = false;
    isPrime[1] = false;

    // Standard sieve: for each prime i, start from i*i and mark multiples.
    for (int i = 2; i <= n / i; ++i) {
        if (isPrime[i]) {
            // Mark all multiples of i starting from i*i as composite.
            for (int multiple = i * i; multiple <= n; multiple += i) {
                isPrime[multiple] = false;
            }
        }
    }

    // Count the primes.
    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) ++count;
    }
    return count;
}

// The core algorithm is the classic Sieve of Eratosthenes, implemented with a `std::vector<bool>` or a boolean array for marking composites. Initialize all entries from 2 to `n` as `true` (meaning "possibly prime"), and set index 0 and 1 to `false` because they are not prime. Then, for each `i` from 2 up to `sqrt(n)` (or until `i * i <= n` to avoid overflow), if `i` is still marked as prime, mark all multiples of `i` starting from `i * i` as `false`. This is correct because any composite number less than `i * i` has a smaller prime factor already processed. After sieving, count the number of `true` values in the range `[2, n]`. Edge cases: if `n < 2`, return 0 immediately to avoid unnecessary work. The time complexity is `O(n log log n)` for the sieving plus `O(n)` for the final count, which is effectively `O(n log log n)`. Space complexity is `O(n)` for the boolean vector. To prevent overflow when computing `i * i`, compare `i <= n / i` instead of `i * i <= n`.
