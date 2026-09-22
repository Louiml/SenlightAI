// Write a C++ function named `classifyPrime` that takes a positive integer `n` as input and returns a `bool` indicating whether `n` is a prime number. The function must handle the edge case where `n` is less than 2 (returning `false`), and correctly identify all prime numbers greater than or equal to 2. The function should be efficient enough to handle numbers up to at least \(10^9\) within reasonable time, using trial division only up to the square root of `n`. Do not use external libraries beyond the standard ones; your implementation must be self-contained.

#include <cassert>

// Forward declaration of the solution function (already defined elsewhere)
bool classifyPrime(int n);

int main() {
    // Edge cases
    assert(classifyPrime(0) == false);
    assert(classifyPrime(1) == false);
    assert(classifyPrime(2) == true);
    assert(classifyPrime(3) == true);
    // Small composites
    assert(classifyPrime(4) == false);
    assert(classifyPrime(9) == false);
    assert(classifyPrime(15) == false);
    // Larger known primes and composites
    assert(classifyPrime(97) == true);
    assert(classifyPrime(100) == false);
    assert(classifyPrime(9973) == true);
    assert(classifyPrime(1000000007) == true); // large prime
    assert(classifyPrime(1000000008) == false); // even composite
    return 0;
}

#include <cmath>

// Returns true if n is a prime number, false otherwise.
// Handles n < 2 by returning false.
bool classifyPrime(int n) {
    if (n < 2) {
        return false;
    }
    // Check divisibility from 2 up to sqrt(n)
    for (int divisor = 2; divisor <= static_cast<int>(std::sqrt(n)); ++divisor) {
        if (n % divisor == 0) {
            return false;
        }
    }
    return true;
}

// The simplest correct approach is trial division: for a given `n`, if `n` is less than 2, it is not prime. Otherwise, check divisibility from `2` up to `sqrt(n)`. If any integer divides `n` evenly, `n` is composite; if none do, it is prime. The key optimizations are: (1) only iterate up to `sqrt(n)` (since any factor larger than that would have a corresponding factor smaller than it), and (2) for `n` that are even or divisible by 3 early, we can quickly return `false`. However, a straightforward loop that checks all integers from 2 to `sqrt(n)` is sufficient for the constraints, as `sqrt(10^9)` is about 31623, which is small.  
// Edge cases: `n = 1` and `n = 0` (if allowed) return `false`; `n = 2` and `n = 3` are prime; `n` being a perfect square (e.g., 25) must be correctly identified as composite.  
// Time complexity: \(O(\sqrt{n})\) per call, which is fine. Space complexity: \(O(1)\) auxiliary, not counting input.
