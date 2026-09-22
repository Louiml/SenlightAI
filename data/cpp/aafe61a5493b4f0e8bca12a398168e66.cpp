// Write a C++ function named `modularExponentWithPrimeCheck` that takes three integers: a base `a`, a prime modulus `p`, and a non-negative exponent `degree`. The function should return the result of \(a^{degree} \mod p\) if and only if `p` is prime and `a` is not divisible by `p`. If `p` is not prime or `a` is divisible by `p`, the function should return `-1` as a sentinel value indicating an invalid operation. You must implement your own primality test (do not use built-in prime functions) and compute the modular exponentiation using iterative multiplication (not the fast exponentiation method). The exponent is guaranteed to be non-negative. The function should handle negative bases correctly (the result should be the mathematical modulo, i.e., between 0 and p-1 for valid cases, and -1 for invalid cases).
// The solution requires two main steps: a primality check and modular exponentiation. For the primality check, we test divisibility from 2 up to the square root of `p`. If `p` is less than 2, it is not prime. We also need to check that `a % p` is not zero (i.e., `a` is not divisible by `p`). For the modular exponentiation, we iterate from 1 to `degree` inclusive, multiplying the current result by `a` and taking modulo `p` at each step. This ensures the result stays within [0, p-1]. Edge cases: if `p` is not prime, return -1. If `a % p == 0` (including the case where `a` is a multiple of `p`), return -1. If `degree` is 0, the result is 1 (since any non-zero number to the 0th power is 1). Negative bases are handled by C++'s modulo operator; however, to ensure a mathematically correct non-negative result, we add `p` to the product before taking modulo if the product is negative, or equivalently we can adjust after each multiplication. Since we are multiplying and taking modulo each iteration, and `a` might be negative, we do `((result * a) % p + p) % p` to get a positive result. Alternatively, we can use `(result * a) % p` and if negative add `p` afterwards, but doing it inside the loop is cleaner. Time complexity: primality test runs in `O(sqrt(p))`, and exponentiation loop runs in `O(degree)` time, so total `O(sqrt(p) + degree)`. Space complexity is `O(1)`.
#include <cmath>

// Returns (a^degree) mod p if p is prime and a is not divisible by p, otherwise -1.
// Implements a naive primality test and iterative modular exponentiation.
int modularExponentWithPrimeCheck(int a, int p, int degree) {
    // Helper lambda for primality test (local to function for self-containment)
    auto isPrime = [](int n) -> bool {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i <= std::sqrt(n); i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    };

    // Invalid if p is not prime or a is divisible by p
    if (!isPrime(p) || a % p == 0) {
        return -1;
    }

    // Handle non-negative exponent
    if (degree == 0) {
        return 1;
    }

    int result = 1;
    for (int i = 1; i <= degree; ++i) {
        // Multiply and take modulo, ensuring non-negative result
        result = ((static_cast<long long>(result) * a) % p + p) % p;
    }
    return result;
}
#include <cassert>

int main() {
    // Basic valid case: 2^3 mod 5 = 8 mod 5 = 3
    assert(modularExponentWithPrimeCheck(2, 5, 3) == 3);
    // Exponent zero: 3^0 mod 7 = 1
    assert(modularExponentWithPrimeCheck(3, 7, 0) == 1);
    // Base is a multiple of p -> invalid
    assert(modularExponentWithPrimeCheck(6, 3, 2) == -1);
    // p is not prime -> invalid
    assert(modularExponentWithPrimeCheck(2, 4, 2) == -1);
    // Negative base: -2^3 mod 5 = (-8) mod 5 = 2 (since -8+10=2)
    assert(modularExponentWithPrimeCheck(-2, 5, 3) == 2);
    // Larger exponent: 3^4 mod 11 = 81 mod 11 = 4
    assert(modularExponentWithPrimeCheck(3, 11, 4) == 4);
    // p = 2 (smallest prime) and a=1: 1^5 mod 2 = 1
    assert(modularExponentWithPrimeCheck(1, 2, 5) == 1);
    // Base zero with prime p: 0 mod p is 0 but a%p==0 -> invalid
    assert(modularExponentWithPrimeCheck(0, 101, 3) == -1);
    // Negative exponent edge: not expected, but function assumes non-negative; test positive only
    // Check that result stays in range [0,p-1] for large exponent
    assert(modularExponentWithPrimeCheck(7, 13, 10) >= 0 && modularExponentWithPrimeCheck(7, 13, 10) < 13);
}
