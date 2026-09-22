Write a C++ function that, given a positive integer `N`, returns the largest integer `k` such that the sum of the first `k` positive integers (i.e., `1 + 2 + ... + k`) does not exceed `N`. This is equivalent to finding the maximum number of complete rows in a staircase built with `N` blocks, where row `i` contains exactly `i` blocks. The function must handle all positive integers `N` (and also `N = 0`, returning `0`), and must be efficient even for very large `N` (up to 2,147,483,647). The function should be named `maxCompleteStaircaseRows` and take an `int` parameter, returning an `int`.

// The problem reduces to solving for the largest integer `k` such that the triangular number `T(k) = k*(k+1)/2 <= N`. A direct loop from `k = 1` upward would work but could be slow for large `N` (up to about 65,536 iterations if `N` is near 2^31, which is acceptable but not ideal). However, a more robust and elegant approach uses binary search on `k` in the range `[0, 65536]` (since the maximum `k` for `N = 2,147,483,647` is about 65,535, because `T(65535)` ≈ 2.147e9). Alternatively, one can solve the quadratic inequality: `k^2 + k - 2N <= 0`, giving `k = floor((sqrt(1 + 8N) - 1)/2)`. Using integer arithmetic with `double` for the square root is safe because `N` is within 32-bit range and the result is a small integer; but to avoid floating-point precision issues, we can use binary search. Binary search runs in `O(log N)` time, which is effectively constant for typical `int` ranges. Edge cases: `N = 0` returns `0`; `N = 1` returns `1`; for `N` exactly equal to a triangular number, we must include that `k` (e.g., `N=6 → k=3` because `T(3)=6 <= 6`, while `T(4)=10 > 6`). The solution must ensure `k*(k+1)/2` does not overflow `int`, but since `k` is at most ~65,535, `k*(k+1)` fits in `long long` safely. Space complexity is `O(1)`.

#include <cstdint>

// Returns the largest k such that the sum 1+2+...+k <= N.
int maxCompleteStaircaseRows(int N) {
    if (N <= 0) return 0;
    
    long long low = 0;
    long long high = 65536; // because T(65536) > 2^31-1, far above any int N
    long long answer = 0;
    
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        long long sum = mid * (mid + 1) / 2;
        if (sum <= N) {
            answer = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return static_cast<int>(answer);
}

#include <cassert>

int main() {
    assert(maxCompleteStaircaseRows(0) == 0);
    assert(maxCompleteStaircaseRows(1) == 1);
    assert(maxCompleteStaircaseRows(2) == 1); // 1+2=3 > 2
    assert(maxCompleteStaircaseRows(3) == 2); // 1+2=3 exactly
    assert(maxCompleteStaircaseRows(6) == 3); // 1+2+3=6 exactly
    assert(maxCompleteStaircaseRows(7) == 3); // 1+2+3=6, next is 10
    assert(maxCompleteStaircaseRows(10) == 4);
    assert(maxCompleteStaircaseRows(11) == 4);
    assert(maxCompleteStaircaseRows(2147483647) == 65535); // largest int
    assert(maxCompleteStaircaseRows(100) == 13); // T(13)=91, T(14)=105
}
