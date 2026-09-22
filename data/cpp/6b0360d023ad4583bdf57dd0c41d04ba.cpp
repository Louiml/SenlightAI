// Given a positive integer `n` (where `1 < n < 65000`), write a C++ function `bool isCarmichaelNumber(int n)` that returns `true` if `n` is a Carmichael number, and `false` otherwise. A Carmichael number is a composite number `n` that satisfies the congruence `b^n ≡ b (mod n)` for every integer `b` such that `1 ≤ b < n`. In other words, all bases from 2 to `n-1` must satisfy `mod(b, n, n) == b`, where `mod` is modular exponentiation. The function should first determine whether `n` is composite (a Carmichael number must be composite) and then check the congruence condition for all bases. The analysis should consider that Carmichael numbers are relatively rare; for example, 561 is the smallest one. The implementation should not use the `bool` array from the snippet but instead compute primality dynamically (e.g., trial division) to keep the function self-contained. The function must be efficient enough for repeated calls up to 65000. Test cases should verify known Carmichael numbers (e.g., 561, 1105, 1729, 2465, 2821, 6601) and reject primes and ordinary composites.
#include <cassert>

int main() {
    // Known Carmichael numbers (composite and satisfying the condition).
    assert(isCarmichaelNumber(561) == true);
    assert(isCarmichaelNumber(1105) == true);
    assert(isCarmichaelNumber(1729) == true);
    assert(isCarmichaelNumber(2465) == true);
    assert(isCarmichaelNumber(2821) == true);
    assert(isCarmichaelNumber(6601) == true);

    // Ordinary composite numbers that are not Carmichael.
    assert(isCarmichaelNumber(4) == false);
    assert(isCarmichaelNumber(15) == false);
    assert(isCarmichaelNumber(27) == false);
    assert(isCarmichaelNumber(100) == false);

    // Primes are never Carmichael.
    assert(isCarmichaelNumber(2) == false);
    assert(isCarmichaelNumber(3) == false);
    assert(isCarmichaelNumber(17) == false);
    assert(isCarmichaelNumber(7919) == false); // a prime

    // Edge case: 1 is not Carmichael.
    assert(isCarmichaelNumber(1) == false);
}
#include <cmath>

// Fast modular exponentiation: computes (base^exp) % mod.
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Checks whether a positive integer n is a Carmichael number.
bool isCarmichaelNumber(int n) {
    if (n < 2) {
        return false;
    }
    
    // Check if n is prime (Carmichael numbers must be composite).
    bool isPrime = true;
    if (n < 2) isPrime = false;
    else {
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                isPrime = false;
                break;
            }
        }
    }
    if (isPrime) {
        return false;
    }

    // Verify congruence b^n ≡ b (mod n) for all 2 ≤ b ≤ n-1.
    for (int b = 2; b < n; ++b) {
        if (modPow(b, n, n) != b) {
            return false;
        }
    }
    return true;
}
// The solution has two main parts: primality testing and checking the Fermat-like congruence for all bases from 2 to n-1. First, determine if `n` is prime using trial division up to `sqrt(n)`. If prime, return `false` immediately because Carmichael numbers are composite. If composite, iterate `b` from 2 to `n-1` and compute `b^n mod n` using fast modular exponentiation (binary exponentiation). If any `b^n mod n` is not equal to `b`, return `false`. If all pass, return `true`. Edge cases: `n` must be at least 2 (the problem says positive, but the snippet used `n>0`; for `n=1` it is neither prime nor composite, but Carmichael requires composite, so return `false`). The trick is that Carmichael numbers pass the congruence for all bases despite being composite. Time complexity: For a given `n`, primality test is O(sqrt(n)), and the loop over bases is O(n * log n) for modular exponentiation (each exponentiation is O(log n) multiplications). So total O(n log n) per call. Space is O(1). This is acceptable for n < 65000 because the loop for bases is at most about 65000 iterations, each with about 16 multiplications, which is fine.
