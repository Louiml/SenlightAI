Implement a C++ function that, given a prime number `p` and two random private keys `Xa` and `Xb` (both less than `p`), computes the Diffie–Hellman shared secret by first finding a primitive root `g` of `p` (i.e., a generator of the multiplicative group modulo `p`), then computing `Ya = g^Xa mod p` and `Yb = g^Xb mod p`, and finally returning the shared key `K = Yb^Xa mod p` (which must equal `Ya^Xb mod p`). The function should accept `p`, `Xa`, `Xb` as inputs and return the shared key as a `long long`. The function must also be able to handle the case where a primitive root does not exist (which happens when `p` is not prime or when `p` is 2 or 4); in such a case, return -1. Your solution must implement the modular exponentiation yourself (do not use external libraries) and must correctly factor `p-1` to test for primitiveness.
#include <cassert>

int main() {
    // Known prime 453371 (from snippet) — test that shared secret is consistent
    long long p = 453371;
    long long Xa = 12345;
    long long Xb = 67890;
    long long K1 = dhSharedSecret(p, Xa, Xb);
    // If the function returns -1 for any reason, test would fail; but for a prime >4 it should work
    assert(K1 != -1);
    assert(K1 == dhSharedSecret(p, Xb, Xa)); // symmetry

    // Small prime 7: primitive root 3, test known values manually
    // g=3, Xa=2 => Ya=9 mod 7 =2, Xb=3 => Yb=27 mod7=6, K=6^2 mod7=36 mod7=1
    assert(dhSharedSecret(7, 2, 3) == 1);

    // p=2 has no primitive root -> -1
    assert(dhSharedSecret(2, 1, 1) == -1);

    // p=4 (composite-like special) -> -1
    assert(dhSharedSecret(4, 1, 1) == -1);

    // Composite p=8 -> -1
    assert(dhSharedSecret(8, 1, 1) == -1);

    // Private key out of range -> -1
    assert(dhSharedSecret(7, 0, 1) == -1);
    assert(dhSharedSecret(7, 7, 1) == -1);

    // Larger prime 97: known primitive root 5, test with Xa=1, Xb=1 -> K = 5^(1*1) mod97 = 5
    assert(dhSharedSecret(97, 1, 1) == 5);

    // Same private keys give same result
    assert(dhSharedSecret(97, 42, 42) == dhSharedSecret(97, 42, 42));

    return 0;
}
#include <vector>
#include <cmath>

// Fast modular exponentiation: returns (base^exp) % mod.
static long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Simple primality test for p <= 1e9 (trial division up to sqrt).
static bool isPrime(long long n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Compute distinct prime factors of n, store in vector.
static std::vector<long long> distinctPrimeFactors(long long n) {
    std::vector<long long> factors;
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

// Given a prime p and private keys Xa, Xb (each 1 <= key < p),
// returns the Diffie-Hellman shared secret K = g^(Xa*Xb) mod p,
// where g is a primitive root of p. Returns -1 if no primitive root exists or inputs invalid.
long long dhSharedSecret(long long p, long long Xa, long long Xb) {
    // Validate p and keys
    if (!isPrime(p) || p <= 2) return -1; // no primitive root for p=2, composites
    if (p == 4) return -1; // p=4 has no primitive root
    if (Xa <= 0 || Xa >= p || Xb <= 0 || Xb >= p) return -1;

    // Factor p-1
    std::vector<long long> factors = distinctPrimeFactors(p - 1);

    // Find primitive root g
    long long g = -1;
    for (long long candidate = 2; candidate < p; ++candidate) {
        bool ok = true;
        for (long long q : factors) {
            if (modPow(candidate, (p - 1) / q, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok && modPow(candidate, p - 1, p) == 1) {
            g = candidate;
            break;
        }
    }
    if (g == -1) return -1; // should not happen for valid prime >4

    // Compute public keys and shared secret
    long long Ya = modPow(g, Xa, p);
    long long Yb = modPow(g, Xb, p);
    long long Ka = modPow(Yb, Xa, p);
    long long Kb = modPow(Ya, Xb, p);
    // They should be equal; return Ka
    return (Ka == Kb) ? Ka : -1; // if mismatch, error
}
// The core algorithm follows these steps:  
// 1. **Check prime-ness of `p`** (simple trial division up to `sqrt(p)`). If `p` is not prime, or if `p` is 2 or 4 (which lack primitive roots), return -1.  
// 2. **Factor `p-1`** into its distinct prime factors. We do this by trial division from 2 upward, storing unique factors in an array.  
// 3. **Find a primitive root**: For each candidate `g` from 2 upward, test if `g` is a primitive root by checking that `g^( (p-1)/q ) mod p != 1` for every distinct prime factor `q` of `p-1`. Also ensure `g^(p-1) mod p == 1` (which will hold by Fermat's little theorem if `p` is prime, but we verify anyway). The first `g` that satisfies all conditions is a primitive root.  
// 4. **Compute public keys**: Using fast modular exponentiation (binary exponentiation), compute `Ya = g^Xa mod p` and `Yb = g^Xb mod p`.  
// 5. **Compute shared secret**: Compute `K = Yb^Xa mod p` (and optionally verify `K == Ya^Xb mod p`). Return `K`.  
//
// **Edge cases**: If `p` is 2 or 4, no primitive root exists because the multiplicative group is not cyclic for those moduli; return -1. Also, if `Xa` or `Xb` are not in `[1, p-1]` or are zero, the algorithm may still work mathematically but the shared secret could degenerate; to be safe, if any private key is not in the range `1 ≤ key < p` return -1.  
//
// **Time complexity**:  
// - Prime check: O(√p)  
// - Factorization of p-1: O(√(p-1))  
// - Finding primitive root: For each candidate g (at most around a few small numbers), testing requires O(k log p) multiplications where k is the number of distinct prime factors (at most O(log p)).  
// - Two modular exponentiations for public keys and one for shared secret: each O(log Xa) or O(log Xb) multiplications.  
// Thus total is roughly O(√p + log²p) in practice, but for very large p this is not efficient. Given typical p in such exercises (like 453371 in the snippet), it is fine.  
//
// **Space complexity**: O(log p) for the prime factor array.
