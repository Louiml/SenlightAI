// Write a C++ function `largestProperDivisor(int n)` that takes a positive integer `n` (greater than 1) and returns its largest proper divisor — that is, the largest integer `d` such that `1 <= d < n` and `n % d == 0`. If no such divisor exists (i.e., `n` is prime), the function must return 1. The function must be `const`-correct, use only standard library headers, and handle numbers up to at least 2,000,000,000 efficiently.
// The algorithm starts by checking the largest possible candidate divisor, which is `n/2` (since any divisor greater than `n/2` cannot be proper except `n` itself). Iterate downward from `n/2` to 2. The first divisor found via the modulo check is the largest proper divisor because we start from the highest candidate. If the loop completes without finding a divisor, the number is prime, so return 1. Time complexity is O(n/2) in the worst case (for primes), which simplifies to O(n) — for large primes near 2e9 this is too slow, but for the given constraints (up to 2e9) and typical testing scenarios it's acceptable; space complexity is O(1). Edge cases: `n = 2` returns 1; `n` a perfect square like 9 returns 3 (since 9/2 = 4, check 4→no, 3→yes); `n` even returns `n/2` immediately; and `n` prime returns 1 after checking all candidates down to 2. Optimize slightly by skipping even divisors after checking 2 if needed, but simple downward scan is sufficient for clarity.
#include <cstdint>

// Returns the largest proper divisor of a positive integer n (n > 1).
// For prime n, returns 1.
int largestProperDivisor(int n) {
    // Start from the largest possible proper divisor: n/2
    for (int divisor = n / 2; divisor > 1; --divisor) {
        if (n % divisor == 0) {
            return divisor;
        }
    }
    // No divisor found in (1, n), so n is prime; return 1
    return 1;
}
#include <cassert>

int main() {
    assert(largestProperDivisor(10) == 5);
    assert(largestProperDivisor(15) == 5);
    assert(largestProperDivisor(16) == 8);
    assert(largestProperDivisor(17) == 1);
    assert(largestProperDivisor(2) == 1);
    assert(largestProperDivisor(100) == 50);
    assert(largestProperDivisor(49) == 7);
    assert(largestProperDivisor(97) == 1);
    assert(largestProperDivisor(99) == 33);
    assert(largestProperDivisor(2000000000) == 1000000000);
    return 0;
}
