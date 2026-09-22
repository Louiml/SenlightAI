Write a C++ function `int determinant(vector<vector<int>> matrix)` that takes a square matrix (size `n x n`, with `n >= 1`) as input and returns its determinant as an integer. The input may contain zero, positive, and negative integers, and the matrix may be singular (determinant = 0) or have any size. The function must use the Bareiss algorithm (a fraction‑free Gaussian elimination) to avoid floating‑point errors and integer overflow. You cannot use recursion or built‑in determinant functions. The function should be robust for matrices where row swaps are needed and where a pivot is zero.
The Bareiss algorithm is a variant of Gaussian elimination that computes the determinant exactly using only integer arithmetic, avoiding fractions and the numerical instability of floating‑point. The idea is to eliminate entries below the diagonal while maintaining a fraction‑free process. At each step `k`, the pivot is the element `matrix[k][k]`. If it is zero, we find a row below with a non‑zero element in column `k` and swap them, which multiplies the determinant by `-1`. Then for rows `i > k` and columns `j >= k`, we update:  
`matrix[i][j] = (matrix[k][k] * matrix[i][j] - matrix[i][k] * matrix[k][j]) / prev_pivot`  
where `prev_pivot` is the previous diagonal element (for `k=0`, `prev_pivot = 1`). This division is exact because the numerator is divisible by `prev_pivot`. At the end, the determinant is the last diagonal element (or `matrix[n-1][n-1]`). Edge cases: when the matrix is singular, a zero pivot may be encountered with no non‑zero below; in that case the determinant is zero. The algorithm runs in `O(n^3)` time and uses `O(1)` extra space (modifying the input copy). For `n=1`, the determinant is the single element.
#include <vector>
#include <algorithm>

// Compute the determinant of a square integer matrix using the Bareiss algorithm.
// The input matrix is passed by value to avoid modifying the caller's data.
int determinant(std::vector<std::vector<int>> matrix) {
    int n = matrix.size();
    if (n == 1) return matrix[0][0];

    int prev_pivot = 1;  // For the first step, "previous" pivot is 1.
    int sign = 1;

    for (int k = 0; k < n - 1; ++k) {
        // Find a non‑zero pivot in column k, starting from row k.
        int pivot_row = k;
        while (pivot_row < n && matrix[pivot_row][k] == 0) {
            ++pivot_row;
        }

        // If no non‑zero pivot exists, the determinant is zero.
        if (pivot_row == n) return 0;

        // Swap rows if needed and flip the sign.
        if (pivot_row != k) {
            std::swap(matrix[pivot_row], matrix[k]);
            sign = -sign;
        }

        int current_pivot = matrix[k][k];
        // Eliminate entries below the diagonal.
        for (int i = k + 1; i < n; ++i) {
            for (int j = k + 1; j < n; ++j) {
                // Exact integer division is guaranteed by the algorithm.
                matrix[i][j] = (current_pivot * matrix[i][j] - matrix[i][k] * matrix[k][j]) / prev_pivot;
            }
        }

        prev_pivot = current_pivot;
    }

    return sign * matrix[n - 1][n - 1];
}
#include <cassert>
#include <vector>

// (The determinant function is assumed to be defined above.)

int main() {
    // 1x1 matrix
    assert(determinant({{5}}) == 5);
    assert(determinant({{-3}}) == -3);

    // 2x2 matrices
    assert(determinant({{1, 2}, {3, 4}}) == -2);
    assert(determinant({{2, 0}, {0, 2}}) == 4);
    assert(determinant({{0, 1}, {1, 0}}) == -1);
    assert(determinant({{0, 0}, {0, 0}}) == 0);

    // 3x3 matrices
    assert(determinant({{6, 1, 1}, {4, -2, 5}, {2, 8, 7}}) == -306);
    assert(determinant({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) == 0);
    assert(determinant({{2, 0, 0}, {0, 3, 0}, {0, 0, 4}}) == 24);

    // 4x4 matrix with a row swap needed and zero pivot
    assert(determinant({{0, 0, 0, 1}, {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}}) == 1);

    // Larger matrix with negative numbers and duplicates
    assert(determinant({{3, 7, 1, 5}, {2, 1, 9, 4}, {6, 8, 0, 2}, {4, 5, 3, 1}}) == -594);

    return 0;
}
