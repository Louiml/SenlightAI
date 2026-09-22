// Write a C++ function `int countDistinctPrimeFactors(int n)` that returns the number of distinct prime factors of a positive integer `n`. For example, `n = 12` has prime factors 2 and 3, so the function returns 2. `n = 1` has no prime factors, so return 0. The function must be efficient enough to handle up to `n = 10^7` with multiple calls (up to 10^4 queries), so precompute primes up to 10^7 using a sieve of Eratosthenes. The function should not recompute primes for each call—use a static precomputed prime list built once on first use. The input will always be positive (≥1), but handle edge cases like `n = 1` and `n` itself being prime. The memory limit is reasonable, so arrays of size 10^7 are acceptable (use `bool` for composite flag and `vector<int>` for primes). The time limit suggests that the sieve should take O(N log log N) preprocessing and each query should be O(number of distinct primes ≤ sqrt(n)) or faster.
int main() {
    assert(countDistinctPrimeFactors(1) == 0);
    assert(countDistinctPrimeFactors(2) == 1);
    assert(countDistinctPrimeFactors(12) == 2); // 2 and 3
    assert(countDistinctPrimeFactors(30) == 3); // 2,3,5
    assert(countDistinctPrimeFactors(97) == 1); // prime
    assert(countDistinctPrimeFactors(100) == 2); // 2 and 5
    assert(countDistinctPrimeFactors(360) == 3); // 2,3,5
    assert(countDistinctPrimeFactors(1024) == 1); // only 2
    assert(countDistinctPrimeFactors(999983) == 1); // prime (large)
    assert(countDistinctPrimeFactors(10000000) == 2); // 2 and 5
}
#include <vector>
#include <cmath>

// Precompute primes up to a given limit using Sieve of Eratosthenes.
// Returns a vector containing all primes <= limit.
std::vector<int> sievePrimes(int limit) {
    std::vector<bool> isComposite(limit + 1, false);
    for (int i = 2; i * i <= limit; ++i) {
        if (!isComposite[i]) {
            for (int j = i * i; j <= limit; j += i) {
                isComposite[j] = true;
            }
        }
    }
    std::vector<int> primes;
    for (int i = 2; i <= limit; ++i) {
        if (!isComposite[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

// Count distinct prime factors of n.
// Uses static precomputed primes up to a fixed maximum (10^7).
int countDistinctPrimeFactors(int n) {
    static const int MAX_N = 10000000;
    static const std::vector<int> primes = sievePrimes(MAX_N);
    int count = 0;
    int remaining = n;
    for (int p : primes) {
        if (p * p > remaining) break;
        if (remaining % p == 0) {
            ++count;
            while (remaining % p == 0) {
                remaining /= p;
            }
        }
    }
    if (remaining > 1) {
        ++count; // remaining is a prime factor larger than sqrt(original n)
    }
    return count;
}
// The primary challenge is to precompute all primes up to 10^7 efficiently and then, for each query `n`, count distinct primes that divide `n`. We can use a standard sieve of Eratosthenes to mark composite numbers in a `bool` array of size `max_n + 1`. After sieving, we collect primes into a `vector<int>`. For each query, we iterate over primes `p` from the smallest upward while `p * p <= n`, and if `p` divides `n`, we increment the count and then divide `n` by `p` repeatedly to remove all occurrences of that prime (this ensures we count distinct primes only). After the loop, if the remaining `n > 1`, it means `n` itself is a prime factor (the original `n` may have had a large prime factor > sqrt(original n)), so we increment count once more. For `n = 1`, the loop does nothing and the remaining check fails, returning 0. Edge cases: `n` being prime: the loop finds no divisors up to sqrt(n), then `n` remains >1, so count = 1. `n = 2`: loop doesn't run (2*2 > 2), then leftover n=2, count=1. Time complexity: Sieve is O(N log log N) for N=10^7, which is about 10^7 * log(log(10^7)) ~ 10^7 * 3, acceptable. Each query: worst case iterates over primes up to sqrt(10^7) ≈ 3162, but only those that divide, so typical much faster. Space: O(N) for composite flag (use `vector<bool>` to save memory) and O(π(N)) for primes, about 664k primes. Correctness is assured by the fundamental theorem of arithmetic and the sieve.
