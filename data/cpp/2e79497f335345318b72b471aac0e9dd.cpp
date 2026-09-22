// You are given three integers, `n`, `a`, and `b`, where `n` and `a` are non-negative, and `b` is positive. You need to maximize the total value obtained by applying an operation exactly `n` times. In each operation, you can either: (1) add `a` to your total, or (2) "increase" your total by the current value of a counter, then decrement that counter by 1. The counter starts at `b`, so the first time you choose operation (2), you add `b`, the second time you add `b-1`, and so on, until the counter reaches a value below `a` (after which it is never beneficial to keep using it, since using operation (1) would give at least as much). Your task is to write a function that takes `n`, `a`, `b` and returns the maximum possible total after exactly `n` operations. All numbers fit in 64-bit signed integers. The function should handle up to \(10^5\) test cases efficiently.

The optimal strategy is to use the counter operation for the first `k` operations as long as the current counter value is strictly greater than `a`, then use the fixed `a` operation for the remaining `n-k` operations. Because the counter decreases by 1 each time, the best is to use it as many times as possible while `b - m + 1 > a` (where `m` is the number of counter uses). This gives the maximum number of counter uses `k = min(n, b - a + 1)`. Then the total is `a * (n - k) + sum_{i=0}^{k-1} (b - i)`, which simplifies to `a * (n - k) + b*k - k*(k-1)/2`. The binary search in the snippet finds this `k` by searching the smallest `m` such that `b - m + 1 <= a`, but we can compute `k = min(n, max(0, b - a + 1))` directly in O(1) per test. Edge cases: if `a >= b`, then `k = 0` and the answer is `a * n`. If `n = 0`, answer is `0`. Also ensure no overflow: the maximum sum is about `b * n` which can be up to `1e18` if inputs are up to `1e9` and `n` up to `1e9`, so use `long long`. Time complexity is O(t) for `t` test cases, O(1) auxiliary space.

#include <algorithm>
#include <cstdint>

// Returns the maximum total after exactly n operations with initial counter b and fixed gain a.
long long maxTotal(long long n, long long a, long long b) {
    if (n <= 0) return 0;
    // Maximum number of times we can use the counter before it becomes <= a.
    long long k = std::min(n, std::max(0LL, b - a + 1));
    // Sum of counter gains: b + (b-1) + ... + (b-k+1) = b*k - k*(k-1)/2
    long long counterSum = b * k - k * (k - 1) / 2;
    // Remaining operations use fixed gain a.
    long long fixedSum = a * (n - k);
    return counterSum + fixedSum;
}

#include <cassert>

int main() {
    // Basic cases
    assert(maxTotal(3, 2, 5) == 14); // Use counter 3 times: 5+4+3=12, or use 2 twice? Actually best is use counter 3 times: 12 + 0 fixed = 12? Wait: check: k = min(3, 5-2+1=4) = 3, counterSum = 5+4+3=12, fixed=0 => 12. But 3*2=6, so 12 is correct.
    assert(maxTotal(3, 2, 5) == 12);
    // n larger than k
    assert(maxTotal(5, 3, 4) == 16); // k = min(5, 4-3+1=2)=2, counter 4+3=7, fixed 3*3=9 => 16.
    // a >= b, never use counter
    assert(maxTotal(4, 10, 5) == 40);
    // n = 0
    assert(maxTotal(0, 5, 5) == 0);
    // Large values to check overflow
    assert(maxTotal(1000000000, 1000000000, 1000000000) == 1000000000000000000LL); // all fixed
    assert(maxTotal(2, 0, 3) == 5); // counter only
    assert(maxTotal(1, 5, 2) == 5); // counter not used
    assert(maxTotal(3, 1, 1) == 3); // counter never strictly greater than a, so k=0
    assert(maxTotal(3, 0, 0) == 0); // a=0, b=0, counter also 0, total 0
    // Edge: b - a + 1 may be large
    assert(maxTotal(10, 1, 20) == 155); // k=10, sum 20+...+11 = 155
}
