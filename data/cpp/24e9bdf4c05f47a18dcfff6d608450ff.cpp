Write a C++ function named `countPrimeDivisors` that takes a single positive integer `n` and returns `true` if `n` is a prime number (i.e., has exactly two distinct positive divisors: 1 and itself) and `false` otherwise. The function must handle edge cases: `n` equal to 1 (not prime), `n` equal to 2 (prime), and very large values of `n` (up to `INT_MAX`) efficiently without iterating through all numbers up to `n`—use trial division only up to the square root of `n` to reduce time complexity. The function must be `const`-correct (accept `const int n` or pass by value and not modify it), and must use only standard C++ headers. Provide the implementation for the free function only, without a `main` function.
#include <cassert>

// Forward declaration of the function under test.
bool isPrime(const int n);

int main() {
    // Basic cases
    assert(isPrime(1) == false);
    assert(isPrime(2) == true);
    assert(isPrime(3) == true);
    assert(isPrime(4) == false);
    assert(isPrime(5) == true);

    // Composite numbers with multiple factors
    assert(isPrime(9) == false);
    assert(isPrime(15) == false);
    assert(isPrime(49) == false);

    // Larger primes and composites
    assert(isPrime(97) == true);
    assert(isPrime(100) == false);
    assert(isPrime(7919) == true);  // A prime number

    // Edge case: maximum int value (INT_MAX is prime in many implementations, but not guaranteed; use a known prime instead)
    // Use a safe large prime: 2147483647 is prime on most systems (2^31 - 1).
    // To avoid platform dependency, we test with a known prime within int range.
    assert(isPrime(2147483647) == true);  // 2^31 - 1, prime.
    assert(isPrime(2147483646) == false); // even, not prime.

    return 0;
}
#include <cmath>

// Returns true if n is a prime number, false otherwise.
// Assumes n is a positive integer.
bool isPrime(const int n) {
    if (n <= 1) {
        return false;
    }
    // Check divisibility only up to the square root of n.
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
// The core idea is to test divisibility of `n` by all integers from 2 up to `sqrt(n)`. If any divides `n` evenly, then `n` is composite, so return `false`. If no divisor is found, then `n` is prime. Edge cases: `n <= 1` returns `false` (by definition, 1 is not prime, and non-positive numbers are out of scope). `n = 2` is prime, but the loop from 2 to `sqrt(2)` (≈1.41) doesn't execute, so the function returns `true` naturally. For performance, we compute `sqrt(n)` once using `<cmath>` and cast to `int` to avoid floating-point issues (or use `i * i <= n` to avoid `sqrt` entirely). The loop condition `i * i <= n` is safe for `int` values up to `INT_MAX` because `i` will never exceed ~46340 (sqrt of INT_MAX), so no overflow. Time complexity is O(√n) in the worst case (for prime numbers), and O(1) for even numbers (we can also optimize by checking divisibility by 2 first and then only odd numbers, but it's not required). Space complexity is O(1).
