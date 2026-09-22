/*
Write a C++ function `countPrimesUpTo(int limit)` that returns a `struct` containing three fields: the number of prime numbers in the range `[2, limit]`, the sum of all those primes, and a boolean flag indicating whether the result is valid (i.e., `limit >= 2`). The function must handle large limits up to `INT_MAX` carefully—do not allocate memory proportional to the limit; instead, use a segmented sieve or a primality check based on trial division up to the square root for each candidate. However, for clarity and correctness, you may use a simple vector-based sieve of Eratosthenes for limits up to 10 million, but for larger limits, your function must not exceed reasonable memory (e.g., it should return `{0,0,false}` for limits above 10 million with a clear note that the limit is unsupported). The function must be `const`-correct, use efficient types (e.g., `uintmax_t` for the sum), and return the sum as a 64-bit unsigned integer to avoid overflow.
*/
#include <cstdint>
#include <vector>
#include <cmath>

struct PrimeStats {
    int count;          // number of primes in [2, limit]
    uintmax_t sum;      // sum of those primes
    bool valid;        // false if limit < 2 or limit > SUPPORTED_LIMIT
};

// Count and sum primes up to a given limit.
// Supports limits up to 10,000,000; otherwise returns invalid.
PrimeStats countPrimesUpTo(int limit) {
    const int SUPPORTED_LIMIT = 10000000;
    if (limit < 2 || limit > SUPPORTED_LIMIT) {
        return {0, 0, false};
    }

    // Sieve of Eratosthenes: false means composite, true means prime (candidate)
    std::vector<bool> isPrime(limit + 1, true);
    if (limit >= 0) isPrime[0] = false;
    if (limit >= 1) isPrime[1] = false;

    for (int p = 2; p * p <= limit; ++p) {
        if (isPrime[p]) {
            // Mark multiples of p starting from p*p
            for (int multiple = p * p; multiple <= limit; multiple += p) {
                isPrime[multiple] = false;
            }
        }
    }

    PrimeStats result{0, 0, true};
    for (int i = 2; i <= limit; ++i) {
        if (isPrime[i]) {
            ++result.count;
            result.sum += static_cast<uintmax_t>(i);
        }
    }

    return result;
}
#include <cassert>
#include <cstdint>

// declaration of countPrimesUpTo and PrimeStats from solution

int main() {
    // limit < 2 -> invalid
    PrimeStats r1 = countPrimesUpTo(0);
    assert(r1.valid == false);
    assert(r1.count == 0);
    assert(r1.sum == 0);

    r1 = countPrimesUpTo(1);
    assert(r1.valid == false);

    // limit = 2: only prime 2
    r1 = countPrimesUpTo(2);
    assert(r1.valid == true);
    assert(r1.count == 1);
    assert(r1.sum == 2);

    // limit = 10: primes 2,3,5,7 => count=4 sum=17
    r1 = countPrimesUpTo(10);
    assert(r1.valid == true);
    assert(r1.count == 4);
    assert(r1.sum == 17);

    // limit = 100: primes less than 100 (there are 25, sum = 1060)
    r1 = countPrimesUpTo(100);
    assert(r1.valid == true);
    assert(r1.count == 25);
    assert(r1.sum == 1060);

    // limit = 1000: known count = 168, sum = 76127
    r1 = countPrimesUpTo(1000);
    assert(r1.valid == true);
    assert(r1.count == 168);
    assert(r1.sum == 76127);

    // limit = 10000: count = 1229, sum = 5736396
    r1 = countPrimesUpTo(10000);
    assert(r1.valid == true);
    assert(r1.count == 1229);
    assert(r1.sum == 5736396);

    // limit larger than supported -> invalid
    r1 = countPrimesUpTo(10000001);
    assert(r1.valid == false);

    // limit = INT_MAX -> invalid
    r1 = countPrimesUpTo(2147483647);
    assert(r1.valid == false);

    // large supported limit (10 million) – just check validity and count > 0
    r1 = countPrimesUpTo(10000000);
    assert(r1.valid == true);
    assert(r1.count > 0);
    // Known count of primes <= 10^7 = 664579, sum can be large but we just check positivity
    assert(r1.count == 664579);
    // sum of primes <= 10^7 is 3203324994356, fits in uintmax_t (64-bit)
    assert(r1.sum == 3203324994356ULL);

    return 0;
}
// The solution uses the Sieve of Eratosthenes for limits up to 10,000,000 (10^7). We allocate a boolean vector of size `limit+1`, initialized to `true` except indices 0 and 1 set to `false`. We iterate from 2 up to the square root of `limit`; for each prime found, we mark all multiples starting from `p*p` as composite. After the sieve, we traverse the vector from 2 to `limit`, counting primes and accumulating their sum into a `uintmax_t`. Edge cases: if `limit < 2`, the range is empty, so we return `{0,0,false}`. If `limit > 10,000,000`, we return `{0,0,false}` to avoid excessive memory usage (a vector of `bool` for 10^7 takes about 10 MB, which is acceptable). For limits up to 10^7, the time complexity is `O(n log log n)` and space complexity is `O(n)` bits. For larger limits, the function returns invalid without attempting the sieve, so time is `O(1)` in that case.
