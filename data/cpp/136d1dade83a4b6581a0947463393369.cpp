/*
Write a C++ function `long long countPairs(long long n, long long m, long long x, long long k)` that, given two positive integers `n` and `m` representing the dimensions of an `n × m` multiplication table (where the cell at row `i` and column `j` contains the product `i * j` for `1 ≤ i ≤ n`, `1 ≤ j ≤ m`), returns the `k`-th smallest value in this table (1-indexed, i.e., `k=1` gives the smallest product, which is always `1`). The function must compute the result using binary search on the candidate value and a counting function that determines how many products in the table are less than or equal to a given number. Assume `1 ≤ n, m ≤ 10^9` and `1 ≤ k ≤ n * m`, and that the answer fits in a signed 64-bit integer. You may write helper functions, but the main entry point for the task must be a free function named `kthSmallestMultiplication` that takes `n`, `m`, and `k` as arguments and returns the `k`-th smallest product.
*/
#include <algorithm>

// Count how many products in an n x m multiplication table are <= x.
// Uses a two-pointer-like approach to avoid recomputing min each time.
long long countLessEqual(long long n, long long m, long long x) {
    long long j = std::min(m, x);      // maximum possible column for row i=1
    long long max_i = std::min(n, x);  // rows with i > x have all products > x
    long long cnt = 0;
    for (long long i = 1; i <= max_i; ++i) {
        while (i * j > x) --j;         // shrink j until row i fits
        cnt += j;                      // all columns 1..j are valid for this i
    }
    return cnt;
}

// Return the k-th smallest product in an n x m multiplication table (1-indexed).
long long kthSmallestMultiplication(long long n, long long m, long long k) {
    long long low = 1;
    long long high = n * m; // inclusive upper bound
    while (low < high) {
        long long mid = low + (high - low) / 2;
        if (countLessEqual(n, m, mid) < k) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}
#include <cassert>

int main() {
    // Basic small table: 3x3 products are
    // 1 2 3
    // 2 4 6
    // 3 6 9
    // Sorted: 1,2,2,3,3,4,6,6,9
    assert(kthSmallestMultiplication(3, 3, 1) == 1);
    assert(kthSmallestMultiplication(3, 3, 2) == 2);
    assert(kthSmallestMultiplication(3, 3, 3) == 2);
    assert(kthSmallestMultiplication(3, 3, 4) == 3);
    assert(kthSmallestMultiplication(3, 3, 5) == 3);
    assert(kthSmallestMultiplication(3, 3, 6) == 4);
    assert(kthSmallestMultiplication(3, 3, 9) == 9);

    // Single row: n=1, m=5 products are 1,2,3,4,5
    assert(kthSmallestMultiplication(1, 5, 1) == 1);
    assert(kthSmallestMultiplication(1, 5, 3) == 3);
    assert(kthSmallestMultiplication(1, 5, 5) == 5);

    // Single column: n=4, m=1 products are 1,2,3,4
    assert(kthSmallestMultiplication(4, 1, 4) == 4);
    assert(kthSmallestMultiplication(4, 1, 1) == 1);

    // Large values: 1e9 x 1, k=1e9 -> answer is 1e9
    assert(kthSmallestMultiplication(1000000000LL, 1, 1000000000LL) == 1000000000LL);

    // Square table 2x2: 1,2,2,4
    assert(kthSmallestMultiplication(2, 2, 3) == 2);
    assert(kthSmallestMultiplication(2, 2, 4) == 4);

    return 0;
}
// The key observation is that for any candidate value `x`, the number of products `i * j` that are ≤ `x` can be computed by iterating over rows `i` from 1 to `min(n, x)` (since if `i > x`, even `j=1` gives `i*1 > x`). For each row `i`, the number of valid columns is `min(m, x / i)`. By summing these counts over all rows, we get a total count `cnt(x)` that is monotonically non-decreasing in `x`. This allows a binary search in the range `[1, n*m]` to find the smallest `x` such that `cnt(x) ≥ k`. If `cnt(x) < k`, then the answer is larger; otherwise the answer is ≤ `x`. The search narrows the interval until it converges. Important edge cases: when `k=1`, the answer is `1`; when `n` or `m` is `1`, the table is a single row/column and the answer is `k` (since products are `1,2,3,...` up to the other dimension). The counting function uses two optimizations: set `j = min(m, x)` and `i` up to `min(n, x)`, and for each `i`, maintain a running `j` that decrements only when `i*j` exceeds `x`, avoiding recomputation. Time complexity: the counting function runs in `O(min(n, x))` worst-case, and binary search does `O(log(n*m))` iterations, so overall `O(min(n, n*m) * log(n*m))` in the worst case, but in practice `x` is bounded by `n*m` and `min(n, x) ≤ n`, giving `O(n * log(n*m))`. Space complexity is `O(1)`.
