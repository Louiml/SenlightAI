// Given a square matrix of integers with rows and columns sorted in non-decreasing order, write a C++ function `int kthSmallest(const std::vector<std::vector<int>>& matrix, int k)` that returns the k-th smallest element in the matrix (1-indexed). The matrix has at least one element, k is guaranteed to be between 1 and n*n (inclusive), where n is the matrix dimension. The matrix values may be negative, zero, or positive, and duplicates may exist. Do not use the STL `std::nth_element` or sorting; instead, implement a binary-search-based approach over the value range of the matrix.
The key insight is that the element values themselves lie between `matrix[0][0]` (minimum) and `matrix[n-1][n-1]` (maximum). We perform binary search on this integer value range. For a candidate `mid`, we count how many elements in the matrix are less than or equal to `mid`. Because each row is sorted, we can quickly count using a pointer `temp` that starts at the last column and moves left while `matrix[i][temp] > mid`. For each row, we add `temp+1` to the count. If the total count is less than `k`, we need a larger value, so `low = mid + 1`; otherwise, `high = mid`. The loop continues until `low == high`, which is the smallest value for which the count is at least `k` — that is the k-th smallest element (accounting for duplicates). Edge cases: when `k = n*n`, the answer is the maximum value; when k=1, the minimum. The algorithm works with negative values because the binary search operates on integers regardless of sign. Time complexity: `O(n log(max-min))` for the binary search, where each count takes `O(n)` because `temp` resets only at the start and can only decrease per row (worst case `n` steps per row, but total across rows is `O(n)` if we reset per row? Actually, `temp` resets to `n-1` for each row, so counting is `O(n)` per mid, giving `O(n log(V))` where `V` is the range of values). Space complexity: `O(1)` extra space, excluding the input.
#include <vector>

// Returns the k-th smallest element (1-indexed) in a row-wise and column-wise
// sorted square matrix. Uses binary search on the value range.
int kthSmallest(const std::vector<std::vector<int>>& matrix, int k) {
    const int n = static_cast<int>(matrix.size());
    int low = matrix[0][0];
    int high = matrix[n - 1][n - 1];

    while (low < high) {
        const int mid = low + (high - low) / 2;
        int count = 0;
        int col = n - 1;  // Reset per mid; can be optimized but this is clear.

        for (int row = 0; row < n; ++row) {
            // Move left until the current row's element at col is <= mid.
            while (col >= 0 && matrix[row][col] > mid) {
                --col;
            }
            // All elements from col down to 0 are <= mid.
            count += (col + 1);
        }

        if (count < k) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}
#include <cassert>
#include <vector>

// Function declaration (assume defined elsewhere as in the solution).
int kthSmallest(const std::vector<std::vector<int>>& matrix, int k);

int main() {
    // Basic 3x3 matrix.
    std::vector<std::vector<int>> m1 = {{1,5,9}, {10,11,13}, {12,13,15}};
    assert(kthSmallest(m1, 1) == 1);
    assert(kthSmallest(m1, 3) == 9);
    assert(kthSmallest(m1, 5) == 11);
    assert(kthSmallest(m1, 8) == 13);
    assert(kthSmallest(m1, 9) == 15);

    // 1x1 matrix.
    std::vector<std::vector<int>> m2 = {{-7}};
    assert(kthSmallest(m2, 1) == -7);

    // 2x2 with duplicates and negatives.
    std::vector<std::vector<int>> m3 = {{-5, -5}, {-4, -2}};
    assert(kthSmallest(m3, 1) == -5);
    assert(kthSmallest(m3, 2) == -5);
    assert(kthSmallest(m3, 3) == -4);
    assert(kthSmallest(m3, 4) == -2);

    // All identical values.
    std::vector<std::vector<int>> m4 = {{3,3,3}, {3,3,3}, {3,3,3}};
    for (int k = 1; k <= 9; ++k) {
        assert(kthSmallest(m4, k) == 3);
    }

    // Large range with zeros and positives.
    std::vector<std::vector<int>> m5 = {{0, 10, 20}, {1, 11, 21}, {2, 12, 22}};
    assert(kthSmallest(m5, 2) == 1);
    assert(kthSmallest(m5, 4) == 10);
    assert(kthSmallest(m5, 7) == 20);

    // Edge case k = n*n (largest element).
    assert(kthSmallest(m1, 9) == 15);
    assert(kthSmallest(m3, 4) == -2);

    return 0;
}
