// Write a C++ function named `smallestPrimeFactorTable` that takes a non-negative integer `n` and returns a `std::vector<long long>` of length `n+1` where the element at index `i` is the smallest prime factor of `i` (with the convention that `smallestPrimeFactorTable[0] = 0` and `smallestPrimeFactorTable[1] = 1`). For composite numbers, the smallest prime factor is the smallest prime that divides the number; for prime numbers, the element equals the prime itself. The function must handle `n = 0` and `n = 1` correctly, and must compute the table using a sieve-based approach in `O(n log log n)` time.

#include <cassert>
#include <vector>

// Function declaration (solution above)
std::vector<long long> smallestPrimeFactorTable(long long n);

int main() {
    // n = 0
    std::vector<long long> t0 = smallestPrimeFactorTable(0);
    assert(t0.size() == 1);
    assert(t0[0] == 0);

    // n = 1
    std::vector<long long> t1 = smallestPrimeFactorTable(1);
    assert(t1.size() == 2);
    assert(t1[0] == 0);
    assert(t1[1] == 1);

    // n = 10
    std::vector<long long> t10 = smallestPrimeFactorTable(10);
    assert(t10.size() == 11);
    assert(t10[0] == 0);
    assert(t10[1] == 1);
    assert(t10[2] == 2);
    assert(t10[3] == 3);
    assert(t10[4] == 2);
    assert(t10[5] == 5);
    assert(t10[6] == 2);
    assert(t10[7] == 7);
    assert(t10[8] == 2);
    assert(t10[9] == 3);
    assert(t10[10] == 2);

    // n = 20 (spot checks)
    std::vector<long long> t20 = smallestPrimeFactorTable(20);
    assert(t20[12] == 2);
    assert(t20[15] == 3);
    assert(t20[17] == 17);
    assert(t20[19] == 19);

    // n = 100, check composite and prime
    std::vector<long long> t100 = smallestPrimeFactorTable(100);
    assert(t100[97] == 97);
    assert(t100[100] == 2);
    assert(t100[99] == 3);
    assert(t100[49] == 7);
    assert(t100[2] == 2);

    return 0;
}

#include <vector>
#include <cstdint>

// Returns a vector of size n+1 where spf[i] is the smallest prime factor of i.
// spf[0] = 0, spf[1] = 1, spf[prime] = prime, spf[composite] = its smallest prime factor.
std::vector<long long> smallestPrimeFactorTable(long long n) {
    std::vector<long long> spf(static_cast<size_t>(n) + 1, -1);
    if (n >= 0) spf[0] = 0;
    if (n >= 1) spf[1] = 1;

    for (long long i = 2; i <= n; ++i) {
        if (spf[static_cast<size_t>(i)] != -1) continue; // already marked as composite or prime
        // i is prime
        spf[static_cast<size_t>(i)] = i;
        for (long long j = i + i; j <= n; j += i) {
            if (spf[static_cast<size_t>(j)] == -1) {
                spf[static_cast<size_t>(j)] = i;
            }
        }
    }
    return spf;
}

// The solution builds a table `spf` (smallest prime factor) of size `n+1`, initialized with `-1` to mark "unprocessed". We set `spf[0] = 0` and `spf[1] = 1` as special cases. Then, for every integer `i` from 2 up to `n`, if `spf[i]` is still `-1`, then `i` is prime; we set `spf[i] = i`, and then for every multiple `j = 2*i, 3*i, ...` up to `n`, if `spf[j]` is still `-1` (meaning it hasn't been assigned a smaller prime factor), we set `spf[j] = i`. This guarantees that each composite gets its smallest prime factor because we process primes in increasing order and only fill unfilled slots. The special cases for `0` and `1` avoid confusion since they are not primes. The time complexity is `O(n log log n)` because the inner loop runs for each prime `p` about `n/p` times, summing to `n * (sum of reciprocals of primes) ≈ n log log n`. Space complexity is `O(n)` for the vector. Edge cases: for `n = 0`, the vector has size 1; for `n = 1`, size 2. The function must use `long long` to avoid overflow for large `n`, and the input is non-negative so no negative handling is needed.
