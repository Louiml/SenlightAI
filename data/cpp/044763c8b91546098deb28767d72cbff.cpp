/*
Write a C++ function named `isPrime` that takes a positive integer `n` (with `n >= 2`) and returns a boolean indicating whether `n` is a prime number. The function must correctly decide primality for all integers from 2 up to 1,000,000. Use an optimized trial-division approach that checks divisibility only up to the square root of `n`. The function should be `const`-correct (it takes its argument by value and does not modify any external state) and must be self-contained with all necessary includes. The function should return `true` for prime numbers (e.g., 2, 3, 5, 7, 11) and `false` for composite numbers (e.g., 4, 6, 8, 9, 10, 100). Edge cases: `n=2` is prime; `n` being even but greater than 2 must be handled efficiently (optionally skip even divisors after checking `2`). The solution must not use any external library beyond the standard C++ headers.
*/

#include <cstdint>

// Returns true if n is a prime number, false otherwise.
// Precondition: n >= 2.
bool isPrime(const int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false; // even numbers > 2 are composite

    // Check odd divisors up to sqrt(n)
    for (int d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

#include <cassert>

int main() {
    // Basic primes
    assert(isPrime(2) == true);
    assert(isPrime(3) == true);
    assert(isPrime(5) == true);
    assert(isPrime(7) == true);
    assert(isPrime(11) == true);
    assert(isPrime(13) == true);

    // Composites
    assert(isPrime(4) == false);
    assert(isPrime(6) == false);
    assert(isPrime(8) == false);
    assert(isPrime(9) == false);
    assert(isPrime(10) == false);
    assert(isPrime(100) == false);

    // Larger primes and composites near the limit
    assert(isPrime(999983) == true);  // known prime
    assert(isPrime(1000000) == false); // 1,000,000 = 2^6 * 5^6

    // Edge: even large number
    assert(isPrime(100000) == false);
    assert(isPrime(99991) == true); // known prime

    // Square of a prime
    assert(isPrime(49) == false); // 7*7
    assert(isPrime(121) == false); // 11*11
}

// The most straightforward approach is to loop from `2` to `sqrt(n)` and check if any value divides `n` evenly. If a divisor is found, the number is composite. If no divisor is found up to `sqrt(n)`, then `n` is prime. This works because any composite number `n` has at least one divisor `d` such that `2 <= d <= sqrt(n)`. The time complexity is O(√n), which for `n` up to 1,000,000 means at most 1,000 iterations, which is efficient. Space complexity is O(1) as only a few integer variables are used. Edge cases: `n=2` is prime (the loop starts at 2 and `sqrt(2) ≈ 1.41`, so the loop doesn't run, and we return true). For even `n > 2`, we can immediately return false after checking divisibility by 2, which cuts the loop in half (check odd divisors only). To avoid floating-point issues with `sqrt`, we use `c * c <= n` in the loop condition instead of `c <= sqrt(n)`. This also avoids including `n` itself as a divisor (the loop stops before `c` exceeds the square root). The initial code snippet in the prompt had a subtle bug: it counted divisors from `1` to `sqrt(n)` and then checked if `ndiv >= 2`, but it didn't count the divisor `n` itself, so it correctly identified primes but was less efficient and slightly confusing. Our solution will be cleaner and directly return a boolean.
