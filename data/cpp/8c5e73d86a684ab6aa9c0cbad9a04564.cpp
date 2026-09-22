/*
Write a C++ function that takes a positive integer `n` and returns a 2D vector representing an \( (2n-1) \times (2n-1) \) square matrix filled with concentric square rings of decreasing numbers from `n` at the outermost ring down to `1` at the center. The outermost ring consists of all edge cells set to `n`, the next inner ring (offset by 1 from each side) set to `n-1`, and so on until the single center cell is `1`. The function should be named `concentricRings` and should accept `int n` (assume `n >= 1`). The returned matrix must be a `std::vector<std::vector<int>>` where each row is a vector of integers. For example, for `n = 3`, the matrix should be:
```
3 3 3 3 3
3 2 2 2 3
3 2 1 2 3
3 2 2 2 3
3 3 3 3 3
```
The function must be self-contained and must not rely on global state.
*/

#include <vector>

// Returns a (2n-1) x (2n-1) matrix where each concentric square
// ring has a constant value, starting with n at the outermost ring
// and decreasing by 1 toward the center (which is 1).
std::vector<std::vector<int>> concentricRings(int n) {
    const int size = 2 * n - 1;
    // Initialize matrix with zeros.
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));

    int low = 0;
    int high = size;      // exclusive high index for convenient loops
    int value = n;

    while (value > 0) {
        // Fill top row of current ring
        for (int col = low; col < high; ++col) {
            matrix[low][col] = value;
        }
        // Fill right column (excluding top corner already filled)
        for (int row = low + 1; row < high; ++row) {
            matrix[row][high - 1] = value;
        }
        // Fill bottom row (right to left, excluding right corner already filled)
        for (int col = high - 2; col >= low; --col) {
            matrix[high - 1][col] = value;
        }
        // Fill left column (bottom to top, excluding bottom corner already filled)
        for (int row = high - 2; row > low; --row) {
            matrix[row][low] = value;
        }

        ++low;
        --high;
        --value;
    }

    return matrix;
}

#include <cassert>
#include <vector>

// The solution function is declared above (included via header or prior definition).
// For this test harness we copy the declaration and implementation here.
// To keep the test self-contained, we include the function definition again.
std::vector<std::vector<int>> concentricRings(int n) {
    const int size = 2 * n - 1;
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));
    int low = 0;
    int high = size;
    int value = n;
    while (value > 0) {
        for (int col = low; col < high; ++col) matrix[low][col] = value;
        for (int row = low + 1; row < high; ++row) matrix[row][high - 1] = value;
        for (int col = high - 2; col >= low; --col) matrix[high - 1][col] = value;
        for (int row = high - 2; row > low; --row) matrix[row][low] = value;
        ++low; --high; --value;
    }
    return matrix;
}

int main() {
    // Test n = 1 (single cell)
    {
        auto result = concentricRings(1);
        assert(result.size() == 1);
        assert(result[0] == std::vector<int>{1});
    }
    // Test n = 2 (3x3)
    {
        auto result = concentricRings(2);
        std::vector<std::vector<int>> expected = {
            {2,2,2},
            {2,1,2},
            {2,2,2}
        };
        assert(result == expected);
    }
    // Test n = 3 (5x5)
    {
        auto result = concentricRings(3);
        std::vector<std::vector<int>> expected = {
            {3,3,3,3,3},
            {3,2,2,2,3},
            {3,2,1,2,3},
            {3,2,2,2,3},
            {3,3,3,3,3}
        };
        assert(result == expected);
    }
    // Test n = 4 (7x7) check corners and center
    {
        auto result = concentricRings(4);
        assert(result.size() == 7);
        assert(result[0][0] == 4);
        assert(result[6][6] == 4);
        assert(result[3][3] == 1);
        assert(result[1][1] == 3);
        assert(result[2][4] == 3);
    }
    // Test n = 5 (9x9) verify full pattern
    {
        auto result = concentricRings(5);
        // Spot check: all edges are 5, next ring is 4, etc.
        assert(result[0][8] == 5);
        assert(result[8][0] == 5);
        assert(result[1][7] == 4);
        assert(result[7][1] == 4);
        assert(result[2][6] == 3);
        assert(result[6][2] == 3);
        assert(result[3][5] == 2);
        assert(result[5][3] == 2);
        assert(result[4][4] == 1);
    }
    return 0;
}

// The problem is analogous to filling a square matrix layer by layer. The size is `size = 2*n - 1`. We can initialize the matrix with all zeros of that size. Then we iterate a "layer" variable from `0` up to `n-1`. For each layer `k`, the current value to fill is `n - k`. The boundaries of the ring are from row/column index `low = k` to `high = size-1-k` (inclusive). We fill all cells in the top row of this ring, the bottom row, the left column, and the right column with this value. Overlap at corners is harmless because we assign the same value. This matches the pattern given in the snippet but using vectors rather than a fixed-size array. Edge cases: `n=1` gives a 1×1 matrix with value `1`. Since the ring is a single cell, the assignment works (top/bottom/left/right all point to the same cell). Time complexity is \(O( (2n-1)^2 )\) because we fill each cell at most 4 times but each assignment is constant; effectively we visit all cells multiple times but total work is \(O(n^2)\). Space complexity is \(O(n^2)\) for storing the matrix. An alternative more efficient approach would directly compute each cell's value based on distance from edges, but the ring-filling approach is simple and directly mirrors the snippet.
