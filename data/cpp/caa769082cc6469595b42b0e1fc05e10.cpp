/*
Given three positive integers `m`, `n`, and `k`, write a C++ function `int kthSmallestInMultiplicationTable(int m, int n, int k)` that returns the `k`-th smallest number in the multiplication table of size `m x n`, where the table contains all products `i * j` for `1 ≤ i ≤ m` and `1 ≤ j ≤ n`. The multiplication table is considered as a multiset (i.e., duplicates are counted separately). For example, with `m = 3, n = 3`, the multiset of all products is `{1,2,2,3,3,4,6,6,9}` (unordered), and the 5th smallest is `3`. You may assume `1 ≤ m, n ≤ 3×10⁴` and `1 ≤ k ≤ m*n`. The function must be efficient for large inputs.
*/
#include <algorithm>

// Count how many products in the multiplication table (i * j for 1<=i<=m, 1<=j<=n) are <= value.
int countLessEqual(int value, int m, int n) {
    int rows = m;
    int cols = n;
    // Iterate over the smaller dimension to reduce complexity.
    if (rows > cols) {
        std::swap(rows, cols);
    }
    int cnt = 0;
    for (int i = 1; i <= rows; ++i) {
        cnt += std::min(cols, value / i);
    }
    return cnt;
}

// Return the k-th smallest number in the m x n multiplication table (1-indexed k).
int kthSmallestInMultiplicationTable(int m, int n, int k) {
    int low = 1;
    int high = m * n;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (countLessEqual(mid, m, n) >= k) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}
#include <cassert>

int main() {
    // Basic examples
    assert(kthSmallestInMultiplicationTable(3, 3, 1) == 1);
    assert(kthSmallestInMultiplicationTable(3, 3, 2) == 2);
    assert(kthSmallestInMultiplicationTable(3, 3, 5) == 3);
    assert(kthSmallestInMultiplicationTable(3, 3, 9) == 9);
    // 1xN table
    assert(kthSmallestInMultiplicationTable(1, 5, 3) == 3);
    assert(kthSmallestInMultiplicationTable(5, 1, 5) == 5);
    // Larger table with duplicates
    assert(kthSmallestInMultiplicationTable(2, 3, 4) == 3); // products: {1,2,3,2,4,6} sorted: 1,2,2,3,4,6
    assert(kthSmallestInMultiplicationTable(4, 2, 7) == 6); // products: 1,2,3,4,2,4,6,8 sorted:1,2,2,3,4,4,6,8
    // Boundary: k equals m*n
    assert(kthSmallestInMultiplicationTable(2, 2, 4) == 4);
    // Single cell
    assert(kthSmallestInMultiplicationTable(1, 1, 1) == 1);
    // Large m, n (not exhaustive but checks correctness on small k)
    assert(kthSmallestInMultiplicationTable(1000, 1000, 1) == 1);
}
// The problem is a classic "k-th smallest element in a sorted matrix" variant. We use binary search over the value range. The smallest possible product is `1` (when `i=1, j=1`), and the largest is `m * n`, but since `k ≤ m*n`, we can set the search range as `[1, m*n]` (or more tightly `[1, k]` because the `k`-th smallest cannot exceed `k` if all numbers are positive integers and `m,n ≥ 1`, but using `m*n` is also safe). For a candidate value `mid`, we count how many products in the table are ≤ `mid`. This count function iterates over rows (or columns, whichever is smaller for efficiency) and for each `i` from 1 to n (or 1 to m), the number of columns `j` such that `i*j ≤ mid` is `min(m, mid/i)` if we fix `i` as row index and `m` as number of columns. Summing these gives the total count of products ≤ `mid`. If this count is ≥ `k`, then the answer is ≤ `mid`, so we move the upper bound down; otherwise we move the lower bound up. The binary search continues until `l > r`, and `l` is the answer. Edge cases: when `m` or `n` is 1, the table is just a single row/column, but the algorithm still works. When `k` is 1, the answer is always 1. When `k` equals `m*n`, the answer is `m*n`. Time complexity: binary search runs in `O(log(m*n))` iterations, and each count is `O(min(m,n))` if we iterate over the smaller dimension, so total `O(min(m,n) * log(m*n))`. Space complexity is `O(1)`.
