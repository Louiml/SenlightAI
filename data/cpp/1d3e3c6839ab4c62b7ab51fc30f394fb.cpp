Write a C++ function that computes the integer square root of a non-negative integer `x` using only integer arithmetic (no floating point or library functions like `std::sqrt`). The integer square root is defined as the largest integer `r` such that `r * r <= x`. The function must handle edge cases: `x = 0`, `x = 1`, and very large values (up to `INT_MAX`), and must avoid overflow when computing midpoints or comparisons. The function should be named `integerSqrt` and accept an `int` parameter, returning an `int`. You may assume the input is always non-negative.

#include <cassert>
#include <climits>

int main() {
    // Edge cases
    assert(integerSqrt(0) == 0);
    assert(integerSqrt(1) == 1);
    assert(integerSqrt(2) == 1);
    assert(integerSqrt(3) == 1);
    assert(integerSqrt(4) == 2);
    assert(integerSqrt(15) == 3);
    assert(integerSqrt(16) == 4);
    // Large values
    assert(integerSqrt(INT_MAX) == 46340);  // 46340^2 = 2147395600, 46341^2 = 2147488281 > INT_MAX
    assert(integerSqrt(2147395600) == 46340);
    assert(integerSqrt(2147483646) == 46340);
    // Random check
    assert(integerSqrt(1000000) == 1000);
    assert(integerSqrt(999999) == 999);
    return 0;
}

#include <cstdint>  // for INT_MAX? Not needed; we use int.

// Compute the integer square root of a non-negative integer x.
// Returns the largest integer r such that r * r <= x.
int integerSqrt(int x) {
    if (x < 0) return 0;  // Undefined for negative; but per spec non-negative.
    if (x == 0) return 0;

    int left = 1;
    int right = x;
    int last_mid = 0;

    while (left <= right) {
        int mid = left + (right - left) / 2; // avoids overflow

        if (x / mid > mid) {
            // mid is too small; it is a candidate (mid^2 < x)
            left = mid + 1;
            last_mid = mid;
        } else if (x / mid < mid) {
            right = mid - 1;
        } else {
            return mid; // exact square root
        }
    }
    return last_mid;
}

// The solution uses a binary search over the range `[0, x]` (or `[1, x]` with a special case for 0). We maintain `left` and `right` bounds. The key trick to avoid overflow is to compare `x / mid` with `mid` instead of computing `mid * mid`, which could exceed `int` range. For each midpoint `mid`, we evaluate:
// - If `x / mid > mid`, then `mid` is too small, so we move `left = mid + 1` and remember `last_mid = mid` because this `mid` is a candidate answer (it is less than the true sqrt).
// - If `x / mid < mid`, then `mid` is too large, so we move `right = mid - 1`.
// - If equal, we found the exact square root and return `mid`.
//
// When the loop ends (left > right), the last valid `mid` that satisfied `x / mid >= mid` is the integer sqrt. Edge cases: for `x = 0`, the binary search starting at `left = 1` would skip, so we must return 0 immediately. For `x = 1`, the loop works: mid=1, x/mid==mid, returns 1. For `x = INT_MAX`, the division approach avoids overflow since `x / mid` is always within int range. Time complexity is O(log x) because we halve the search space each iteration. Space complexity is O(1).
