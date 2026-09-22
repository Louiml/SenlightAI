/*
Write a C++ function `countPrimesUpTo` that takes an integer `n` (where `0 <= n <= 1,000,000`) and returns the number of prime numbers in the range `[2, n]` inclusive. The function must use the Sieve of Eratosthenes algorithm for efficiency. For `n < 2`, return `0`. The function should be `const`-correct and not allocate memory on each call—instead, it should use a static precomputed array that is initialized only once (e.g., with a static flag or using a static vector). The solution must be self-contained and not rely on any external libraries beyond the C++ standard library.
*/

#include <vector>
#include <cmath>

// Returns the number of prime numbers in [2, n] inclusive.
// Uses a static precomputed Sieve of Eratosthenes with prefix counts.
int countPrimesUpTo(int n) {
    const int MAXN = 1000000;
    static std::vector<int> primeCount;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        primeCount.assign(MAXN + 1, 0);
        std::vector<bool> isPrime(MAXN + 1, true);
        isPrime[0] = isPrime[1] = false;
        for (int i = 2; i * i <= MAXN; ++i) {
            if (isPrime[i]) {
                for (int j = i * i; j <= MAXN; j += i) {
                    isPrime[j] = false;
                }
            }
        }
        int count = 0;
        for (int i = 2; i <= MAXN; ++i) {
            if (isPrime[i]) {
                ++count;
            }
            primeCount[i] = count;
        }
    }
    if (n < 0) {
        return 0;
    }
    if (n > MAXN) {
        n = MAXN; // Clamp to max supported value, but we assume valid input
    }
    return primeCount[n];
}

#include <cassert>

int main() {
    assert(countPrimesUpTo(0) == 0);
    assert(countPrimesUpTo(1) == 0);
    assert(countPrimesUpTo(2) == 1); // {2}
    assert(countPrimesUpTo(3) == 2); // {2,3}
    assert(countPrimesUpTo(10) == 4); // {2,3,5,7}
    assert(countPrimesUpTo(100) == 25);
    assert(countPrimesUpTo(-5) == 0);
    assert(countPrimesUpTo(1000000) == 78498);
    // Test consistency: countPrimesUpTo(20) = 8 (2,3,5,7,11,13,17,19)
    assert(countPrimesUpTo(20) == 8);
    return 0;
}

// The core algorithm is the Sieve of Eratosthenes, which works by creating a boolean array of size `n+1` initially all set to `true` (except index 0 and 1 which are not prime). For each integer `i` from 2 to `sqrt(n)`, if `i` is still marked as prime, mark all multiples of `i` (starting from `i*i`) as non-prime. After the sieve, the array has `true` at indices that are prime numbers. To answer counts for any `n` quickly, we can precompute a prefix count array `primeCount[i]` that stores the number of primes up to `i`. The function can then return `primeCount[n]` directly in O(1) after the precomputation. Edge cases: `n` may be 0 or 1, which have no primes (return 0); `n` may be 2 (the only even prime). Also, we must handle potential negative inputs gracefully (e.g., by clamping to 0). Time complexity: Precomputation is O(N log log N) for sieve and O(N) for prefix sum, where N is the maximum possible input (1,000,000). Per call: O(1) due to direct lookup. Space complexity: O(N) for the arrays. We will use a static vector of `int` to store prefix counts, and a static vector of `bool` for sieve; both are initialized once using a static flag to avoid re-computation on repeated calls.
