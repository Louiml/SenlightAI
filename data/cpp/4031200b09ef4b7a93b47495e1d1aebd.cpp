Write a C++ function that takes a positive integer `n` and returns the least common multiple (LCM) of all integers from 1 to `n` inclusive. The function must use a loop to iteratively compute the LCM by combining the current result with each integer `i` in the range, using the formula `lcm(a, b) = a / gcd(a, b) * b` to avoid overflow. The result can be large, so the function should return `unsigned long long` (which is at least 64 bits, sufficient for `n` up to 42 in typical environments). The function must be self-contained, use `const` correctly for any input parameters, and include necessary headers. The task is to implement the function `lcmOfRange(unsigned int n)` that returns the value, with `n` guaranteed to be between 1 and 50 (but behavior for larger values up to 42 must be correct; beyond that, overflow may occur, but such inputs are outside the guaranteed range).
The core algorithm is to start with `result = 1` and for each integer `i` from 1 to `n`, update `result = lcm(result, i)`. For each step, compute the greatest common divisor (GCD) using Euclid's algorithm (iterative, with `%`), then compute `lcm = (result / gcd) * i` to avoid intermediate overflow compared to `(result * i) / gcd`. Because `result` is always a multiple of the GCD, the division is exact. This process yields the LCM of all numbers from 1 to `n`. Edge cases: `n = 1` returns 1 (loop runs from 1 to 1). For `n = 0` or negative input, the behavior is undefined, but the task specifies `n` is positive. Overflow is a concern for large `n`; for `n` up to 42, the result fits in 64-bit unsigned (the 42nd primorial-like value is ~2.19e18 < 1.8e19 max for `unsigned long long`). Time complexity is O(n log M) where M is the maximum intermediate result, but in practice for n ≤ 50 it's effectively O(n) with constant-time GCD steps (each GCD is O(log(result)) which is at most ~63 iterations). Space complexity is O(1). The solution must not use `main`; only the free function.
#include <cstdint>

// Compute greatest common divisor of two non-negative integers using Euclid's algorithm.
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Return the least common multiple of all integers from 1 to n inclusive.
// Assumes n >= 1. The result fits in 64-bit for n up to 42.
unsigned long long lcmOfRange(const unsigned int n) {
    unsigned long long result = 1;
    for (unsigned int i = 1; i <= n; ++i) {
        result = (result / gcd(result, i)) * i;
    }
    return result;
}
#include <cassert>

// Forward declaration of the solution function (if not in same translation unit).
unsigned long long lcmOfRange(const unsigned int n);

int main() {
    assert(lcmOfRange(1) == 1ULL);
    assert(lcmOfRange(2) == 2ULL);
    assert(lcmOfRange(3) == 6ULL);
    assert(lcmOfRange(4) == 12ULL);
    assert(lcmOfRange(5) == 60ULL);
    assert(lcmOfRange(6) == 60ULL);
    assert(lcmOfRange(7) == 420ULL);
    assert(lcmOfRange(10) == 2520ULL);
    assert(lcmOfRange(20) == 232792560ULL);
    assert(lcmOfRange(21) == 232792560ULL);
}
