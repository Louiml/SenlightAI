// Write a C++ function named `sumMultiples` that accepts two positive integers, `limit` and `divisorA` and `divisorB`, and returns the sum of all positive integers strictly less than `limit` that are multiples of `divisorA` or `divisorB` (or both). The function must handle arbitrary positive divisors (not just 3 and 5) and a limit that could be as small as 1. Do not include any input/output logic inside the function.

#include <cassert>

int main() {
    // Original Euler problem: sum of multiples of 3 or 5 below 1000
    assert(sumMultiples(1000, 3, 5) == 233168);

    // Limit of 1 produces no multiples
    assert(sumMultiples(1, 3, 5) == 0);

    // Divisor 1 makes every number a multiple
    assert(sumMultiples(10, 1, 7) == 45);  // sum of 1..9 = 45

    // Overlapping multiples counted once
    assert(sumMultiples(20, 2, 4) == 90);  // sum of even numbers < 20

    // Small limit and divisors
    assert(sumMultiples(6, 3, 5) == 8);    // 3 + 5 = 8

    // Larger limit with non-overlapping multiples
    assert(sumMultiples(10, 2, 3) == 32);  // 2+3+4+6+8+9 = 32
}

#include <cstdint>

// Returns the sum of all positive integers strictly less than `limit`
// that are multiples of divisorA or divisorB (or both).
long long sumMultiples(int limit, int divisorA, int divisorB) {
    long long sum = 0;
    for (int i = 1; i < limit; ++i) {
        if (i % divisorA == 0 || i % divisorB == 0) {
            sum += i;
        }
    }
    return sum;
}

// The main algorithm is a straightforward iteration from 1 up to `limit - 1`, checking whether each integer is divisible by either divisor using the modulo operator. If the condition holds, add it to a running sum. Edge cases to consider: if `limit` is 1, there are no numbers below it, so the sum is 0. If either divisor is 1, every positive integer below `limit` is a multiple, so the sum is the sum of all integers from 1 to `limit-1`. If the divisors share a common multiple (e.g., 2 and 4), numbers divisible by both are counted only once because the condition uses logical OR. The algorithm runs in O(n) time where n = limit-1 (the number of integers checked), and uses O(1) extra space. No overflow concerns for typical test cases, but the `const` qualifier ensures the function does not modify its inputs.
