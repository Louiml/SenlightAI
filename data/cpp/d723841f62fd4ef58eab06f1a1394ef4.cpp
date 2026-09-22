/*
Write a C++ function named `primeFactorization` that takes a single `long long` integer `n` (with `n >= 2`) and returns a `std::vector<std::pair<long long, long long>>` representing its prime factorization. Each pair contains a prime factor and its exponent (the number of times that prime divides `n`). The pairs must be ordered in strictly increasing order of the prime factor, from smallest to largest. For example, for input `84`, the result should be `{{2,2},{3,1},{7,1}}` because `84 = 2^2 * 3^1 * 7^1`. The function must handle large numbers up to `10^18` efficiently by trial division up to the square root, and must also correctly handle inputs that are prime numbers, powers of two, and perfect squares of primes.
*/
#include <vector>
#include <utility>

// Return the prime factorization of n as a vector of {prime, exponent} pairs,
// ordered by increasing prime factor.
std::vector<std::pair<long long, long long>> primeFactorization(long long n) {
    std::vector<std::pair<long long, long long>> factors;

    // Handle factor 2 separately.
    long long count = 0;
    while (n % 2 == 0) {
        n /= 2;
        ++count;
    }
    if (count > 0) {
        factors.push_back({2, count});
    }

    // Check odd divisors up to sqrt(n).
    for (long long i = 3; i * i <= n; i += 2) {
        count = 0;
        while (n % i == 0) {
            n /= i;
            ++count;
        }
        if (count > 0) {
            factors.push_back({i, count});
        }
    }

    // If n is still > 2, it is a prime factor with exponent 1.
    if (n > 2) {
        factors.push_back({n, 1});
    }

    return factors;
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration
std::vector<std::pair<long long, long long>> primeFactorization(long long n);

int main() {
    // Basic cases
    assert(primeFactorization(2) == std::vector<std::pair<long long, long long>>{{2, 1}});
    assert(primeFactorization(3) == std::vector<std::pair<long long, long long>>{{3, 1}});
    assert(primeFactorization(4) == std::vector<std::pair<long long, long long>>{{2, 2}});
    assert(primeFactorization(12) == std::vector<std::pair<long long, long long>>{{2, 2}, {3, 1}});
    assert(primeFactorization(84) == std::vector<std::pair<long long, long long>>{{2, 2}, {3, 1}, {7, 1}});

    // Perfect square of a prime
    assert(primeFactorization(49) == std::vector<std::pair<long long, long long>>{{7, 2}});

    // Large prime
    assert(primeFactorization(9999999967LL) == std::vector<std::pair<long long, long long>>{{9999999967LL, 1}});

    // Power of two
    assert(primeFactorization(1024) == std::vector<std::pair<long long, long long>>{{2, 10}});

    // Product of primes and additional checks
    assert(primeFactorization(1000003LL * 1000003LL) == std::vector<std::pair<long long, long long>>{{1000003LL, 2}});
    assert(primeFactorization(97 * 97 * 2) == std::vector<std::pair<long long, long long>>{{2, 1}, {97, 2}});

    return 0;
}
// The solution uses trial division: remove all factors of 2 first, then iterate through odd candidates from 3 up to the current value of `n` while `i*i <= n`. For each candidate `i`, count how many times it divides `n`, reduce `n` accordingly, and if the count is positive, store `{i, count}`. After the loop, if the remaining `n` is greater than 2, it must be a prime factor with exponent 1 (because any composite would have been split earlier). This handles all cases: prime numbers (no divisors found before the loop ends, then `n > 2` is stored with exponent 1), powers of 2 (add factor 2 with appropriate exponent, then `n` becomes 1), and perfect squares (the square root is examined exactly when `i*i == n`). Time complexity is `O(√n)` in the worst case (e.g., for a large prime), but for most inputs it is faster because `n` decreases quickly. Space complexity is `O(k)` where `k` is the number of distinct prime factors (at most about 60 for `2*3*5*...`). Edge cases include `n = 2` (returns `{{2,1}}`), `n` being a large prime like `9999999967` (returns `{{9999999967,1}}`), and `n` being a power of a large prime like `1000003^2` (loops until `i = 1000003`, then adds it with exponent 2).
