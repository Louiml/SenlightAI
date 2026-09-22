/*
Write a C++ function named `findPollardFactor` that, given a composite integer `n` (as a `long long`), returns one non-trivial factor of `n` using Pollard's Rho algorithm with the polynomial `f(x) = x^2 + 1`. If the algorithm fails to find a factor (e.g., the cycle detection returns `n` itself as the GCD), the function should return `-1` to signal failure. The function must handle `n` up to \(10^{18}\) safely, so use `unsigned long long` or `__int128` for intermediate arithmetic to avoid overflow. The function signature should be `long long findPollardFactor(long long n)`, and it must be self-contained (include all necessary headers, but no `main`). The key requirement is correctness and robustness: if `n` is prime or `n` is 1, return `-1` (since no factor exists). For composite `n`, attempt to find a factor; only return `-1` if the algorithm truly fails (e.g., GCD becomes `n` and the loop restarts with a different initial value at least once, but if all attempts fail, return `-1`). Assume `n` is positive and less than or equal to \(10^{18}\).
*/
#include <cstdint>
#include <cstdlib>
#include <ctime>

// Forward declaration for Miller-Rabin primality test (used to quickly reject primes)
static bool isPrime(long long n);

// Pollard's Rho factorization attempt with a given constant c
// Returns a factor if found, otherwise -1
static long long pollardAttempt(long long n, long long c) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    long long x = 2, y = 2, d = 1;
    auto f = [&](long long v) -> long long {
        __int128 mul = (__int128)v * v + c;
        return (long long)(mul % n);
    };

    for (int i = 0; i < 1000 && d == 1; ++i) {
        x = f(x);
        y = f(f(y));
        long long diff = x > y ? x - y : y - x;
        if (diff == 0) return -1; // cycle without a factor
        // Compute gcd(diff, n) using a simple loop (n up to 1e18)
        long long a = diff, b = n;
        while (b) {
            long long t = a % b;
            a = b;
            b = t;
        }
        d = a;
    }
    if (d == n || d == 1) return -1;
    return d;
}

// Main function: find a non-trivial factor of n, or -1 if none found
long long findPollardFactor(long long n) {
    if (n <= 3) return -1;
    if (isPrime(n)) return -1;

    // Try several different constants for the polynomial f(x)=x^2+c
    for (long long c = 1; c <= 10; ++c) {
        long long factor = pollardAttempt(n, c);
        if (factor != -1 && factor != n && factor != 1) {
            return factor;
        }
    }
    return -1;
}

// Simple Miller-Rabin deterministic for 64-bit integers
static bool isPrime(long long n) {
    if (n < 2) return false;
    for (long long p : {2LL, 3LL, 5LL, 7LL, 11LL, 13LL, 17LL, 19LL, 23LL, 29LL, 31LL, 37LL}) {
        if (n % p == 0) return n == p;
    }
    long long d = n - 1, s = 0;
    while (d % 2 == 0) { d /= 2; s++; }
    for (long long a : {2LL, 3LL, 5LL, 7LL, 11LL, 13LL, 17LL, 19LL, 23LL, 29LL, 31LL, 37LL}) {
        if (a >= n) continue;
        __int128 x = 1, base = a, exp = d;
        while (exp) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}
#include <cassert>

// Declare the function (as defined above)
long long findPollardFactor(long long n);

int main() {
    // Composite numbers: should return a non-trivial factor
    long long f1 = findPollardFactor(91);
    assert(f1 != -1 && f1 != 1 && f1 != 91 && (91 % f1 == 0));
    assert((f1 == 7) || (f1 == 13));

    long long f2 = findPollardFactor(2111 * 233);
    assert(f2 != -1 && f2 != 1 && f2 != 2111*233 && ( (2111*233) % f2 == 0));
    assert(f2 == 2111 || f2 == 233);

    long long f3 = findPollardFactor(10403); // 101*103
    assert(f3 != -1 && f3 != 1 && f3 != 10403 && (10403 % f3 == 0));

    long long f4 = findPollardFactor(8051); // 83*97
    assert(f4 != -1 && f4 != 1 && f4 != 8051 && (8051 % f4 == 0));

    // Prime numbers: should return -1
    assert(findPollardFactor(2) == -1);
    assert(findPollardFactor(3) == -1);
    assert(findPollardFactor(104729) == -1); // a prime
    assert(findPollardFactor(1000000007) == -1); // prime

    // Edge case: n=1
    assert(findPollardFactor(1) == -1);

    // Larger composite with 64-bit values
    long long n5 = 1000000000000000003LL; // actually prime? – we check
    // Since we don't know if it’s prime, skip assert for this, but we can test a known composite:
    long long n6 = 1000000000000000000LL; // even, so factor 2
    long long f6 = findPollardFactor(n6);
    assert(f6 == 2 || (n6 % f6 == 0 && f6 > 1 && f6 < n6));

    // Test a product of two primes near 1e9
    long long p = 1000000007; // prime
    long long q = 1000000009; // prime
    long long n7 = p * q; // ~1e18
    long long f7 = findPollardFactor(n7);
    assert(f7 != -1 && f7 != 1 && f7 != n7 && (n7 % f7 == 0));
    assert(f7 == p || f7 == q);

    // Test a perfect square
    long long n8 = 121 * 121;
    long long f8 = findPollardFactor(n8);
    assert(f8 != -1 && f8 != 1 && f8 != n8 && (n8 % f8 == 0));
    assert(f8 == 11 || f8 == 121);

    return 0;
}
// Pollard's Rho algorithm is a probabilistic factorization method that finds a non-trivial factor of a composite number in expected \(O(n^{1/4})\) arithmetic operations. The algorithm uses two pointers, `a` and `b`, both starting from the same seed (here `x0=2`). At each iteration, `a` advances one step by applying the polynomial `f(x)=x^2+1`, while `b` advances two steps. The difference `|a-b|` is then combined with `n` via a GCD. If the GCD is greater than 1 but less than `n`, we have found a factor. If the GCD equals `n`, the cycle detection failed and the algorithm should restart with a different seed (e.g., increment `x0`). Since `n` can be up to \(10^{18}\), multiplication `x*x` can overflow 64-bit integers, so we use `__int128` for intermediate products and then take modulo `n` after each multiplication. Edge cases: if `n` is less than 2, or if `n` is even, we can immediately return `2` (but to keep it general, we check primality first). The algorithm can loop indefinitely if not capped; we should add a maximum iteration count (e.g., 1000 attempts per seed, and 10 different seeds) before returning `-1`. Time complexity is roughly \(O(n^{1/4})\) in expectation, but with overflow-safe operations and a bounded attempt count; space complexity is \(O(1)\) auxiliary, but we use a few `__int128` temporaries.
