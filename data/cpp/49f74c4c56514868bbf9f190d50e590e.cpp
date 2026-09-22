// Write a C++ function `int nthPrimeAtIndex(int n)` that, given a non-negative integer `n`, returns the n-th prime number where the indexing starts at 0 (i.e., `n=0` returns 2, `n=1` returns 3, `n=2` returns 5, and so on). The function must generate primes dynamically up to a sufficient bound without using a fixed upper limit, and it should handle large `n` (e.g., up to 10,000) efficiently. It should also validate that `n` is non-negative, and for invalid input (negative `n`) it should return -1.
#include <cassert>

int nthPrimeAtIndex(int); // declaration from solution

int main() {
    // Basic known values
    assert(nthPrimeAtIndex(0) == 2);
    assert(nthPrimeAtIndex(1) == 3);
    assert(nthPrimeAtIndex(2) == 5);
    assert(nthPrimeAtIndex(3) == 7);
    assert(nthPrimeAtIndex(4) == 11);
    assert(nthPrimeAtIndex(10) == 29);
    assert(nthPrimeAtIndex(25) == 97);
    // Invalid input
    assert(nthPrimeAtIndex(-5) == -1);
    // Large index (0-indexed, 10000-th prime is 104729)
    assert(nthPrimeAtIndex(10000) == 104729);
    // Edge case: index 9999 gives 104723 (prime before 104729)
    assert(nthPrimeAtIndex(9999) == 104723);
    return 0;
}
#include <vector>
#include <cstddef>

// Returns the n-th prime (0-indexed) or -1 if n is negative.
int nthPrimeAtIndex(int n) {
    if (n < 0) {
        return -1;
    }

    std::vector<int> primes;
    size_t limit = 100; // initial sieve size

    while (true) {
        // Sieve of Eratosthenes up to 'limit'
        std::vector<bool> isComposite(limit + 1, false);
        isComposite[0] = true;
        isComposite[1] = true;

        primes.clear();
        for (size_t i = 2; i <= limit; ++i) {
            if (!isComposite[i]) {
                primes.push_back(static_cast<int>(i));
                // Mark multiples starting from i*i
                if (i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i) {
                        isComposite[j] = true;
                    }
                }
            }
        }

        if (static_cast<size_t>(n) < primes.size()) {
            return primes[n];
        }

        // Not enough primes, double the sieve size and retry
        limit = (limit == 0) ? 100 : limit * 2;
    }
}
// The solution uses a growing sieve approach. Start with an initial sieve size (e.g., 100) and a boolean vector `isComposite` marking 0 and 1 as composite (or use a `vector<bool>` with false meaning prime, but handle 0 and 1 specially). Iterate through numbers from 2 upward. For each prime found, mark all multiples as composite. Store primes in a vector. If the prime count exceeds `n`, return `primes[n]`. If we reach the end of the sieve without finding enough primes, double the sieve size and re-sieve from scratch (or extend the existing sieve). Since the sieve is reset when expanding, the time complexity for generating primes up to `m` is O(m log log m) per sieve pass, but because we may rebuild multiple times, the overall complexity is O(m log log m) for the final sieve size, plus overhead. For `n=10000`, the 10,000-th prime (0-indexed) is 104,729, so the largest sieve needed is about 105,000. Memory is O(m) for the sieve. Edge cases: `n=0` returns 2; negative `n` returns -1; extremely large `n` can be handled by repeated doubling until enough primes are found. The function must be `const`-correct and avoid global state.
