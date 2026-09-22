// Write a C++ function named `closestPrimeAfterN` that takes a positive integer `N` (as a `long long`) and returns the smallest prime number strictly greater than `N`. The function must use the provided Sieve of Eratosthenes approach adapted for a given upper bound, but must automatically determine a sufficient upper bound to find the next prime. The function should handle edge cases such as very small `N` (including `N = 1` or `N = 2`), and must be efficient for `N` up to \(10^9\). The function should return the prime as a `long long`.
#include <cassert>

int main() {
    assert(closestPrimeAfterN(1) == 2);
    assert(closestPrimeAfterN(2) == 3);
    assert(closestPrimeAfterN(3) == 5);
    assert(closestPrimeAfterN(4) == 5);
    assert(closestPrimeAfterN(10) == 11);
    assert(closestPrimeAfterN(13) == 17);
    assert(closestPrimeAfterN(100) == 101);
    assert(closestPrimeAfterN(113) == 127);
    assert(closestPrimeAfterN(1000) == 1009);
    assert(closestPrimeAfterN(1000000) == 1000003);
    return 0;
}
#include <vector>
#include <cstdint>
#include <cmath>

// Returns the smallest prime strictly greater than N.
// Uses a sieve that only stores odd numbers for efficiency.
long long closestPrimeAfterN(long long N) {
    if (N < 2) {
        return 2;
    }
    if (N == 2) {
        return 3;
    }

    // Bertrand's postulate: for N > 1, a prime exists between N and 2N.
    // We search up to 2*N; ensure we handle the case N=3 (2N=6) fine.
    const long long limit = 2 * N;
    // Number of odd numbers from 3 up to limit (inclusive) if limit is even,
    // otherwise up to limit-1. We only care about odd numbers >= 3.
    // Store odd numbers: index i represents odd number (2*i + 1).
    // We only need i starting from 1 (number 3).
    const size_t halfSize = static_cast<size_t>((limit + 1) / 2); // ceil(limit/2)
    std::vector<char> isPrime(halfSize, 1); // char instead of bool for speed

    // Mark 1 (index 0) as not prime (odd number 1 is not prime)
    isPrime[0] = 0;

    const long long root = static_cast<long long>(std::sqrt(limit));
    for (long long p = 3; p <= root; p += 2) {
        const size_t idx = static_cast<size_t>(p / 2); // because 2*i+1 = p => i = (p-1)/2
        if (isPrime[idx]) {
            // Mark odd multiples: p*p, p*p+2p, p*p+4p, ... until limit
            for (long long multiple = p * p; multiple <= limit; multiple += 2 * p) {
                isPrime[static_cast<size_t>(multiple / 2)] = 0;
            }
        }
    }

    // Start from the first odd number greater than N
    long long candidate = (N % 2 == 0) ? N + 1 : N + 2;
    while (candidate <= limit) {
        if (isPrime[static_cast<size_t>(candidate / 2)]) {
            return candidate;
        }
        candidate += 2;
    }

    // Should never reach here because of Bertrand's postulate.
    return -1;
}
// The core idea is to generate all primes up to some upper bound using a modified Sieve of Eratosthenes that only considers odd numbers (to save memory and time), then find the first prime greater than `N`. The key challenge is determining a safe upper bound. For any integer `N ≥ 1`, the next prime is guaranteed to exist somewhere. A classic theorem (Bertrand's postulate) states that for any `n > 1`, there is always a prime between `n` and `2n`. Therefore, we can set our search upper bound to `2 * N` (or `2 * N + 2` to be safe when `N` is small). If `N = 1`, the answer is `2`. If `N = 2`, the answer is `3`. For `N ≥ 3`, the sieve up to `2 * N` will contain all primes up to `2N`, and at least one prime greater than `N` will be present. In the sieve, we only store odd numbers: index `i` (starting from 0) corresponds to the odd number `2*i + 1`. We mark composites by iterating over odd multiples of each prime. After sieving, we iterate from the odd number just above `N` (if `N` is odd, start from `N+2`; if `N` is even, start from `N+1`) and check the sieve for unmarked values, returning the first found. Time complexity is \(O(M \log \log M)\) where \(M = 2N\), which is acceptable for `N` up to \(10^9\) in practice (though memory for the boolean vector is about 1GB since it uses `vector<bool>` which is bit-packed; we can optimize by using `vector<char>` to avoid bit overhead, but the problem expects a standalone solution). Space complexity is \(O(M)\).
