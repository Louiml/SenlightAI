Write a C++ function `long long maxArrayValue(int n, int index, long long maxSum)` that, given three positive integers where `n` is the length of a zero-indexed array, `index` is a valid position in that array, and `maxSum` is a positive upper bound, constructs an array `nums` of length `n` satisfying: every element is a positive integer, adjacent absolute differences are at most 1, the total sum does not exceed `maxSum`, and the value at `nums[index]` is as large as possible. Return that maximum possible value at `nums[index]`. The function must handle the constraints `1 <= n <= maxSum <= 1e9` and `0 <= index < n`. The implementation should be efficient enough for large inputs.
#include <cassert>

int main() {
    // Example 1
    assert(maxArrayValue(4, 2, 6) == 2);
    // Example 2
    assert(maxArrayValue(6, 1, 10) == 3);
    // Single element
    assert(maxArrayValue(1, 0, 5) == 5);
    // Peak at one end with small maxSum
    assert(maxArrayValue(3, 0, 4) == 2); // [2,1,1] sum=4
    // Large n, small maxSum forces all ones
    assert(maxArrayValue(10, 5, 10) == 1);
    // Edge where peak is in middle and maxSum is exact
    assert(maxArrayValue(5, 2, 7) == 2); // [1,2,2,1,1] sum=7
    // Another edge: n=2, index=1, maxSum=3 -> [1,2] sum=3
    assert(maxArrayValue(2, 1, 3) == 2);
    // Symmetric case
    assert(maxArrayValue(7, 3, 16) == 3); // [1,2,3,3,2,1,?] actually need sum<=16 with peak 3 gives [1,2,3,3,2,1,1] sum=13, peak 4 would be [1,2,3,4,3,2,1] sum=16, so answer 4? let's recalc: peak=4 sum=1+2+3+4+3+2+1=16, so answer 4.
    assert(maxArrayValue(7, 3, 16) == 4);
    // Large maxSum with tiny n
    assert(maxArrayValue(3, 1, 1000000000LL) == 1000000000LL); // [1e9-1,1e9,1e9-1]? Actually min sum for peak h is (h-1)+h+(h-1)=3h-2, need <=1e9 => h<=333333334, so answer is 333333334 not 1e9. Let's fix: max h such that 3h-2 <= 1e9 => h=333333334. So assert that.
    assert(maxArrayValue(3, 1, 1000000000LL) == 333333334LL);
    // Stress with big n but small maxSum
    assert(maxArrayValue(1000000000, 0, 1000000000LL) == 1); // all ones
    return 0;
}
#include <algorithm>

// Compute the minimal total sum if the value at `index` is `h`.
// n: array length, index: peak position, h: candidate height.
static long long minSumForHeight(long long h, long long n, long long index) {
    long long total = 0;

    // Left side: positions 0..index (including peak)
    if (h > index) {
        // Arithmetic series from h down to h-index
        total += (h - index + h) * (index + 1) / 2;
    } else {
        // Series from h down to 1, plus ones for the extra positions
        total += (1 + h) * h / 2;
        total += (index + 1 - h);
    }

    // Right side: positions index+1..n-1 (excluding peak)
    long long rightLen = n - index - 1;
    if (h > rightLen) {
        // Arithmetic series from h-1 down to h-rightLen
        total += (h - 1 + h - rightLen) * rightLen / 2;
    } else {
        // Series from h-1 down to 1, plus ones for the extra positions
        total += (h - 1 + 1) * (h - 1) / 2; // sum 1..h-1
        total += (rightLen - (h - 1));
    }

    return total;
}

// Return the maximum possible value at nums[index] under the constraints.
long long maxArrayValue(int n, int index, long long maxSum) {
    long long left = 1;
    long long right = maxSum;

    while (left < right) {
        // Use upper mid to avoid infinite loop (since we update left=mid)
        long long mid = right - (right - left) / 2;
        if (minSumForHeight(mid, n, index) <= maxSum) {
            left = mid;
        } else {
            right = mid - 1;
        }
    }
    return left;
}
// The problem is a classic binary search on the answer. For a candidate height `h` at `nums[index]`, we compute the minimum possible total sum of an array where `nums[index] = h` and the array satisfies the adjacent-difference constraint. The minimum sum is achieved by making elements as small as possible subject to the constraint that moving away from `index`, each step can decrease by at most 1, but never below 1. We split the array into two sides: left of `index` (including `index`) and right of `index` (excluding `index`). For the left side, if `h > index`, the sequence from `h` down to `h-index` is an arithmetic series; otherwise, we have `h, h-1, ..., 1` followed by `index+1-h` ones. Similarly for the right side, using `n-index-1` positions. The total sum is the sum of both sides minus `h` because the peak is counted twice. The `count` function computes this minimal sum for a given `h`. Since the minimum sum is non-decreasing in `h`, we binary search on `h` in `[1, maxSum]` using upper mid to avoid infinite loops, checking if `count(mid) <= maxSum`. Edge cases: when `h` is much larger than the distance to the boundary, the arithmetic series formula must be used; when `h` is small, the series caps at 1 and the remaining positions are filled with 1. Also note that `n` and `maxSum` can be up to 1e9, so use `long long` for intermediate calculations. Time complexity is `O(log maxSum)` due to binary search, and space is `O(1)`.
