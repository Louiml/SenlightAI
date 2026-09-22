/*
Given a square matrix `matrix` of size `n x n` where each row and each column is sorted in strictly non-decreasing order, and a positive integer `k` (1 ≤ k ≤ n²), write a C++ function `kthSmallestInSortedMatrix` that returns the `k`-th smallest element in the matrix (1-indexed, i.e., the smallest element is the 1st). The matrix elements are integers and may contain duplicates. If the matrix is empty, return `-1` to indicate invalid input. The function must be efficient: it should not flatten and sort the entire matrix (which would be O(n² log n²)), but instead use a binary-search-based approach over the value range. The matrix is guaranteed to satisfy the row- and column-wise sorted property.
*/
#include <vector>
#include <algorithm>

// Returns the k-th smallest element in a row- and column-sorted square matrix.
// Returns -1 if the matrix is empty.
int kthSmallestInSortedMatrix(const std::vector<std::vector<int>>& matrix, int k) {
    if (matrix.empty() || matrix[0].empty()) {
        return -1;
    }
    const int n = static_cast<int>(matrix.size());
    int left = matrix[0][0];
    int right = matrix[n - 1][n - 1];

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int count = 0;
        for (int i = 0; i < n; ++i) {
            // Count elements <= mid in row i
            count += static_cast<int>(
                std::upper_bound(matrix[i].begin(), matrix[i].end(), mid) - matrix[i].begin()
            );
        }
        if (count >= k) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return left;
}
#include <cassert>
#include <vector>

// Assume kthSmallestInSortedMatrix is defined as above.

int main() {
    // Example 1: 3x3 matrix, k=5 -> 5
    std::vector<std::vector<int>> m1 = {
        {1, 5, 9},
        {10, 11, 13},
        {12, 13, 15}
    };
    assert(kthSmallestInSortedMatrix(m1, 5) == 11);
    assert(kthSmallestInSortedMatrix(m1, 1) == 1);
    assert(kthSmallestInSortedMatrix(m1, 9) == 15);

    // Example 2: Duplicates and k=3
    std::vector<std::vector<int>> m2 = {
        {2, 2, 2},
        {2, 2, 2},
        {2, 2, 2}
    };
    assert(kthSmallestInSortedMatrix(m2, 1) == 2);
    assert(kthSmallestInSortedMatrix(m2, 5) == 2);
    assert(kthSmallestInSortedMatrix(m2, 9) == 2);

    // Example 3: 1x1 matrix
    std::vector<std::vector<int>> m3 = {{-100}};
    assert(kthSmallestInSortedMatrix(m3, 1) == -100);

    // Example 4: 2x2 with negative values
    std::vector<std::vector<int>> m4 = {{-3, -1}, {-2, 0}};
    assert(kthSmallestInSortedMatrix(m4, 1) == -3);
    assert(kthSmallestInSortedMatrix(m4, 2) == -2);
    assert(kthSmallestInSortedMatrix(m4, 3) == -1);
    assert(kthSmallestInSortedMatrix(m4, 4) == 0);

    // Example 5: Edge case – empty matrix
    std::vector<std::vector<int>> m5;
    assert(kthSmallestInSortedMatrix(m5, 1) == -1);

    return 0;
}
// The optimal approach is a binary search on the value range from `matrix[0][0]` (minimum) to `matrix[n-1][n-1]` (maximum). For a candidate `mid`, we count how many matrix elements are less than or equal to `mid`. Because each row is sorted individually, we can use `std::upper_bound` per row to find the first element greater than `mid`; the difference between that iterator and the row's begin gives the count for that row. Summing across all `n` rows gives the total count `cnt`. If `cnt >= k`, the candidate is too large (or exactly the answer), so we shrink the upper bound to `mid - 1`. Otherwise, we increase the lower bound to `mid + 1`. The loop terminates when `head > tail`, and `head` becomes the smallest value for which the count of elements ≤ `head` is at least `k`; that value is exactly the `k`-th smallest element. Edge cases: an empty matrix returns `-1`; duplicates are naturally handled because the count includes them; if `k` equals 1 or n², the search still works correctly. Time complexity: each iteration performs `n` binary searches over rows of length `n`, so each count is O(n log n). The binary search over the value range runs O(log(max-min+1)) ≈ O(log(maxVal)) iterations. In the worst case, for an `n x n` matrix with values up to `V`, the total is O(n log n * log V). Space complexity is O(1) beyond the input.
