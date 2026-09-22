Write a C++ function `isPrimeSieve(long long n)` that determines whether a given non-negative integer `n` is prime using the Sieve of Eratosthenes. The function must build a sieve up to a maximum bound of at least `n` (use a fixed limit of `1e7 + 10` as in the snippet, but ensure the function works correctly for any `n` up to that bound). The function should return `true` if `n` is prime and `false` otherwise. Handle edge cases: 0 and 1 are not prime, and any `n` above the sieve limit should be handled by extending the sieve or returning `false` with a clear comment (for simplicity, assume input is within `[0, 1e7]`). The solution must be self-contained, using modern C++ (C++17 or later), with appropriate `const` correctness and comments.

#include <cassert>

int main() {
    // Basic primality tests
    assert(isPrimeSieve(0) == false);
    assert(isPrimeSieve(1) == false);
    assert(isPrimeSieve(2) == true);
    assert(isPrimeSieve(3) == true);
    assert(isPrimeSieve(4) == false);
    assert(isPrimeSieve(5) == true);
    assert(isPrimeSieve(9) == false);
    assert(isPrimeSieve(97) == true);
    assert(isPrimeSieve(100) == false);

    // Larger known primes and composites
    assert(isPrimeSieve(104729) == true);   // 10000th prime
    assert(isPrimeSieve(104730) == false);
    assert(isPrimeSieve(9999991) == true);  // a prime near 1e7
    assert(isPrimeSieve(10000000) == false); // 1e7 is not prime

    // Edge cases: 0, 1, and a negative number (out of spec, returns false)
    assert(isPrimeSieve(-1) == false);
    assert(isPrimeSieve(10000001) == false); // beyond maxLimit-1

    return 0;
}

#include <vector>
#include <cstdint>

// Determine if a non-negative integer n is prime using a precomputed sieve.
// Assumes n is within [0, maxLimit-1]; returns false for n >= maxLimit.
bool isPrimeSieve(long long n) {
    const long long maxLimit = 10000000LL + 10; // 1e7 + 10
    if (n < 0 || n >= maxLimit) {
        // Out of range: for this exercise, we treat as not prime (with comment)
        return false;
    }

    // Sieve of Eratosthenes: mark non-primes.
    // vector<bool> is space-efficient (bit-packed).
    std::vector<bool> isPrime(maxLimit, true);
    isPrime[0] = isPrime[1] = false;

    for (long long i = 2; i < maxLimit; ++i) {
        if (isPrime[i]) {
            // Mark multiples of i as composite, starting from i*i for optimization.
            for (long long j = i * i; j < maxLimit; j += i) {
                isPrime[j] = false;
            }
        }
    }

    return isPrime[n];
}

// The Sieve of Eratosthenes works by initially marking all numbers from 2 to `maxlimit-1` as prime, then iterating from 2 upwards. For each prime `i`, mark all multiples of `i` (starting from `i*i` for optimization, but the snippet uses `i*2`, which is also correct, just slightly less efficient) as composite. After the sieve is built, checking primality of `n` is an O(1) lookup in the boolean vector. Edge cases: 0 and 1 are explicitly set to `false`. For numbers less than 2, the function returns `false`. If `n` is outside the precomputed limit, the function should either extend the sieve dynamically (not done here for simplicity) or return `false` with a comment. Time complexity: building the sieve takes O(`maxlimit` log log `maxlimit`) due to the harmonic series of multiples. Space: O(`maxlimit`) for the boolean vector. For fixed `maxlimit` = 1e7+10, the memory is about 10 MB (as `vector<bool>` is bit-packed, much less). Queries are O(1) after sieve construction.
