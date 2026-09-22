// Write a C++ function `std::vector<std::vector<int>> generateSpiralPattern(int n)` that, given a positive integer `n`, returns a 2D vector of size `(2*n-1) x (2*n-1)` filled with concentric square layers. The outermost layer should be filled with the value `n`, the next inner layer with `n-1`, and so on, until the center cell (at position `[n-1][n-1]`) is filled with `1`. All other cells not yet assigned to a layer should follow the same rule: for a given layer index `k` (starting from 0 for the outermost), cells lying on the border of the square submatrix from row `k` to row `2*n-2-k` and column `k` to column `2*n-2-k` get the value `n-k`. The function must handle edge cases like `n=1` (returns a 1x1 matrix with value 1) and ensure all elements are correctly filled without overlap. The output should be a rectangular matrix where every row has the same length.

The problem is a classic pattern generation task. The core idea is to simulate filling layers from the outermost to the innermost. Given `n`, the matrix size is `m = 2*n-1`. Initialize a 2D vector of size `m x m` with zeros. Then, set `t = n` (the current layer value) and `start = 0`, `endp = m-1`. In each iteration (while `t > 0`):
- Fill all cells on the border of the square defined by rows `[start, endp]` and columns `[start, endp]` with value `t`. A cell lies on the border if its row is `start` or `endp`, or its column is `start` or `endp`.
- Then increment `start`, decrement `endp`, and decrement `t`.
This continues until `t` becomes 0, by which time all layers from `n` down to `1` are filled. For `n=1`, `m=1`, the loop runs once filling the single cell with `1`, which is correct. Edge cases include very large `n` (memory usage is O(n^2)), but the algorithm is straightforward. Time complexity is O(n^2) because we iterate over each cell exactly once when its layer is processed (the inner loops run O(m^2) total across all layers, but since layers shrink, total is O(m^2) = O(n^2)). Space complexity is O(n^2) for the output matrix.

#include <vector>
#include <cstddef>

// Generate a (2*n-1) x (2*n-1) matrix with concentric square layers.
// The outermost layer is filled with n, the next inner with n-1, ..., center with 1.
std::vector<std::vector<int>> generateSpiralPattern(int n) {
    if (n <= 0) {
        return {};
    }
    const std::size_t size = static_cast<std::size_t>(2 * n - 1);
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));

    int start = 0;
    int endp = static_cast<int>(size) - 1;
    int value = n;

    while (value >= 1) {
        // Fill the border of the current square layer
        for (int i = start; i <= endp; ++i) {
            for (int j = start; j <= endp; ++j) {
                if (i == start || j == start || i == endp || j == endp) {
                    matrix[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] = value;
                }
            }
        }
        ++start;
        --endp;
        --value;
    }

    return matrix;
}

#include <cassert>
#include <vector>

// declaration of the solution function (assumed included from above)
std::vector<std::vector<int>> generateSpiralPattern(int n);

int main() {
    // Test n=1: single cell with value 1
    auto m1 = generateSpiralPattern(1);
    assert(m1.size() == 1 && m1[0].size() == 1);
    assert(m1[0][0] == 1);

    // Test n=2: 3x3 matrix, outer layer 2, inner cell 1
    auto m2 = generateSpiralPattern(2);
    assert(m2.size() == 3 && m2[0].size() == 3);
    assert(m2[0][0] == 2 && m2[0][1] == 2 && m2[0][2] == 2);
    assert(m2[1][0] == 2 && m2[1][1] == 1 && m2[1][2] == 2);
    assert(m2[2][0] == 2 && m2[2][1] == 2 && m2[2][2] == 2);

    // Test n=3: 5x5 matrix, check some cells
    auto m3 = generateSpiralPattern(3);
    assert(m3.size() == 5 && m3[0].size() == 5);
    // Outer layer value 3 on all borders
    for (int i = 0; i < 5; ++i) {
        assert(m3[0][i] == 3);
        assert(m3[4][i] == 3);
        assert(m3[i][0] == 3);
        assert(m3[i][4] == 3);
    }
    // Second layer value 2 (rows 1..3, cols 1..3 borders)
    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j) {
            if (i == 1 || j == 1 || i == 3 || j == 3) {
                assert(m3[i][j] == 2);
            }
        }
    }
    // Center value 1
    assert(m3[2][2] == 1);

    // Test n=4: 7x7 matrix, verify symmetry
    auto m4 = generateSpiralPattern(4);
    assert(m4.size() == 7 && m4[0].size() == 7);
    assert(m4[0][0] == 4 && m4[0][6] == 4 && m4[6][0] == 4 && m4[6][6] == 4);
    assert(m4[1][1] == 3 && m4[1][5] == 3 && m4[5][1] == 3 && m4[5][5] == 3);
    assert(m4[2][2] == 2 && m4[2][4] == 2 && m4[4][2] == 2 && m4[4][4] == 2);
    assert(m4[3][3] == 1);

    // Edge case n <= 0 should return empty
    auto empty = generateSpiralPattern(0);
    assert(empty.empty());

    return 0;
}
