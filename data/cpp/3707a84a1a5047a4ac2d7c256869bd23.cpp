Write a C++ function named `findCommitmentPrime` that simulates the core minting logic from the Zerocoin `PrivateCoin::mintCoin` method, but in a simplified standalone form. The function should take three parameters: a vector of 64-bit unsigned integers representing candidate commitments (already computed as `g^s * h^r mod p`), a 64-bit integer `minValue`, and a 64-bit integer `maxValue`. It should return the index of the first candidate in the vector that is prime, greater than `minValue`, and less than `maxValue`. If no such candidate exists, return `-1`. Assume the input vector is non-empty. Your function must implement its own primality test (do not use external libraries like GMP); use a deterministic Miller–Rabin test for 64-bit integers with bases `{2, 3, 5, 7, 11, 13, 17}` plus a trial division by small primes. The bounds `minValue` and `maxValue` are inclusive in the sense that a valid value must satisfy `candidate > minValue && candidate < maxValue`. Apply `const` correctness appropriately.

// The solution requires two main components: a robust primality test and a linear scan over the candidate vector. The primality test must handle 64-bit unsigned integers (up to ~1.8e19), which exceeds the range of `long long` (signed), so we use `uint64_t`. A deterministic Miller–Rabin test with bases `{2, 3, 5, 7, 11, 13, 17}` is sufficient for all 64-bit integers. The test first handles small cases: if `n < 2`, return false; if `n` equals any of the bases, return true; if `n` is even, return false. Then we factor out powers of 2 from `n-1` to write `n-1 = d * 2^s` where `d` is odd. For each base `a`, compute `x = a^d mod n` using modular exponentiation with 128-bit intermediates to avoid overflow. If `x` is 1 or `n-1`, continue to next base; otherwise repeatedly square `x` up to `s-1` times, and if it never becomes `n-1`, return false. After all bases pass, return true. Additionally, we could add trial division by small primes first for efficiency, but the Miller–Rabin alone is acceptable. The main function iterates through the vector from index 0 upward, checking each candidate with the primality test and the range conditions. The first match returns its index; if none match, return -1. Time complexity is O(k * log n) per candidate where k is the number of Miller–Rabin bases (7) and log n is the bit length (up to 64), plus the O(m) scan for m candidates, so overall O(m * 7 * 64) ≈ O(m). Space complexity is O(1) as we use only a few local variables.

#include <cstdint>
#include <vector>

// Modular multiplication for uint64_t that avoids overflow using 128-bit intermediates.
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (static_cast<__uint128_t>(a) * b) % mod;
}

// Modular exponentiation: (base^exp) % mod for uint64_t values.
static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = mul_mod(result, base, mod);
        }
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// Deterministic Miller–Rabin primality test for 64-bit unsigned integers.
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    // Check small primes directly.
    const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17};
    for (uint64_t p : small_primes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    // Write n-1 = d * 2^s with d odd.
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    // Test each base.
    for (uint64_t a : small_primes) {
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool witness = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                witness = false;
                break;
            }
        }
        if (witness) return false; // Composite
    }
    return true;
}

// Find the index of the first candidate in `candidates` that is prime and lies
// strictly between minValue (exclusive) and maxValue (exclusive).
// Returns -1 if no such candidate exists.
int findCommitmentPrime(const std::vector<uint64_t>& candidates,
                        uint64_t minValue,
                        uint64_t maxValue) {
    for (size_t i = 0; i < candidates.size(); ++i) {
        uint64_t value = candidates[i];
        if (value > minValue && value < maxValue && is_prime(value)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

#include <cassert>
#include <cstdint>
#include <vector>

// (The solution function and helpers are assumed to be included above.)

int main() {
    // No candidate meets the range or primality requirements.
    std::vector<uint64_t> candidates1 = {4, 6, 8, 9};
    assert(findCommitmentPrime(candidates1, 2, 10) == -1);

    // First valid prime is at index 2.
    std::vector<uint64_t> candidates2 = {4, 6, 7, 10};
    assert(findCommitmentPrime(candidates2, 5, 11) == 2);

    // Edge case: candidate equal to minValue or maxValue is excluded.
    std::vector<uint64_t> candidates3 = {5, 7, 11};
    assert(findCommitmentPrime(candidates3, 5, 12) == 1); // 7 only

    // Large prime near uint64_t max.
    std::vector<uint64_t> candidates4 = {18446744073709551557ULL}; // largest prime < 2^64
    assert(findCommitmentPrime(candidates4, 0, UINT64_MAX) == 0);

    // Test that composite large number is rejected.
    std::vector<uint64_t> candidates5 = {18446744073709551615ULL}; // 2^64 - 1 (composite)
    assert(findCommitmentPrime(candidates5, 0, UINT64_MAX) == -1);

    // Multiple candidates: prime appears later.
    std::vector<uint64_t> candidates6 = {10, 12, 13, 14};
    assert(findCommitmentPrime(candidates6, 0, 100) == 2);

    // Single candidate that is prime and in range.
    std::vector<uint64_t> candidates7 = {3};
    assert(findCommitmentPrime(candidates7, 2, 4) == 0);

    // Candidate equal to minValue or maxValue boundary.
    std::vector<uint64_t> candidates8 = {2, 4, 6};
    assert(findCommitmentPrime(candidates8, 2, 5) == -1); // 2 and 4 not > 2 and < 5

    // Empty candidate vector returns -1 (though problem says non-empty, test anyway).
    std::vector<uint64_t> candidates9 = {};
    assert(findCommitmentPrime(candidates9, 0, 100) == -1);
}
