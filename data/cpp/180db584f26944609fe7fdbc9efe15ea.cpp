// Write a standalone C++ function named `findPeaksInMatrix` that accepts a 2D integer matrix `matrix` (represented as `std::vector<std::vector<int>>`) and returns a `std::vector<std::pair<int, int>>` containing the 0-based coordinates of all "peak" cells. A peak is a cell whose value is **greater than or equal to** all of its existing neighbors (up, down, left, right) within the bounds of the matrix. The coordinates must be returned in row-major order (top-to-bottom, then left-to-right). The matrix is guaranteed to be non-empty and rectangular (all rows have the same length). The function must not modify the input, and it must handle edge cases such as single-row matrices, single-column matrices, and a 1x1 matrix.

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here or via header

int main() {
    // 1x1 matrix
    {
        std::vector<std::vector<int>> m = {{5}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 1 && result[0] == std::make_pair(0, 0));
    }
    // Single row, all equal
    {
        std::vector<std::vector<int>> m = {{1, 1, 1}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 3);
        assert(result[0] == std::make_pair(0, 0));
        assert(result[1] == std::make_pair(0, 1));
        assert(result[2] == std::make_pair(0, 2));
    }
    // Single column, increasing
    {
        std::vector<std::vector<int>> m = {{0}, {1}, {2}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 1 && result[0] == std::make_pair(2, 0));
    }
    // 2x2 with one clear peak
    {
        std::vector<std::vector<int>> m = {{1, 2}, {3, 4}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 1 && result[0] == std::make_pair(1, 1));
    }
    // 3x3 with multiple peaks, including tie-handling
    {
        std::vector<std::vector<int>> m = {
            {3, 1, 2},
            {1, 3, 1},
            {2, 1, 3}
        };
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 3);
        assert(result[0] == std::make_pair(0, 0));
        assert(result[1] == std::make_pair(1, 1));
        assert(result[2] == std::make_pair(2, 2));
    }
    // All equal 2x2 – all cells are peaks
    {
        std::vector<std::vector<int>> m = {{7, 7}, {7, 7}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 4);
    }
    // Matrix with negative values
    {
        std::vector<std::vector<int>> m = {{-5, -3}, {-4, -6}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 1 && result[0] == std::make_pair(0, 1));
    }
    // Row-major order correctness in an asymmetric case
    {
        std::vector<std::vector<int>> m = {{5, 1, 5}, {1, 1, 1}, {5, 1, 5}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 4);
        assert(result[0] == std::make_pair(0, 0));
        assert(result[1] == std::make_pair(0, 2));
        assert(result[2] == std::make_pair(2, 0));
        assert(result[3] == std::make_pair(2, 2));
    }
    // Wider than tall with multiple peaks
    {
        std::vector<std::vector<int>> m = {{2, 1, 2, 1, 2}};
        auto result = findPeaksInMatrix(m);
        assert(result.size() == 3);
        assert(result[0] == std::make_pair(0, 0));
        assert(result[1] == std::make_pair(0, 2));
        assert(result[2] == std::make_pair(0, 4));
    }
    return 0;
}

#include <vector>
#include <utility>

// Return coordinates of all cells that are >= all their existing neighbors.
// Matrix must be non-empty and rectangular. Coordinates are in row-major order.
std::vector<std::pair<int, int>> findPeaksInMatrix(const std::vector<std::vector<int>>& matrix) {
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    std::vector<std::pair<int, int>> peaks;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const int value = matrix[i][j];
            bool is_peak = true;

            // Check up
            if (i > 0 && matrix[i - 1][j] > value) is_peak = false;
            // Check down
            if (i < rows - 1 && matrix[i + 1][j] > value) is_peak = false;
            // Check left
            if (j > 0 && matrix[i][j - 1] > value) is_peak = false;
            // Check right
            if (j < cols - 1 && matrix[i][j + 1] > value) is_peak = false;

            if (is_peak) {
                peaks.emplace_back(i, j);
            }
        }
    }
    return peaks;
}

// The solution uses straightforward nested loops to examine every cell exactly once. For each cell, we determine its "effective neighbors" based on its position: corner cells have two neighbors, edge cells have three, interior cells have four. We compare the cell's value against each neighbor; if it is greater than or equal to all, we add its coordinates to the result. Special care is needed for degenerate cases: a 1x1 matrix has no neighbors, so the single cell is trivially a peak; a single-row matrix means only left/right exist, and a single-column matrix means only up/down exist. The algorithm runs in O(m*n) time and uses O(1) auxiliary space (excluding the result vector). The row-major ordering naturally emerges from the order of iteration (outer loop over rows, inner loop over columns). The function is `const` correct by taking the matrix by const reference.
