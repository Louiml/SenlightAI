Write a C++ function `int nthPrime(int n)` that returns the n-th prime number, with 1-based indexing (i.e., `nthPrime(1)` returns 2, `nthPrime(2)` returns 3, `nthPrime(3)` returns 5, etc.). The function must handle any `n` from 1 to 10000 inclusive. To solve it efficiently, precompute all primes up to a sufficient upper bound (e.g., use the Sieve of Eratosthenes up to at least 110000, since the 10000th prime is 104729). The function must be deterministic and should not rely on global mutable state between calls—either precompute once using a static local variable or pass a precomputed vector as a helper. Ensure correctness for edge cases like `n=1` and large `n` near the upper bound.

#include <cassert>
#include <stdexcept>

int main() {
    // Basic first few primes
    assert(nthPrime(1) == 2);
    assert(nthPrime(2) == 3);
    assert(nthPrime(3) == 5);
    assert(nthPrime(4) == 7);
    assert(nthPrime(5) == 11);

    // Middle values
    assert(nthPrime(10) == 29);
    assert(nthPrime(100) == 541);
    assert(nthPrime(1000) == 7919);

    // Upper bound
    assert(nthPrime(10000) == 104729);

    // Edge case: n = 1 again, and verify multiple calls work
    assert(nthPrime(1) == 2);
    assert(nthPrime(2) == 3);

    // Invalid input should throw
    bool threw = false;
    try {
        nthPrime(0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        nthPrime(10001);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}

#include <vector>
#include <stdexcept>

// Returns the n-th prime number (1-based index). Precomputes primes up to 110000 once.
int nthPrime(int n) {
    if (n <= 0) {
        throw std::invalid_argument("n must be positive");
    }
    const int LIMIT = 110000; // Safe upper bound for 10000th prime (104729)
    static const std::vector<int> primes = []() {
        std::vector<bool> isPrime(LIMIT + 1, true);
        isPrime[0] = isPrime[1] = false;
        for (int i = 2; i * i <= LIMIT; ++i) {
            if (isPrime[i]) {
                for (int j = i * i; j <= LIMIT; j += i) {
                    isPrime[j] = false;
                }
            }
        }
        std::vector<int> result;
        result.reserve(10000);
        for (int i = 2; i <= LIMIT; ++i) {
            if (isPrime[i]) {
                result.push_back(i);
            }
        }
        return result;
    }();
    if (n > static_cast<int>(primes.size())) {
        throw std::out_of_range("n exceeds precomputed prime count");
    }
    return primes[n - 1];
}

// The most straightforward approach is to use the Sieve of Eratosthenes to generate all primes up to a fixed limit. Since the 10000th prime is 104729, choosing a limit of 110000 (or 200000 for safety) guarantees coverage. The sieve works by first initializing a boolean array where all odd numbers are marked as prime, then iterating from 3 up to sqrt(limit) and marking multiples of each prime as composite. This yields O(L log log L) time for the sieve, where L is the limit, and O(L) space. After precomputing, we iterate through the array and collect primes in order; the n-th one is returned. Edge cases: `n=1` returns 2; if `n` exceeds the number of primes found (shouldn’t for n≤10000), we could throw or return -1. Time complexity per call is O(1) after precomputation (just an array lookup), and space complexity is O(L) for the sieve array. The precomputation only happens once, so subsequent calls are very fast.
