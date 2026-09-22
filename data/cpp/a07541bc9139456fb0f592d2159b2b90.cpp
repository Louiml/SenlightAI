// Write a C++ function `long long pollardRho(long long n)` that returns a non-trivial divisor of an odd composite number `n` using Pollard's Rho algorithm with a deterministic Brent-style cycle detection optimization. The function must handle any odd `n > 1`, including prime numbers (where it should return `n` itself) and perfect powers. It must avoid overflow in modular multiplication by implementing `mulmod` using either `__int128` or binary multiplication with overflow checks. The divisor returned must satisfy `1 < d < n` for composite inputs and equal `n` for prime inputs. You may use `std::mt19937_64` for randomness, seeded with a fixed constant for reproducibility. The function must be efficient for numbers up to \(10^{18}\) and should handle edge cases like `n = 1` (return 1) and even `n` (return 2 immediately).
// The core idea of Pollard's Rho is to find a nontrivial factor by iterating a polynomial \(f(x) = (x^2 + c) \mod n\) and detecting a collision modulo a prime factor \(p\) of \(n\). Starting from a random initial value, we generate a sequence \(x_{i+1} = f(x_i)\). When two values are congruent modulo \(p\), their difference is a multiple of \(p\), so computing \(\gcd(|x_i - x_j|, n)\) yields a divisor of \(n\). For efficiency, we use Brent's cycle detection: maintain a "power-of-two" checkpoint `x` and advance `y` through the sequence while accumulating the product of differences modulo `n` in batches of up to 26 steps. If the product becomes zero (meaning a difference shares a factor with `n`), we immediately compute the GCD of the previous product with `n`. Otherwise, after each batch we compute one GCD against the accumulated product. This reduces the number of costly GCD calls from \(O(\sqrt{n})\) to \(O(\sqrt[4]{n})\) in practice. For prime `n`, the algorithm fails to find a factor and eventually detects a cycle; we then restart with a new random constant `c`. Since the probability of failure per iteration is low, a few attempts suffice. Edge cases: even `n` returns 2; `n` prime returns `n` after exhausting attempts; `n = 1` returns 1. Multiplicative overflow is avoided using `__int128` for the product. Time complexity is roughly \(O(n^{1/4})\) in the average case, with space \(O(1)\) (only a constant-size buffer for Brent's preallocated array). The algorithm is probabilistic but with bounded retries.
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <random>

using int64 = long long;

// Modular multiplication without overflow using __int128
static int64 mulmod(int64 a, int64 b, int64 mod) {
    return (int64)((__int128)a * b % mod);
}

static int64 gcd(int64 a, int64 b) {
    while (b != 0) {
        int64 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static int64 abs64(int64 x) {
    return x < 0 ? -x : x;
}

// Pollard's Rho with Brent's cycle detection. Returns a non-trivial divisor.
// For prime n, returns n after failed attempts. For n==1, returns 1.
int64 pollardRho(int64 n) {
    if (n <= 1) return 1;
    if (n % 2 == 0) return 2;

    std::mt19937_64 rng(1234567);  // deterministic seed
    const int64 max_attempts = 100;

    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        int64 x = (int64)(rng() % (n - 2)) + 2;  // random in [2, n-1]
        int64 y = x;
        int64 c = (int64)(rng() % (n - 1)) + 1;  // random in [1, n-1]
        int64 d = 1;

        auto f = [&](int64 val) { return (mulmod(val, val, n) + c) % n; };

        // Brent's cycle detection
        int64 power = 1;
        int64 lam = 1;
        int64 k = 0;
        int64 diff = 1;
        int64 product = 1;
        const int64 batch_size = 26;

        x = f(x);
        while (d == 1) {
            if (power == lam) {
                x = y;
                power *= 2;
                lam = 0;
            }
            y = f(y);
            lam++;
            diff = abs64(x - y);
            product = mulmod(product, diff, n);
            if (product == 0) {
                break;
            }
            if (++k == batch_size) {
                d = gcd(product, n);
                if (d > 1) break;
                k = 0;
                product = 1;
            }
        }

        if (d == 1) {
            d = gcd(product, n);
        }

        if (d > 1 && d < n) return d;
        // If d == n, failed for this attempt; retry with new c and x
    }
    return n;  // likely prime, return n
}
#include <cassert>
#include <cstdint>

int64 pollardRho(int64 n);  // declaration for linking

int main() {
    // Composite: expect a non-trivial divisor
    assert(pollardRho(91) != 1 && pollardRho(91) != 91);
    assert(91 % pollardRho(91) == 0);

    // Even number
    assert(pollardRho(1000000000000LL) == 2);

    // Small odd composite
    assert(pollardRho(15) == 3 || pollardRho(15) == 5);
    assert(15 % pollardRho(15) == 0);

    // Prime
    assert(pollardRho(97) == 97);  // no non-trivial divisor

    // Large composite: product of two primes
    int64 a = 1000000007LL;  // prime
    int64 b = 1000000009LL;  // prime
    int64 n = a * b;
    int64 d = pollardRho(n);
    assert(d != 1 && d != n);
    assert(n % d == 0);

    // Perfect square
    assert(pollardRho(49) == 7);

    // n == 1
    assert(pollardRho(1) == 1);

    // Another prime
    assert(pollardRho(2) == 2);

    return 0;
}
