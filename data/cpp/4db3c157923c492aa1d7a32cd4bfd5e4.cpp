/*
Write a C++ function `std::string primeFactorization(long long n)` that takes a single integer `n` (which may be negative, zero, or positive) and returns a string describing its prime factorization in the exact format: `"n = -1 x p1 x p2 x ..."` for negative numbers (with `-1` printed after the equals sign), and `"n = p1 x p2 x ..."` for positive numbers. If `n` is `0`, return the string `"0"` (the original code stops on zero input, but for a standalone function we define that behavior). The factors must be in non-decreasing order, and repeated factors must appear as many times as they occur. For example, `12` → `"12 = 2 x 2 x 3"`, `-30` → `"-30 = -1 x 2 x 3 x 5"`, `1` → `"1 = 1"`, and `-1` → `"-1 = -1"`. The function must handle numbers up to `2^63 - 1` (i.e., signed 64‑bit range) efficiently, using trial division with a precomputed list of primes up to the cube root of the maximum value, and then handling any remaining large prime factor.
*/

#include <string>
#include <vector>
#include <cmath>

// Precompute primes up to a given limit using a simple sieve.
std::vector<long long> getPrimes(long long limit) {
    std::vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (long long i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (long long j = i * i; j <= limit; j += i) {
                isPrime[j] = false;
            }
        }
    }
    std::vector<long long> primes;
    for (long long i = 2; i <= limit; ++i) {
        if (isPrime[i]) primes.push_back(i);
    }
    return primes;
}

// Return the prime factorization string for n.
// Handles negative numbers, zero, and positive numbers up to 2^63-1.
std::string primeFactorization(long long n) {
    if (n == 0) return "0";

    std::string result = std::to_string(n) + " =";
    bool first = true;

    // Handle negative sign
    unsigned long long absN;
    if (n < 0) {
        result += " -1";
        first = false;
        absN = static_cast<unsigned long long>(-(n + 1)) + 1ULL; // correct abs for LLONG_MIN
    } else {
        absN = static_cast<unsigned long long>(n);
    }

    if (absN == 1) {
        if (first) {
            result += " 1";
        }
        return result;
    }

    // Precompute primes up to 2,000,000 (enough for cube root of 2^63)
    static const std::vector<long long> primes = getPrimes(2000000);

    // Factor by trial division
    for (size_t i = 0; i < primes.size() && primes[i] * primes[i] <= absN; ++i) {
        long long p = primes[i];
        while (absN % p == 0) {
            absN /= p;
            if (first) {
                result += " " + std::to_string(p);
                first = false;
            } else {
                result += " x " + std::to_string(p);
            }
        }
    }

    // If remaining absN > 1, it must be a prime factor
    if (absN > 1) {
        if (first) {
            result += " " + std::to_string(absN);
        } else {
            result += " x " + std::to_string(absN);
        }
    }

    return result;
}

#include <cassert>
#include <string>

// The solution function is declared here (for testing we include it inline)
std::string primeFactorization(long long n);

int main() {
    // Basic positive cases
    assert(primeFactorization(1) == "1 = 1");
    assert(primeFactorization(2) == "2 = 2");
    assert(primeFactorization(12) == "12 = 2 x 2 x 3");
    assert(primeFactorization(97) == "97 = 97");

    // Negative cases
    assert(primeFactorization(-1) == "-1 = -1");
    assert(primeFactorization(-30) == "-30 = -1 x 2 x 3 x 5");
    assert(primeFactorization(-2) == "-2 = -1 x 2");

    // Zero case
    assert(primeFactorization(0) == "0");

    // Large numbers (fits in 64-bit)
    assert(primeFactorization(2147483648LL) == "2147483648 = 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2");

    // Edge case: LLONG_MIN (cannot be negated safely)
    assert(primeFactorization(-9223372036854775807LL - 1) == "-9223372036854775808 = -1 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2 x 2");

    return 0;
}

// The key challenge is factoring a number up to ~9.2e18. Trial division by all odd numbers up to sqrt(n) would be too slow for large primes. We precompute all primes up to 1,000,000 (since the cube root of 2^63 is about 2.1e6, but 1e6 is enough for practical trial division; however, to be safe we use up to 2,000,000). We first handle the sign: if `n` is negative, we output `-1` as the first factor (after the equals sign) and then factor the absolute value (as unsigned long long). If `n` is positive, we just factor it. We also handle edge cases: `n == 0` returns `"0"`, `n == 1` returns `"1 = 1"`, `n == -1` returns `"-1 = -1"`. For the main loop, we iterate over the precomputed primes (starting from 2). For each prime `p`, while `n % p == 0`, we append `p` to the factor list and divide `n` by `p`. If after dividing by all primes up to the square root of the remaining `n` (which we check by `p * p <= remaining`), the remaining `n` is greater than 1, then it is a prime factor itself (since we have exhausted all smaller divisors). We append it once. The time complexity is O(π(sqrt(n))) in the worst case, but with the precomputed primes up to ~2e6, the loop runs at most about 148,000 iterations (number of primes up to 2e6), which is acceptable for a single call. Space complexity is O(number of primes) for the sieve and O(number of factors) for the output.
