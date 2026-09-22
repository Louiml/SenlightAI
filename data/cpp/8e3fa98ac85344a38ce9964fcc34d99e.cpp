/*
Write a C++ function that takes a positive integer `N` and returns the sum of all integers from 1 to `N` inclusive. The function should be named `sumFromOneToN` and must handle the case where `N` is large (up to 10^9) without overflowing using a 32-bit integer; use a 64-bit integer for the result. The function should not use loops but compute the result using the closed-form formula `N*(N+1)/2` to ensure constant time performance. The input is guaranteed to be a valid positive integer, but you should still handle `N=1` correctly (returning 1) and `N=0` (returning 0, though not expected). The function must be self-contained, using only standard headers, and be suitable for direct testing.
*/
#include <cstdint>

// Returns the sum of integers from 1 to N inclusive.
// Uses the closed-form arithmetic series formula to achieve O(1) time.
// N is a non-negative integer; result is returned as int64_t to avoid overflow.
int64_t sumFromOneToN(int N) {
    if (N <= 0) {
        return 0;
    }
    // Use 64-bit arithmetic to safely compute N*(N+1)/2 without overflow.
    int64_t n = N;
    if (n % 2 == 0) {
        return (n / 2) * (n + 1);
    } else {
        return n * ((n + 1) / 2);
    }
}
#include <cassert>

int main() {
    // Basic cases
    assert(sumFromOneToN(1) == 1);
    assert(sumFromOneToN(2) == 3);
    assert(sumFromOneToN(3) == 6);
    assert(sumFromOneToN(10) == 55);
    // Edge cases
    assert(sumFromOneToN(0) == 0);
    assert(sumFromOneToN(100) == 5050);
    // Large N to ensure no overflow and correct result
    assert(sumFromOneToN(1000000000) == 500000000500000000LL);
    assert(sumFromOneToN(999999999) == 499999999500000000LL);
    // Odd N edge
    assert(sumFromOneToN(5) == 15);
    assert(sumFromOneToN(7) == 28);
    // Even N edge
    assert(sumFromOneToN(4) == 10);
    assert(sumFromOneToN(8) == 36);
}
// The problem asks for the sum of an arithmetic series from 1 to N. The naive loop approach (as in the snippet) works for small N but is O(N) time. For N up to 10^9, that would be too slow. Instead, apply the well-known formula: sum = N*(N+1)/2. Using 64-bit integers (`long long`) avoids overflow because N is at most 10^9, so N*(N+1) is at most ~10^18, which fits in a signed 64-bit range (max ~9.22e18). The division by 2 can be done after multiplication, but to be safe with integer arithmetic, we can do: if N is even, compute (N/2)*(N+1); else N*((N+1)/2). This avoids any potential overflow even if N were near the limits of 64-bit. Edge cases: N=0 returns 0; N=1 returns 1. Time complexity is O(1), space complexity O(1).
