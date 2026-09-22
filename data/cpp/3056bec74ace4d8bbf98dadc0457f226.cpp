/*
Write a standalone C++ function named `findLargestFactor` that takes a single positive integer `z` (with `z >= 1`) and returns the largest integer `i` such that `1 <= i < z` and `i` divides `z` evenly. The function must be efficient for large inputs, avoiding a naive linear scan when possible. The solution must handle the edge case where `z == 1` by returning `1` (since the largest factor of 1 less than 1 is technically undefined, but the source code treats it as 1). The function should be self-contained with appropriate headers and use `const` and correct types. Do not include a `main` function.
*/

#include <cmath>

// Returns the largest integer i (1 <= i < z) that divides z evenly.
// For z == 1, returns 1 (as per the original fft5d logic).
// Uses trial division to find the smallest prime factor, then divides z by it.
int findLargestFactor(int z) {
    if (z == 1) {
        return 1;
    }
    if (z == 2) {
        return 1; // Largest proper divisor of 2 is 1
    }
    // If even, the largest proper divisor is z/2
    if (z % 2 == 0) {
        return z / 2;
    }
    // Find the smallest odd divisor that divides z
    // We only need to check up to sqrt(z)
    int limit = static_cast<int>(std::sqrt(static_cast<double>(z)));
    for (int i = 3; i <= limit; i += 2) {
        if (z % i == 0) {
            return z / i; // Since i is the smallest factor, z/i is the largest
        }
    }
    // z is prime (or 1 handled above), so the only proper divisor is 1
    return 1;
}

#include <cassert>

int main() {
    // Edge cases
    assert(findLargestFactor(1) == 1);
    assert(findLargestFactor(2) == 1);
    assert(findLargestFactor(3) == 1); // prime
    assert(findLargestFactor(5) == 1); // prime

    // Even numbers
    assert(findLargestFactor(4) == 2);
    assert(findLargestFactor(6) == 3);
    assert(findLargestFactor(8) == 4);
    assert(findLargestFactor(10) == 5);
    assert(findLargestFactor(12) == 6);

    // Odd composite numbers
    assert(findLargestFactor(9) == 3);
    assert(findLargestFactor(15) == 5);
    assert(findLargestFactor(21) == 7);
    assert(findLargestFactor(25) == 5);
    assert(findLargestFactor(27) == 9);

    // Large composite
    assert(findLargestFactor(100) == 50);
    assert(findLargestFactor(121) == 11);
    assert(findLargestFactor(143) == 13); // 143 = 11*13

    // Prime large number (should return 1)
    assert(findLargestFactor(97) == 1);
    assert(findLargestFactor(101) == 1);
}

// The algorithm should first handle the trivial case: if `z == 1`, return `1` immediately. For `z > 1`, the largest proper divisor (other than itself) is either `z/2` if `z` is even, or the largest divisor found by starting from `z/2` and decrementing until a divisor is found. However, a more efficient approach leverages the fact that if `z` is even, the largest proper divisor is `z/2`; if odd, we only need to check odd candidates. Even better, we can note that the largest proper divisor is `z / smallest_prime_factor(z)`, but to keep it simple and robust, we can iterate downward from `z/2` but skip even candidates if `z` is odd, reducing checks. For large `z`, this is still O(z) in the worst case (prime numbers), but for typical cases it terminates quickly. An optimization is to use the fact that if `z` is composite, the largest proper divisor is at least `sqrt(z)`. We can find the smallest prime factor via trial division up to sqrt(z), then divide `z` by it. This yields O(sqrt(z)) time and O(1) space. Edge cases: `z = 1` returns `1`; `z = 2` returns `1` (since 1 divides 2); `z = prime` returns `1`. The time complexity for the optimized version is O(sqrt(z)) and space O(1).
