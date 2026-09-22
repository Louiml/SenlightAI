Write a C++ function named `countTripleSums` that takes two integers, `n` (a positive integer, with `n >= 3`) and `x` (an integer), and returns the number of distinct triples `(i, j, k)` such that `1 <= i < j < k <= n` and `i + j + k == x`. The function must handle multiple test cases internally? No — instead, your function should be called repeatedly by the test harness. The function should return an `int` count. It must not read from standard input or write to standard output; it should only compute and return the count. The function should be efficient enough for `n` up to 1000. Consider that the sum `i+j+k` has a minimum of `1+2+3=6` and a maximum of `(n-2)+(n-1)+n = 3n-3`. Edge cases include when `x` is outside this range (return 0 immediately) and when `n` is small (e.g., `n=3`). The function must be self-contained, const-correct (parameters are `int` by value, so no const refs needed), and include only necessary headers.

The problem is a straightforward triple nested loop over all combinations of `i`, `j`, and `k` with `1 <= i < j < k <= n`. For each combination, check if the sum equals `x` and increment a counter. The naive approach is `O(n^3)` which is fine for `n <= 1000` (about 1.67 billion operations would be too many, but we can optimize). However, we can improve by pruning: the loop for `i` should only go up to `n-2`, `j` from `i+1` to `n-1`, `k` from `j+1` to `n`. This is exactly the snippet's logic. To avoid excessive runtime for `n=1000`, we can add early exit: if `x < 6` or `x > 3*n-3`, return 0. Also, within loops, if `i + j + (j+1) > x` we can break the `k` loop early (since `k` only increases), and if `i + (i+1) + (i+2) > x` then break outer loops. But given typical test cases, the simple triple loop is acceptable for `n` up to ~200 or so; for `n=1000`, it's about 1.67e8 iterations, which might be borderline but often acceptable in 2 seconds in C++. We can add the early exits to make it safe. Time complexity is `O(n^3)` in worst case, but with pruning it's much faster for small `x`. Space complexity is `O(1)`. Edge cases: `n < 3` is not expected but if passed, return 0. Also handle `x` being an integer (could be negative). The function should be named descriptively.

#include <algorithm>

// Returns the number of distinct triples (i, j, k) with 1 <= i < j < k <= n
// such that i + j + k == x.
int countTripleSums(int n, int x) {
    if (n < 3) return 0;
    // Minimum possible sum is 1+2+3=6, maximum is (n-2)+(n-1)+n = 3*n-3.
    if (x < 6 || x > 3 * n - 3) return 0;

    int count = 0;
    for (int i = 1; i <= n - 2; ++i) {
        // If the smallest possible sum with this i already exceeds x, break.
        if (i + (i + 1) + (i + 2) > x) break;
        for (int j = i + 1; j <= n - 1; ++j) {
            // If with i and j, the smallest k (j+1) makes sum > x, break.
            if (i + j + (j + 1) > x) break;
            for (int k = j + 1; k <= n; ++k) {
                if (i + j + k == x) {
                    ++count;
                }
                // Optional: if sum already exceeds x, break inner loop.
                if (i + j + k > x) break;
            }
        }
    }
    return count;
}

#include <cassert>

int main() {
    // Basic cases
    assert(countTripleSums(3, 6) == 1); // (1,2,3)
    assert(countTripleSums(3, 7) == 0);
    assert(countTripleSums(4, 6) == 1); // (1,2,3)
    assert(countTripleSums(5, 7) == 1); // (1,2,4)
    // Multiple combinations
    assert(countTripleSums(5, 8) == 2); // (1,2,5) and (1,3,4)
    // Larger n, check range boundaries
    assert(countTripleSums(10, 6) == 1); // only (1,2,3)
    assert(countTripleSums(10, 27) == 1); // (8,9,10) because (7,9,10)=26, (7,8,10)=25, (8,9,10)=27)
    // Edge: x out of range
    assert(countTripleSums(10, 5) == 0);
    assert(countTripleSums(10, 28) == 0);
    // Edge: n below 3
    assert(countTripleSums(2, 6) == 0);
    // Negative x
    assert(countTripleSums(5, -1) == 0);
    // Larger n, verify count by brute force comparison
    int total = 0;
    for (int i = 1; i <= 5; ++i) {
        for (int j = i + 1; j <= 6; ++j) {
            for (int k = j + 1; k <= 6; ++k) {
                if (i + j + k == 12) ++total;
            }
        }
    }
    assert(countTripleSums(6, 12) == total);
    return 0;
}
