/*
Write a C++ function named `countPrimeNumbers` that takes an integer `n` and returns the number of prime numbers strictly less than `n` using the Sieve of Eratosthenes algorithm. If `n` is less than or equal to 2, the function should return 0. The function must handle edge cases such as `n = 0`, `n = 1`, `n = 2`, very large values of `n` (up to at least 10^6), and must be efficient in both time and space. The function should not include any external libraries beyond the standard ones, and should contain only the function definition without a `main` function.
*/

#include <vector>

// Count the number of primes strictly less than n using Sieve of Eratosthenes.
int countPrimeNumbers(int n) {
    if (n <= 2) {
        return 0;
    }

    std::vector<bool> isPrime(n, true);
    isPrime[0] = false;
    isPrime[1] = false;

    for (int i = 2; i * i < n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    int count = 0;
    for (int i = 2; i < n; ++i) {
        if (isPrime[i]) {
            ++count;
        }
    }

    return count;
}

#include <cassert>

int main() {
    // Edge cases: n <= 2
    assert(countPrimeNumbers(0) == 0);
    assert(countPrimeNumbers(1) == 0);
    assert(countPrimeNumbers(2) == 0);

    // Small values
    assert(countPrimeNumbers(3) == 1);   // 2
    assert(countPrimeNumbers(4) == 2);   // 2, 3
    assert(countPrimeNumbers(5) == 2);   // 2, 3
    assert(countPrimeNumbers(10) == 4);  // 2,3,5,7

    // Larger values
    assert(countPrimeNumbers(20) == 8);  // 2,3,5,7,11,13,17,19
    assert(countPrimeNumbers(30) == 10); // 2,3,5,7,11,13,17,19,23,29
    assert(countPrimeNumbers(100) == 25);
    assert(countPrimeNumbers(1000) == 168);
    assert(countPrimeNumbers(1000000) == 78498);

    return 0;
}

// The Sieve of Eratosthenes works by creating a boolean vector `isPrime` of size `n`, initialized to `true` for all indices from 0 to `n-1`. We set `isPrime[0]` and `isPrime[1]` to `false` since 0 and 1 are not prime. Then, starting from `i = 2`, we check if `i` is still marked as prime. If so, we mark all multiples of `i` starting from `i * i` (because any smaller multiple would have been already removed by a smaller prime factor) as `false`, incrementing by `i` each step. The loop continues while `i * i < n`. After the sieve is complete, we count the number of `true` entries from index 2 up to `n-1`. Important edge cases: if `n <= 2`, no primes exist (since the smallest prime is 2, which is not less than 2), so return 0. Also, when `n` is large, the loop condition `i * i < n` avoids overflow. Time complexity is O(n log log n) due to the sieve’s harmonic series behavior, and space complexity is O(n) for the boolean vector. Using `std::vector<bool>` is memory-efficient and appropriate.
