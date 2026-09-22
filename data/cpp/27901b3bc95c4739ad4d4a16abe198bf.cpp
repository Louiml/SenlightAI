// Write a C++ function named `isPrime` that takes a positive integer `n` and returns `true` if `n` is a prime number, and `false` otherwise. A prime number is a natural number greater than 1 that has no positive divisors other than 1 and itself. Your function should handle edge cases (e.g., 0, 1, 2, 3, and larger numbers) correctly and efficiently by checking divisibility using the optimized trial-division method (only test divisors up to the square root of `n`, and skip even divisors after 2). Do not include a `main` function in your solution—only provide the function definition.
#include <cassert>

// Test driver for the isPrime function
int main() {
    // Edge cases: 0 and 1 are not prime, 2 is prime
    assert(isPrime(0) == false);
    assert(isPrime(1) == false);
    assert(isPrime(2) == true);

    // Small numbers
    assert(isPrime(3) == true);
    assert(isPrime(4) == false);
    assert(isPrime(5) == true);
    assert(isPrime(6) == false);
    assert(isPrime(7) == true);
    assert(isPrime(9) == false);

    // Larger known primes and composites
    assert(isPrime(97) == true);
    assert(isPrime(100) == false);
    assert(isPrime(9973) == true); // a known prime
    assert(isPrime(1000000) == false);

    // Square of a prime (divisor exactly at sqrt)
    assert(isPrime(49) == false); // 7*7

    return 0;
}
#include <cmath> // for std::sqrt

// Returns true if n is prime, false otherwise.
// Handles edge cases: n < 2 is not prime; 2 is prime; skips even divisors after 2.
bool isPrime(const int n) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false; // even numbers > 2 are not prime
    }
    // Check only odd divisors up to sqrt(n)
    const int limit = static_cast<int>(std::sqrt(n));
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
// The standard trial-division algorithm determines primality by checking if `n` is divisible by any integer from 2 up to `sqrt(n)`. If any divisor is found, `n` is not prime; otherwise, it is prime. Important edge cases: numbers less than 2 (0 and 1) are not prime; 2 is the only even prime, so after checking 2 separately, we can skip all other even divisors, reducing iterations by half. For an odd `n` > 2, we test only odd divisors from 3 up to `sqrt(n)`. The time complexity is O(√n) in the worst case (when `n` is prime), and the space complexity is O(1) as only a few scalar variables are used. The function should use `const` for its parameter where appropriate and be robust for any positive integer input.
