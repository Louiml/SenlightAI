// Write a C++ function `int findKthSmallestProduct(int m, int n, int k)` that, given two positive integers `m` and `n` and a positive integer `k`, returns the `k`-th smallest value from the multiplication table of size `m × n` (i.e., all products `i * j` for `1 ≤ i ≤ m`, `1 ≤ j ≤ n`). The table may contain duplicate values, and each duplicate is counted separately. The function should work correctly for large inputs (up to `m, n ≤ 10^5` and `k` up to `m * n`) and must not actually construct the table. You can assume `1 ≤ k ≤ m * n`.

// The core idea is to binary search over the possible product value range `[1, m*n]`. For a candidate value `mid`, we count how many products in the table are strictly less than or equal to `mid` using a helper function `countLessEqual(m, n, mid)`. For each row `i` from 1 to `m`, the number of columns `j` such that `i * j ≤ mid` is `min(mid / i, n)`, because `j ≤ mid / i` and also `j ≤ n`. Summing over all rows gives the total count.  
// Binary search maintains an invariant: we want the smallest value `ans` such that `countLessEqual(ans) ≥ k`. We set `l = 1`, `r = m * n`. While `l ≤ r`, compute `mid = (l + r) / 2`. If `countLessEqual(mid) < k`, then the answer is larger than `mid`, so move `l = mid + 1`. Otherwise, the answer is at most `mid`, so record `ans = mid` and move `r = mid - 1`.  
// **Edge cases:** `m` or `n` equal to 1 (the table is a single row/column, binary search still works). Duplicate products are counted multiple times as required. If `k = 1`, the answer is always 1; if `k = m*n`, the answer is always `m*n`.  
// **Time complexity:** Binary search runs over `O(log(m*n))` iterations, and each count computation is `O(m)`, so total `O(m * log(m*n))`. Space complexity is `O(1)`.

#include <algorithm>

// Counts how many products in the m x n multiplication table are <= x.
int countLessEqual(int m, int n, int x) {
    int count = 0;
    for (int i = 1; i <= m; ++i) {
        count += std::min(x / i, n);
    }
    return count;
}

// Returns the k-th smallest product from the m x n multiplication table.
int findKthSmallestProduct(int m, int n, int k) {
    int low = 1;
    int high = m * n;
    int answer = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (countLessEqual(m, n, mid) < k) {
            low = mid + 1;
        } else {
            answer = mid;
            high = mid - 1;
        }
    }
    return answer;
}

#include <cassert>

int main() {
    // Basic cases
    assert(findKthSmallestProduct(3, 3, 1) == 1);
    assert(findKthSmallestProduct(3, 3, 5) == 3);
    assert(findKthSmallestProduct(3, 3, 9) == 9);

    // Single row/column
    assert(findKthSmallestProduct(1, 5, 1) == 1);
    assert(findKthSmallestProduct(1, 5, 3) == 3);
    assert(findKthSmallestProduct(5, 1, 4) == 4);

    // Duplicates counted separately: table for 2x3 = {1,2,3,2,4,6} sorted = {1,2,2,3,4,6}
    assert(findKthSmallestProduct(2, 3, 2) == 2);
    assert(findKthSmallestProduct(2, 3, 3) == 2);
    assert(findKthSmallestProduct(2, 3, 4) == 3);
    assert(findKthSmallestProduct(2, 3, 6) == 6);

    // Larger asymmetric table
    assert(findKthSmallestProduct(4, 2, 5) == 4); // sorted products: 1,2,2,3,4,4,6,8

    // Edge with k = m*n
    assert(findKthSmallestProduct(10, 10, 100) == 100);
    assert(findKthSmallestProduct(7, 11, 77) == 77);

    return 0;
}
