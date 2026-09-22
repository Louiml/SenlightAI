// Write a C++ function named `isPrime` that takes an integer `n` and returns `true` if `n` is a prime number, and `false` otherwise. A prime number is a positive integer greater than 1 that has no positive divisors other than 1 and itself. The function must correctly handle edge cases including negative numbers, 0, and 1 (all of which are not prime). The function should determine primality by checking divisibility only up to the square root of `n`. Provide a self-contained implementation with appropriate `const` and type usage — the parameter should be passed by value as `const int n` (or simply `int n` with `const` applied inside), and the function should be efficient enough for inputs up to about 10^9.
The standard trial-division algorithm: first, handle non-positive numbers (≤1) — return `false` immediately. For any even number greater than 2, we can immediately return `false` (since 2 is the only even prime). Then loop from 3 up to `sqrt(n)` in steps of 2, checking if `n % i == 0`. If a divisor is found, return `false`. If no divisor is found, return `true`. Edge cases: `n=2` should return `true` (loop does not run); `n=0`, `1`, or any negative number return `false`; perfect squares like `49` are handled correctly because the loop goes up to `sqrt(49)=7` and `49%7==0` returns `false`. Time complexity is `O(sqrt(n))` in the worst case, but with the even-number skip, for an odd `n` it runs about `sqrt(n)/2` iterations. Space complexity is `O(1)`.
#include<cmath>

// Return true if n is a prime number, false otherwise.
bool isPrime(const int n) {
    if (n <= 1) return false;          // 0, 1, and negatives are not prime
    if (n == 2) return true;           // 2 is the only even prime
    if (n % 2 == 0) return false;      // any other even number is composite

    // Check odd divisors up to sqrt(n)
    const int limit = static_cast<int>(std::sqrt(n));
    for (int i = 3; i <= limit; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}
#include <cassert>

int main() {
    assert(isPrime(0) == false);
    assert(isPrime(1) == false);
    assert(isPrime(-7) == false);
    assert(isPrime(2) == true);
    assert(isPrime(3) == true);
    assert(isPrime(4) == false);
    assert(isPrime(17) == true);
    assert(isPrime(100) == false);
    assert(isPrime(97) == true);
    assert(isPrime(999983) == true);  // large prime
    return 0;
}
