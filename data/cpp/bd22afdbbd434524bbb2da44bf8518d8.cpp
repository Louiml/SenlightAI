Given a positive integer `n` (where `n` is even and at least 2), write a C++ function `long long pileDifference(int n)` that simulates the following process. You have an array `a` of size `n` with 1-based indexing, where `a[i] = 2^i` (i.e., `a[1]=2`, `a[2]=4`, ..., `a[n]=2^n`). You must split all `n` numbers into two piles: the first pile contains indices `1` through `n/2` (inclusive) plus the largest index `n`; the second pile contains indices `(n/2 + 1)` through `n-1` (inclusive). Return the absolute difference between the sums of the two piles. For example, if `n=4`, the first pile has `a[1]+a[2]+a[4] = 2+4+16=22`, the second pile has `a[3]=8`, difference is `14`. Your function should handle `n` up to 30 (so `2^30` fits in a signed 64-bit integer). The input `n` is always even and at least 2.
#include <cassert>

int main() {
    // n=2: first pile indices 1,2 => 2+4=6; second pile empty => 0; diff=6
    assert(pileDifference(2) == 6);
    // n=4: first 1,2,4 => 2+4+16=22; second 3 => 8; diff=14
    assert(pileDifference(4) == 14);
    // n=6: first 1,2,3,6 => 2+4+8+64=78; second 4,5 => 16+32=48; diff=30
    assert(pileDifference(6) == 30);
    // n=8: first 1..4 and 8 => 2+4+8+16+256=286; second 5..7 => 32+64+128=224; diff=62
    assert(pileDifference(8) == 62);
    // n=10: first 1..5 and 10 => sum=2+4+8+16+32+1024=1086; second 6..9 => 64+128+256+512=960; diff=126
    assert(pileDifference(10) == 126);
    // n=30: just check it returns a non-negative number (no overflow)
    assert(pileDifference(30) >= 0);
    // n=30 exact value? compute manually: difference = 2^(n) - 2^(n/2) ? Let's derive: first sum = sum_{i=1}^{n/2} 2^i + 2^n. second = sum_{i=n/2+1}^{n-1} 2^i = (sum_{i=1}^{n-1} 2^i) - sum_{i=1}^{n/2} 2^i = (2^n - 2) - sum_first_half. So first - second = 2*sum_first_half + 2^n - (2^n - 2) = 2*sum_first_half + 2. sum_first_half = 2^(n/2+1)-2. So first-second = 2^(n/2+2) - 4 + 2 = 2^(n/2+2) - 2. For n=30, n/2=15, 2^(17)-2 = 131072-2=131070. Let's test.
    assert(pileDifference(30) == 131070);
    return 0;
}
#include <cstdint>
#include <cstdlib> // for std::llabs

// Compute the absolute difference between two piles formed from powers of two.
// First pile: indices 1..n/2 and n. Second pile: indices n/2+1..n-1.
// n is even and at least 2. Returns the absolute difference (non-negative).
long long pileDifference(int n) {
    long long firstSum = 0;
    long long secondSum = 0;

    // First pile: indices 1 to n/2 (inclusive)
    for (int i = 1; i <= n / 2; ++i) {
        firstSum += (1LL << i);
    }
    // Add the last index n to the first pile
    firstSum += (1LL << n);

    // Second pile: indices n/2+1 to n-1 (inclusive)
    for (int i = n / 2 + 1; i <= n - 1; ++i) {
        secondSum += (1LL << i);
    }

    // Return absolute difference
    return (firstSum > secondSum) ? (firstSum - secondSum) : (secondSum - firstSum);
}
// The problem is straightforward: we sum powers of two over specified index sets. Since `n` is even, the first pile consists of indices `1` to `n/2` inclusive, plus index `n`. The second pile consists of indices `n/2+1` to `n-1` inclusive. Because the powers of two grow rapidly, the difference will often be dominated by the largest term, but for correctness we compute both sums exactly. Constraints: `n` up to 30, so `2^n` fits in `long long` (max `2^30 ≈ 1e9`). Use `1LL << i` for `2^i` to avoid overflow of `int` (since `1<<30` fits in `int`, but using `long long` is safer). Edge cases: `n=2` → first pile indices 1 and 2, second pile empty (sum 0), difference = 2+4=6. For `n=4`, as above difference=14. The algorithm runs in `O(n)` time (summing up to `n` powers) and `O(1)` auxiliary space besides the loop variables. No special data structures are needed.
