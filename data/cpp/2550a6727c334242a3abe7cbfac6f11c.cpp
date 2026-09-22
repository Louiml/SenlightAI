/*
Write a C++ function `isPrimeSieve(int n, const std::array<bool, 10000>& primes)` that, given a precomputed sieve table of primality for numbers 0–9999 and an integer `n` (guaranteed to satisfy `0 <= n < 10000`), returns `true` if `n` is prime and `false` otherwise. The sieve must have been built using the Sieve of Eratosthenes as in the reference snippet, but your function must not perform any sieve computation itself—it only queries the table. Ensure the function correctly handles the edge cases `0` and `1` (both non‑prime) and that it uses `const` references appropriately.
*/

#include <array>
#include <cassert>

// Returns true if n is prime, based on the precomputed sieve.
// Precondition: 0 <= n < primes.size()
bool isPrimeSieve(int n, const std::array<bool, 10000>& primes) {
    assert(n >= 0 && n < static_cast<int>(primes.size()));
    return primes[n];
}

#include <array>
#include <cassert>
#include <iostream>

// Forward declaration (same as solution)
bool isPrimeSieve(int n, const std::array<bool, 10000>& primes);

int main() {
    // Build the sieve exactly as in the reference snippet
    constexpr auto LEN = 10000;
    std::array<bool, LEN> primes;
    primes.fill(true);
    primes[0] = primes[1] = false;

    for (int i = 2; i * i < LEN; ++i) {
        if (!primes[i])
            continue;
        for (int j = i * i; j < LEN; j += i)
            primes[j] = false;
    }

    // Test edge cases
    assert(isPrimeSieve(0, primes) == false);
    assert(isPrimeSieve(1, primes) == false);

    // Test small known primes
    assert(isPrimeSieve(2, primes) == true);
    assert(isPrimeSieve(3, primes) == true);
    assert(isPrimeSieve(5, primes) == true);
    assert(isPrimeSieve(7, primes) == true);

    // Test composite numbers
    assert(isPrimeSieve(4, primes) == false);
    assert(isPrimeSieve(9, primes) == false);
    assert(isPrimeSieve(100, primes) == false);

    // Test large prime near limit
    assert(isPrimeSieve(9973, primes) == true);
    assert(isPrimeSieve(9999, primes) == false);

    // Test consecutive numbers around a prime
    assert(isPrimeSieve(1009, primes) == true);
    assert(isPrimeSieve(1010, primes) == false);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The solution centers on the Sieve of Eratosthenes, which marks composite numbers in a boolean array. The sieve is initialized with all entries `true`, then `primes[0]` and `primes[1]` are set to `false`. For each integer `i` starting at 2, if `i` is still marked as prime, all multiples of `i` starting from `i*i` are marked as composite. The loop runs only while `i*i < 10000` because any composite number `c` has a prime factor ≤ √c; thus after processing up to √(9999) ≈ 99.995, all composites are already marked. The function itself simply returns `primes[n]` after an assertion that `0 <= n < 10000`. Edge cases: `0` and `1` are handled by initialization; duplicates or large `n` are guarded by the assertion. The precomputation runs in O(N log log N) time (where N=10000) and uses O(N) space; the query function runs in O(1) time and O(1) space.
