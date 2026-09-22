Write a C++ function `int findPrivateExponent(int publicExponent, int modulus)` that, given a public RSA exponent `e` and a modulus `n = p * q` (where `p` and `q` are distinct primes), finds the private exponent `d` such that `(e * d) % φ(n) == 1`, where `φ(n) = (p - 1) * (q - 1)`. The function must factor `n` by trial division to find `p` and `q`, compute `φ(n)`, and then compute the modular multiplicative inverse of `e` modulo `φ(n)` using the extended Euclidean algorithm. Assume `n` is a product of two distinct primes, `e` is coprime with `φ(n)`, `e > 1`, and `n > 4`. The function should return `0` if `φ(n) == 1` (which cannot happen under the assumptions, but handle it gracefully), and it must handle cases where the inverse is negative by adding `φ(n)` to make it positive. Do not include a `main` function in your solution.
// The algorithm proceeds in three phases: (1) Factorization — iterate `p` from 2 up to `n-1`, and as soon as `n % p == 0`, set `q = n / p` and break. Since `n` is a product of two primes, this finds the smaller prime factor first (assuming trial division starts at 2). (2) Compute `phi = (p - 1) * (q - 1)`. (3) Compute the modular inverse of `e` modulo `phi` using the extended Euclidean algorithm: maintain coefficients `x0` and `x1` for `a` and `m` such that `a*x0 + m*y0 = gcd(a,m)`. The loop runs while `a > 1`, updating `a` and `m` via `q = a / m`, `t = m`, `m = a % m`, `a = t`, and update `x0 = x1 - q * x0`, `x1 = t` (where `t` was the old `x0`). After the loop, `x1` is the multiplicative inverse; if negative, add `m0` (the original modulus). Edge cases: if `phi == 1`, return 0 (though this cannot occur for distinct primes > 1); if `a` (initially `e`) is 1, the loop never runs and `x1` remains 1, which is correct. Time complexity is dominated by trial division, which is O(n) in the worst case (since we scan up to the smaller prime factor, which could be close to n/2 for large n), but for typical small test values it is fine. The extended Euclidean part is O(log(phi)). Space complexity is O(1) auxilary.
#include <utility>

// Given public exponent e and modulus n = p * q (p, q distinct primes),
// return the private exponent d = e^{-1} mod phi(n), where phi(n) = (p-1)*(q-1).
int findPrivateExponent(int publicExponent, int modulus) {
    // Step 1: Factor modulus by trial division to find p (the smaller factor)
    int p = 2;
    while (p < modulus) {
        if (modulus % p == 0) {
            break;
        }
        ++p;
    }
    int q = modulus / p;

    // Step 2: Compute Euler's totient
    int phi = (p - 1) * (q - 1);

    // Edge case: if phi == 1, no inverse exists (but this won't happen for distinct primes > 1)
    if (phi == 1) {
        return 0;
    }

    // Step 3: Extended Euclidean algorithm to find modular inverse of publicExponent mod phi
    int m0 = phi;
    int a = publicExponent;
    int m = phi;
    int x0 = 0;
    int x1 = 1;

    while (a > 1) {
        int q_step = a / m;
        int t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q_step * x0;
        x1 = t;
    }

    // Make the result positive
    if (x1 < 0) {
        x1 += m0;
    }

    return x1;
}
#include <cassert>

// Declare the solution function (in real usage, include the header or define above)
int findPrivateExponent(int publicExponent, int modulus);

int main() {
    // Example from snippet: e=31, n=3599 = 59 * 61, phi=58*60=3480, d = 31^{-1} mod 3480 = 3031
    assert(findPrivateExponent(31, 3599) == 3031);

    // Simple case: n=15 (3*5), phi=8, e=3, inverse of 3 mod 8 is 3 (since 3*3=9 ≡ 1 mod 8)
    assert(findPrivateExponent(3, 15) == 3);

    // n=21 (3*7), phi=12, e=5, inverse of 5 mod 12 is 5 (5*5=25 ≡ 1 mod 12)
    assert(findPrivateExponent(5, 21) == 5);

    // n=35 (5*7), phi=24, e=11, inverse of 11 mod 24 is 11 (11*11=121 ≡ 1 mod 24)
    assert(findPrivateExponent(11, 35) == 11);

    // n=33 (3*11), phi=20, e=7, inverse of 7 mod 20 is 3 (7*3=21 ≡ 1 mod 20)
    assert(findPrivateExponent(7, 33) == 3);

    // n=77 (7*11), phi=60, e=13, inverse of 13 mod 60 is 37 (13*37=481 ≡ 1 mod 60)
    assert(findPrivateExponent(13, 77) == 37);

    // n=9 is not valid (3*3 not distinct), but test with distinct: n=55 (5*11), phi=40, e=3, inverse of 3 mod 40 is 27 (3*27=81 ≡ 1 mod 40)
    assert(findPrivateExponent(3, 55) == 27);

    // Larger example: n=143 (11*13), phi=120, e=7, inverse of 7 mod 120 is 103 (7*103=721 ≡ 1 mod 120)
    assert(findPrivateExponent(7, 143) == 103);

    // Ensure the return is positive even when the raw inverse would be negative (e.g., e=3, n=55 already positive, but test a case with larger e)
    // n=91 (7*13), phi=72, e=5, inverse of 5 mod 72 is 29 (5*29=145 ≡ 1 mod 72)
    assert(findPrivateExponent(5, 91) == 29);

    return 0;
}
