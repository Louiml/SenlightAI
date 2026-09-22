// Given a 5×5 matrix containing exactly one `1` and all other entries equal to `0`, write a C++ function `int minMovesToCenter(const vector<vector<int>>& matrix)` that returns the minimum number of adjacent moves (up, down, left, right) required to move the `1` to the center cell at row index 2, column index 2. The matrix is guaranteed to be valid: it is exactly 5×5 and contains exactly one `1`. The function should compute the Manhattan distance between the position of the `1` and the center. Handle the edge case where the `1` is already at the center, in which case the result is 0. The function must not modify the input matrix and should use `const` references appropriately.

#include <cassert>
#include <vector>

// Function under test is assumed to be included from the solution header.
int main() {
    // 1 already at center
    std::vector<std::vector<int>> m1 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(minMovesToCenter(m1) == 0);

    // 1 at top-left corner
    std::vector<std::vector<int>> m2 = {
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(minMovesToCenter(m2) == 4);

    // 1 at bottom-right corner
    std::vector<std::vector<int>> m3 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1}
    };
    assert(minMovesToCenter(m3) == 4);

    // 1 at top row, middle column (row 0, col 2)
    std::vector<std::vector<int>> m4 = {
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(minMovesToCenter(m4) == 2);

    // 1 at left column, middle row (row 2, col 0)
    std::vector<std::vector<int>> m5 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(minMovesToCenter(m5) == 2);

    // 1 at row 1, col 4
    std::vector<std::vector<int>> m6 = {
        {0,0,0,0,0},
        {0,0,0,0,1},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(minMovesToCenter(m6) == 3);

    // 1 at row 4, col 1
    std::vector<std::vector<int>> m7 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,1,0,0,0}
    };
    assert(minMovesToCenter(m7) == 3);

    // 1 at row 2, col 4 (right edge, middle row)
    std::vector<std::vector<int>> m8 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(minMovesToCenter(m8) == 2);

    // 1 at row 4, col 2 (bottom edge, middle column)
    std::vector<std::vector<int>> m9 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0}
    };
    assert(minMovesToCenter(m9) == 2);

    // 1 at row 1, col 1 (near top-left)
    std::vector<std::vector<int>> m10 = {
        {0,0,0,0,0},
        {0,1,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(minMovesToCenter(m10) == 2);
}

#include <vector>
#include <cstdlib>

// Return the minimum number of moves to bring the single '1' to the center (2,2) of a 5x5 matrix.
int minMovesToCenter(const std::vector<std::vector<int>>& matrix) {
    int rowPos = 0;
    int colPos = 0;

    // Locate the '1'
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == 1) {
                rowPos = i;
                colPos = j;
                break;
            }
        }
    }

    // Manhattan distance to center (row 2, col 2)
    return std::abs(rowPos - 2) + std::abs(colPos - 2);
}

// The problem is a direct application of the Manhattan distance metric, because each move changes either the row or column index by one, and the shortest path between two grid cells (with no obstacles) is the sum of the absolute differences in row and column coordinates. The algorithm is straightforward: scan the 5×5 matrix to locate the position `(r, c)` where the value equals 1. Since the matrix size is fixed, the scan runs in constant time. Then compute `abs(r - 2) + abs(c - 2)`, where `2` is the zero-based index of the center row and column. Edge cases include the `1` already at the center (distance 0), and the `1` at any corner (maximum distance 4). No other edge cases exist because the input is guaranteed to contain exactly one `1` and all other entries are `0`. The time complexity is O(1) because the matrix dimensions are constant (25 cells scanned at most). The space complexity is O(1) because only a few integer variables are used, and the input matrix is passed by const reference, avoiding any copy.
