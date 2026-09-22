/*
Write a C++ function `long long productOfDigits(int start, int end)` that accepts two integers `start` and `end` with `start <= end`, and returns the product of the factorial of each integer in the inclusive range `[start, end]`. For example, if `start = 2` and `end = 4`, the result should be `2! * 3! * 4! = 2 * 6 * 24 = 288`. The function must handle the case where the range contains only one number, and must guarantee no overflow by using `long long` return type. You are **not** allowed to use any standard library factorial function; implement your own factorial helper.
*/
#include <cstdint>

// Helper: compute factorial of a non-negative integer n iteratively.
// Returns 1 for n = 0. Assumes n >= 0.
long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Compute product of factorials for all integers from start to end inclusive.
// Assumes start <= end and both non-negative. Returns product as long long.
long long productOfFactorials(int start, int end) {
    long long product = 1;
    for (int i = start; i <= end; ++i) {
        product *= factorial(i);
    }
    return product;
}
#include <cassert>

int main() {
    // Single number in range
    assert(productOfFactorials(0, 0) == 1);       // 0! = 1
    assert(productOfFactorials(1, 1) == 1);       // 1! = 1
    assert(productOfFactorials(5, 5) == 120);     // 5! = 120

    // Multi-number ranges
    assert(productOfFactorials(2, 4) == 288);     // 2! * 3! * 4! = 2*6*24
    assert(productOfFactorials(1, 3) == 12);      // 1! * 2! * 3! = 1*2*6
    assert(productOfFactorials(0, 2) == 2);       // 0! * 1! * 2! = 1*1*2

    // Range starting at 0 with larger end
    assert(productOfFactorials(0, 3) == 12);      // 1*1*2*6

    // Slightly larger but safe range
    assert(productOfFactorials(4, 5) == 2880);    // 24*120

    // Range with length > 1 and all small numbers
    assert(productOfFactorials(3, 3) == 6);       // 3! = 6

    // Edge: start = end = 0 and 1 covered above, also test 2-element range
    assert(productOfFactorials(0, 1) == 1);       // 1*1

    // Final check with a range of size 4
    assert(productOfFactorials(1, 4) == 288);     // 1*2*6*24
    return 0;
}
// The solution requires two components: a helper function `factorial(int n)` that computes `n!` iteratively for `n >= 0`, and the main function `productOfDigits` that iterates from `start` to `end`, multiplys the factorial of each integer into a `long long` accumulator initialized to 1. Edge cases: (1) `start == end` returns factorial of that single number; (2) `n = 0` factorial is defined as 1 (since `0! = 1`); (3) negative `n` is not expected as per the task assumption (`start <= end` and likely non-negative), but for robustness, we can return 1 or handle with an assert; here we assume non-negative input. Time complexity is `O(k * m)` where `k = end - start + 1` numbers in range and `m` is approximately `end` (the largest factorial argument) — effectively `O(end^2)` in worst case. Space complexity is `O(1)` as only constant extra space is used (excluding input). Multiplying factorials can overflow even `long long` for large `end` (e.g., `end=20` gives ~2.4e18, product grows quickly), so the function is suitable for small ranges; we assume inputs are small enough to stay within `long long`.
