// Write a C++ function that takes three integers `a`, `b`, and `c` (all between 1 and 10^9) and returns the smallest positive multiple of `c` that lies within the inclusive range `[a, b]`. If no such multiple exists, return `-1`. The function must handle large values efficiently without iterating beyond the necessary number of steps, and must work correctly for cases where `a > b`, `c` is larger than the range, or the first multiple is far from `a`.

// The key insight is that instead of brute-force checking every multiple starting from `c`, we can compute the first multiple of `c` that is at least `a`. The smallest multiple of `c` greater than or equal to `a` is `((a + c - 1) / c) * c` using integer arithmetic. Let this candidate be `m`. If `m <= b`, then `m` is the answer; otherwise, there is no multiple of `c` in `[a, b]`, so return `-1`. This works because any multiple of `c` in `[a, b]` must be at least the first multiple ≥ `a`, and if that first one exceeds `b`, no later multiple can be within the range (since multiples are spaced by `c > 0`). Edge cases: if `a > b`, the formula still gives a candidate, but since it will be > b, it returns `-1`. If `c` is huge and `a` is also huge, the formula uses 64-bit arithmetic to avoid overflow (use `long long`). Time complexity is O(1), space complexity is O(1).

#include <cstdint>

// Returns the smallest multiple of c in [a, b], or -1 if none exists.
// Assumes a, b, c are positive integers up to 1e9.
long long smallestMultipleInRange(long long a, long long b, long long c) {
    // Compute the first multiple of c that is >= a.
    // Using integer ceiling division: (a + c - 1) / c * c.
    long long first = ((a + c - 1) / c) * c;
    return (first <= b) ? first : -1;
}

#include <cassert>

int main() {
    // Basic cases
    assert(smallestMultipleInRange(2, 10, 3) == 3);
    assert(smallestMultipleInRange(5, 6, 2) == 6);
    assert(smallestMultipleInRange(7, 9, 4) == 8);
    
    // No multiple exists
    assert(smallestMultipleInRange(10, 11, 5) == -1);
    assert(smallestMultipleInRange(1, 2, 10) == -1);
    
    // Exact boundary
    assert(smallestMultipleInRange(4, 4, 2) == 4);
    assert(smallestMultipleInRange(3, 3, 3) == 3);
    
    // a > b
    assert(smallestMultipleInRange(8, 5, 2) == -1);
    
    // Large values (up to 1e9)
    assert(smallestMultipleInRange(1000000000LL, 1000000000LL, 1LL) == 1000000000LL);
    assert(smallestMultipleInRange(999999999LL, 1000000000LL, 2LL) == 1000000000LL);
    assert(smallestMultipleInRange(1000000000LL, 1000000000LL, 3LL) == -1);
    
    // c=1 always returns a if a<=b, else -1
    assert(smallestMultipleInRange(1, 5, 1) == 1);
    assert(smallestMultipleInRange(6, 5, 1) == -1);
    
    return 0;
}
