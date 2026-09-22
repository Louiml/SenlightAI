/*
Write a C++ function `countPrimesUpTo(int x)` that returns the number of prime numbers from 1 to `x` inclusive, given that `x` is a positive integer (≥ 1). The function should handle `x = 1` correctly (returning 0) and should use an efficient prime-counting method rather than trial division for each number. The function must be self-contained and not rely on global state or precomputed tables.
*/

#include <vector>
#include <cmath>

// Count the number of primes from 1 to x inclusive.
// Assumes x >= 1.
int countPrimesUpTo(int x) {
    if (x < 2) {
        return 0;
    }

    std::vector<bool> isPrime(x + 1, true);
    isPrime[0] = false;
    isPrime[1] = false;

    // Mark even numbers > 2 as non-prime
    for (int i = 4; i <= x; i += 2) {
        isPrime[i] = false;
    }

    // Sieve odd numbers only
    int limit = static_cast<int>(std::sqrt(x));
    for (int i = 3; i <= limit; i += 2) {
        if (isPrime[i]) {
            for (int j = i * i, step = i * 2; j <= x; j += step) {
                isPrime[j] = false;
            }
        }
    }

    int count = 0;
    for (int i = 2; i <= x; ++i) {
        if (isPrime[i]) {
            ++count;
        }
    }
    return count;
}

#include <cassert>

int main() {
    assert(countPrimesUpTo(1) == 0);
    assert(countPrimesUpTo(2) == 1);
    assert(countPrimesUpTo(3) == 2);
    assert(countPrimesUpTo(10) == 4);   // 2,3,5,7
    assert(countPrimesUpTo(20) == 8);   // 2,3,5,7,11,13,17,19
    assert(countPrimesUpTo(50) == 15);
    assert(countPrimesUpTo(100) == 25);
    assert(countPrimesUpTo(1000) == 168);
    assert(countPrimesUpTo(5000) == 669);
    return 0;
}

// The solution uses the Sieve of Eratosthenes to precompute all primes up to `x`. Start with a boolean vector of size `x+1` initially all `true`. Mark indices 0 and 1 as `false` since they are not prime. For even numbers starting from 4 up to `x`, mark them `false` (since 2 is the only even prime). Then iterate odd numbers from 3 to the integer square root of `x`; if a number is still `true`, it is prime, so mark all its multiples (starting from its square, stepping by twice the number to skip even multiples) as `false`. After the sieve, count the `true` values in the range [2..x] and return that count. This approach correctly handles `x=1` (returns 0), `x=2` (returns 1), and `x=0` (undefined, but the task guarantees positive input). Time complexity is O(x log log x) for the sieve, and space is O(x). Edge cases: `x=1` returns 0 because neither 0 nor 1 is prime; `x` as a large value ensures the sieve’s inner loop only runs to sqrt(x) for efficiency.
