Write a C++ function `vector<bool> primeRange(int m, int n)` that returns a `std::vector<bool>` of length `n+1` where index `i` is `true` if `i` is prime and `false` otherwise, for all `i` in the range `[0, n]`. The function must correctly handle cases where `n < 2` (returning a vector where indices 0 and 1 are `false` and all others are `false`), and must compute primes up to `n` using the Sieve of Eratosthenes. The caller will use this function to print all primes from `m` to `n` inclusive, but the function itself should not perform any I/O. Ensure the function works efficiently for `n` up to 1,000,000, and correctly marks 0 and 1 as non-prime. The function signature must be `std::vector<bool> primeRange(int m, int n)` (note: `m` is provided for clarity but does not affect the sieve computation; only `n` matters).

#include <cassert>
#include <vector>

// The solution function is defined above (or included via header).

int main() {
    // Test small n
    std::vector<bool> primes0 = primeRange(0, 0);
    assert(primes0.size() == 1);
    assert(!primes0[0]);

    std::vector<bool> primes1 = primeRange(0, 1);
    assert(primes1.size() == 2);
    assert(!primes1[0]);
    assert(!primes1[1]);

    std::vector<bool> primes10 = primeRange(0, 10);
    // Expected primes up to 10: 2,3,5,7
    assert(primes10.size() == 11);
    assert(!primes10[0] && !primes10[1]);
    assert(primes10[2] && primes10[3] && !primes10[4]);
    assert(primes10[5] && !primes10[6] && primes10[7]);
    assert(!primes10[8] && !primes10[9] && !primes10[10]);

    // Test that m is ignored (function returns full sieve up to n)
    std::vector<bool> primesSame = primeRange(5, 10);
    assert(primesSame.size() == 11);
    for (int i = 0; i <= 10; ++i) {
        assert(primesSame[i] == primes10[i]);
    }

    // Test larger n (e.g., 100) with known primes
    std::vector<bool> primes100 = primeRange(0, 100);
    // Known primes up to 100: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97
    int expectedPrimes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    for (int p : expectedPrimes) {
        assert(primes100[p]);
    }
    // Check a few non-primes
    assert(!primes100[4] && !primes100[6] && !primes100[8] && !primes100[9]);
    assert(!primes100[1] && !primes100[0]);

    // Test n=2 (smallest prime)
    std::vector<bool> primes2 = primeRange(2, 2);
    assert(primes2.size() == 3);
    assert(!primes2[0] && !primes2[1] && primes2[2]);

    // Test n=3
    std::vector<bool> primes3 = primeRange(0, 3);
    assert(primes3[2] && primes3[3] && !primes3[0] && !primes3[1]);

    // Test a moderately large n (e.g., 1000) – spot check via known count of primes
    std::vector<bool> primes1000 = primeRange(0, 1000);
    int count = 0;
    for (bool b : primes1000) if (b) ++count;
    assert(count == 168); // Known number of primes ≤ 1000

    return 0;
}

#include <vector>
#include <cmath>

// Return a vector<bool> where index i is true if i is prime, false otherwise, for 0 <= i <= n.
std::vector<bool> primeRange(int m, int n) {
    // m is unused; n determines the sieve size.
    std::vector<bool> isPrime(n + 1, true);
    if (n >= 0) {
        isPrime[0] = false;
    }
    if (n >= 1) {
        isPrime[1] = false;
    }

    int limit = static_cast<int>(std::sqrt(n));
    for (int i = 2; i <= limit; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    return isPrime;
}

// The solution uses the classic Sieve of Eratosthenes. Initialize a `vector<bool>` of size `n+1` with all elements set to `true` (assuming all numbers are prime initially). Then set indices 0 and 1 to `false` because they are not prime. For each integer `i` from 2 up to `sqrt(n)`, if `i` is still marked as prime, then mark all multiples of `i` starting from `i*i` as `false` (since smaller multiples like `2*i` would have already been marked by smaller primes). The optimization of starting at `i*i` reduces redundant work. Edge cases: if `n < 2`, the vector of size `n+1` will have all `false` (since 0 and 1 are non-prime, and no other numbers exist). If `n` is 0 or 1, the loop from 2 will not execute, and the vector correctly has `false` for all indices. Also note that the `m` parameter is unused inside the function, but it is included to match the task specification; the caller is responsible for extracting the subrange. Time complexity is O(n log log n) due to the sieve, and space complexity is O(n) for the vector. The vector<bool> uses 1 bit per element in typical implementations, so it is memory-efficient for n up to 1,000,000.
