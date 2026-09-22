// Write a C++ function named `movesToReachOne` that takes a 5x5 grid of integers (all zeros except a single `1`) as a 2D vector (or array) and returns the minimum number of orthogonal moves (up, down, left, right) needed to move the `1` to the center cell (row 2, column 2, using 0-based indexing). The grid is always 5x5, contains exactly one `1`, and all other cells are `0`. The function should return the Manhattan distance from the `1`'s position to the center. If the grid does not contain exactly one `1`, the behavior is undefined — you may assume valid input. For clarity, the center cell is at (2,2) in 0-based indexing, or (3,3) in 1-based indexing. The input grid is passed by const reference, and no modification is allowed.
// The task reduces to finding the coordinates (row, column) of the single `1` in the 5x5 grid, then computing the Manhattan distance to the center (2,2). Manhattan distance is `abs(row - 2) + abs(col - 2)`. Since the grid is fixed at 5x5 and the center is always (2,2), the solution is straightforward: iterate over all 25 cells, find the one with value `1`, record its row and column, and compute the distance. The challenge is to handle the indexing correctly (0-based vs 1-based) and avoid early termination if the problem demands a complete scan — but here we can return immediately upon finding the `1`. Edge cases: the `1` could already be in the center, yielding distance 0; the `1` could be in any corner, yielding distance 4. Time complexity is O(25) = O(1) since the grid size is constant; space complexity is O(1) aside from the input storage.
#include <vector>
#include <cmath>

// Returns the Manhattan distance from the cell containing 1 to the center (2,2) of a 5x5 grid.
int movesToReachOne(const std::vector<std::vector<int>>& grid) {
    const int centerRow = 2;
    const int centerCol = 2;
    
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            if (grid[row][col] == 1) {
                return std::abs(row - centerRow) + std::abs(col - centerCol);
            }
        }
    }
    // Should never reach here given valid input, but return -1 as a sentinel.
    return -1;
}
#include <cassert>
#include <vector>

// Assume movesToReachOne is defined above.

int main() {
    // 1 is at center (2,2) -> distance 0
    std::vector<std::vector<int>> grid1 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToReachOne(grid1) == 0);

    // 1 at top-left corner (0,0) -> |0-2|+|0-2| = 4
    std::vector<std::vector<int>> grid2 = {
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToReachOne(grid2) == 4);

    // 1 at bottom-right corner (4,4) -> |4-2|+|4-2| = 4
    std::vector<std::vector<int>> grid3 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1}
    };
    assert(movesToReachOne(grid3) == 4);

    // 1 at (1,3) -> |1-2|+|3-2| = 1+1 = 2
    std::vector<std::vector<int>> grid4 = {
        {0,0,0,0,0},
        {0,0,0,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToReachOne(grid4) == 2);

    // 1 at (4,0) -> |4-2|+|0-2| = 2+2 = 4
    std::vector<std::vector<int>> grid5 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {1,0,0,0,0}
    };
    assert(movesToReachOne(grid5) == 4);

    // 1 at (0,4) -> |0-2|+|4-2| = 2+2 = 4
    std::vector<std::vector<int>> grid6 = {
        {0,0,0,0,1},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToReachOne(grid6) == 4);

    // 1 at (2,0) -> |2-2|+|0-2| = 0+2 = 2
    std::vector<std::vector<int>> grid7 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToReachOne(grid7) == 2);

    // 1 at (0,2) -> |0-2|+|2-2| = 2+0 = 2
    std::vector<std::vector<int>> grid8 = {
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToReachOne(grid8) == 2);

    // 1 at (4,2) -> |4-2|+|2-2| = 2+0 = 2
    std::vector<std::vector<int>> grid9 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0}
    };
    assert(movesToReachOne(grid9) == 2);

    // 1 at (3,3) -> |3-2|+|3-2| = 1+1 = 2
    std::vector<std::vector<int>> grid10 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,1,0},
        {0,0,0,0,0}
    };
    assert(movesToReachOne(grid10) == 2);

    return 0;
}
