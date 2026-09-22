Write a standalone C++ function that accepts a square matrix (represented as `std::vector<std::vector<int>>`) and modifies it in-place so that it becomes its transpose: the element at row `i`, column `j` moves to row `j`, column `i`. The function should return nothing (`void`) and must handle matrices of any size `n x n` where `n >= 1`. After transposition, the matrix must be fully transformed—no extra output is required from the function itself. Ensure that the original matrix is modified, not copied, and that the algorithm works correctly for a 1x1 matrix (which remains unchanged) and for matrices where diagonal elements stay in place.

The transpose of a square matrix is obtained by swapping elements symmetrically across the main diagonal (from top-left to bottom-right). The key observation is that to avoid swapping an element twice, we only need to iterate over the upper triangle (excluding the diagonal): for each `i` from 0 to `n-1`, for each `j` from `i+1` to `n-1`, swap `matrix[i][j]` with `matrix[j][i]`. This ensures every off-diagonal pair is swapped exactly once, while diagonal elements (`i == j`) remain unchanged. Edge cases include `n = 1` (no swaps needed) and `n = 2` (a single swap). Since we operate directly on the input vector, no extra storage is required beyond a temporary variable for swapping. Time complexity is `O(n^2)` because we visit roughly `n^2/2` cells; space complexity is `O(1)` auxiliary. The function should validate that the matrix is non-empty and square for robustness, though the problem guarantees valid input.

#include <vector>
#include <utility>  // for std::swap

/**
 * Transpose a square matrix in-place.
 * 
 * The matrix is modified so that element at (i, j) moves to (j, i).
 * Diagonal elements remain in place.
 * 
 * @param matrix A non-empty square matrix to be transposed.
 */
void transposeSquareMatrix(std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix.size() != matrix[0].size()) {
        return;  // Guard against invalid input; assumption is square and non-empty.
    }
    const int n = static_cast<int>(matrix.size());
    // Swap elements above the diagonal with their symmetric counterpart.
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test.
void transposeSquareMatrix(std::vector<std::vector<int>>& matrix);

int main() {
    // Test 1: 1x1 matrix remains unchanged.
    std::vector<std::vector<int>> m1 = {{42}};
    transposeSquareMatrix(m1);
    assert(m1 == std::vector<std::vector<int>>({{42}}));

    // Test 2: 2x2 matrix swaps off-diagonal.
    std::vector<std::vector<int>> m2 = {{1, 2}, {3, 4}};
    transposeSquareMatrix(m2);
    assert(m2 == std::vector<std::vector<int>>({{1, 3}, {2, 4}}));

    // Test 3: 3x3 matrix with varied values.
    std::vector<std::vector<int>> m3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    transposeSquareMatrix(m3);
    assert(m3 == std::vector<std::vector<int>>({{1, 4, 7}, {2, 5, 8}, {3, 6, 9}}));

    // Test 4: 4x4 matrix (like the original snippet).
    std::vector<std::vector<int>> m4 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    transposeSquareMatrix(m4);
    assert(m4 == std::vector<std::vector<int>>({{1, 5, 9, 13}, {2, 6, 10, 14}, {3, 7, 11, 15}, {4, 8, 12, 16}}));

    // Test 5: Transpose twice returns original.
    std::vector<std::vector<int>> m5 = {{11, 22, 33}, {44, 55, 66}, {77, 88, 99}};
    std::vector<std::vector<int>> original = m5;
    transposeSquareMatrix(m5);
    transposeSquareMatrix(m5);
    assert(m5 == original);

    // Test 6: Matrix with negative numbers.
    std::vector<std::vector<int>> m6 = {{-1, -2}, {-3, -4}};
    transposeSquareMatrix(m6);
    assert(m6 == std::vector<std::vector<int>>({{-1, -3}, {-2, -4}}));

    return 0;
}
