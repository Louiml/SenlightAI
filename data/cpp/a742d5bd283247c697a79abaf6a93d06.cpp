/*
Write a C++ function that takes a 5×5 grid of integers as a 2D vector (or array) where exactly one element is `1` and all others are `0`. The function must return the minimum number of single-step moves (up, down, left, or right) required to bring that `1` to the center cell of the grid (row index 2, column index 2, using 0-based indexing). The grid is guaranteed to contain exactly one `1`. The function should be named `movesToCenter` and should accept a `const std::vector<std::vector<int>>&` (or a fixed-size 5×5 array) and return an `int`. The grid dimensions are always 5×5, and the center is always fixed at (2,2). Assume the input is valid (exactly one `1` exists).
*/
#include <vector>
#include <cstdlib> // for std::abs

// Find the Manhattan distance from the single '1' in a 5x5 grid to the center (2,2).
// The grid is guaranteed to have exactly one 1 and all other entries are 0.
int movesToCenter(const std::vector<std::vector<int>>& grid) {
    const int rows = 5;
    const int cols = 5;
    const int targetRow = 2;
    const int targetCol = 2;
    
    int foundRow = 0;
    int foundCol = 0;
    bool found = false;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) {
                foundRow = i;
                foundCol = j;
                found = true;
                break;
            }
        }
        if (found) break;
    }
    
    return std::abs(foundRow - targetRow) + std::abs(foundCol - targetCol);
}
#include <cassert>
#include <vector>

// The solution function is declared above; this main() tests it.

int main() {
    // Example: 1 at top-left
    std::vector<std::vector<int>> grid1 = {
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToCenter(grid1) == 4);

    // Example: 1 already at center
    std::vector<std::vector<int>> grid2 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToCenter(grid2) == 0);

    // Example: 1 at bottom-right
    std::vector<std::vector<int>> grid3 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1}
    };
    assert(movesToCenter(grid3) == 4);

    // Example: 1 at (1,3) -> distance |1-2|+|3-2| = 2
    std::vector<std::vector<int>> grid4 = {
        {0,0,0,0,0},
        {0,0,0,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToCenter(grid4) == 2);

    // Example: 1 at (4,0) -> distance |4-2|+|0-2| = 4
    std::vector<std::vector<int>> grid5 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {1,0,0,0,0}
    };
    assert(movesToCenter(grid5) == 4);

    // Example: 1 at (0,4) -> distance |0-2|+|4-2| = 4
    std::vector<std::vector<int>> grid6 = {
        {0,0,0,0,1},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToCenter(grid6) == 4);

    // Example: 1 at (2,0) -> distance 2
    std::vector<std::vector<int>> grid7 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(movesToCenter(grid7) == 2);

    return 0;
}
// The problem reduces to finding the Manhattan distance between the position of `1` and the center `(2,2)`. Since moves are only up/down/left/right (no diagonals), the minimum number of moves equals `abs(r - 2) + abs(c - 2)`, where `(r,c)` is the location of the `1`. The algorithm scans the entire 5×5 grid to locate the `1`, then computes the Manhattan distance. No special edge cases exist beyond the guarantee of exactly one `1`; if the `1` is already at the center, the result is `0`. Time complexity is O(25) = O(1) since the grid size is fixed, and space complexity is O(1) beyond the input storage. The solution avoids simulating moves, which would be unnecessarily complex.
