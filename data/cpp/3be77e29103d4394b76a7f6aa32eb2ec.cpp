Write a C++ function `long long gcdOfTwoNumbers(long long a, long long b)` that computes and returns the greatest common divisor (GCD) of two given non-negative integers `a` and `b`. The input may be up to \(10^{18}\), and both `a` and `b` can be zero (but not both, to avoid undefined behavior). The function must handle large values efficiently using the Euclidean algorithm. The task is to implement this function without relying on the standard library’s `std::gcd`, and the function should be self-contained, const-correct, and usable in any context.
// The solution uses the Euclidean algorithm, which repeatedly replaces the larger number by its remainder when divided by the smaller number until one becomes zero. Specifically, while `b != 0`, compute `t = a % b`, set `a = b`, `b = t`. The result is the last non-zero value of `a`. This algorithm runs in \(O(\log \min(a,b))\) time because the numbers decrease exponentially. Edge cases include when one input is zero — if `a` is zero, the GCD is `b`; if `b` is zero, the GCD is `a`. If both are zero, it is undefined, but the task specifies they are not both zero. Since inputs are up to \(10^{18}\), using `long long` (64-bit) is sufficient; the modulo operation is safe. Space complexity is \(O(1)\) — only a few temporary variables are used.
#include <cstdint>   // for int64_t

// Compute the greatest common divisor of two non-negative integers.
// Both inputs must not be zero simultaneously.
long long gcdOfTwoNumbers(long long a, long long b) {
    // Handle cases where one input is zero.
    if (a == 0) return b;
    if (b == 0) return a;

    // Euclidean algorithm: reduce larger by remainder.
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}
#include <cassert>

int main() {
    // Basic positive pairs
    assert(gcdOfTwoNumbers(12, 18) == 6);
    assert(gcdOfTwoNumbers(100, 25) == 25);
    assert(gcdOfTwoNumbers(17, 31) == 1);
    
    // One zero
    assert(gcdOfTwoNumbers(0, 42) == 42);
    assert(gcdOfTwoNumbers(42, 0) == 42);
    
    // Equal numbers
    assert(gcdOfTwoNumbers(7, 7) == 7);
    
    // Large values near 1e18
    assert(gcdOfTwoNumbers(1000000000000000000LL, 500000000000000000LL) == 500000000000000000LL);
    assert(gcdOfTwoNumbers(999999999999999989LL, 999999999999999989LL) == 999999999999999989LL);
    
    // One is a multiple of the other
    assert(gcdOfTwoNumbers(123456789, 1) == 1);
    assert(gcdOfTwoNumbers(9876543210LL, 9876543210LL * 3) == 9876543210LL);
    
    return 0;
}
