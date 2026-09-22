/*
Write a C++ function `int distanceToCenter(const std::vector<std::vector<int>>& grid)` that, given a fixed 5×5 binary matrix (each entry is exactly 0 or 1, with exactly one entry equal to 1), returns the Manhattan distance from that single `1` entry to the center cell at row index 2 and column index 2 (using 0-based indexing). For example, if the `1` is at (0,0), the distance is `|0-2| + |0-2| = 4`. If the `1` is at (2,2), the distance is `0`. You may assume the input always satisfies these constraints (5×5, exactly one `1`, rest zeros). The function should be pure, not read from standard input, and must not modify the input grid.
*/

#include <vector>
#include <cstdlib> // for std::abs

// Return the Manhattan distance from the unique 1-cell to the center (2,2) of a 5x5 grid.
int distanceToCenter(const std::vector<std::vector<int>>& grid) {
    // Grid dimensions are fixed at 5x5 as per problem specification.
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            if (grid[row][col] == 1) {
                // Compute Manhattan distance to center row=2, col=2.
                return std::abs(row - 2) + std::abs(col - 2);
            }
        }
    }
    // Should never reach here due to problem constraints.
    return -1;
}

#include <cassert>
#include <vector>

// The solution function is declared above; this test verifies correctness.
int main() {
    std::vector<std::vector<int>> g1 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(distanceToCenter(g1) == 0);

    std::vector<std::vector<int>> g2 = {
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(distanceToCenter(g2) == 4);

    std::vector<std::vector<int>> g3 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1}
    };
    assert(distanceToCenter(g3) == 4);

    std::vector<std::vector<int>> g4 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(distanceToCenter(g4) == 2);

    std::vector<std::vector<int>> g5 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(distanceToCenter(g5) == 1);

    std::vector<std::vector<int>> g6 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {1,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(distanceToCenter(g6) == 5);

    std::vector<std::vector<int>> g7 = {
        {0,0,0,0,0},
        {0,1,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(distanceToCenter(g7) == 3);

    std::vector<std::vector<int>> g8 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,1,0}
    };
    assert(distanceToCenter(g8) == 3);

    std::vector<std::vector<int>> g9 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,1,0,0,0}
    };
    assert(distanceToCenter(g9) == 5);

    std::vector<std::vector<int>> g10 = {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,1,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    assert(distanceToCenter(g10) == 1);

    return 0;
}

// The problem reduces to locating the unique cell containing `1` in a 5×5 matrix and computing its Manhattan distance to the center (2,2). The Manhattan distance between two cells `(r1,c1)` and `(r2,c2)` is `|r1-r2| + |c1-c2|`. Since the grid size is fixed and exactly one `1` exists, we can simply iterate over all 25 cells, and when we find `grid[i][j] == 1`, compute `abs(i - 2) + abs(j - 2)` and return it. No special edge cases exist beyond the guaranteed input constraints, but the solution is robust if the grid were to contain multiple `1`s — in that case, we would return the distance of the first encountered `1`, which is acceptable per the problem statement. Time complexity is O(25) = O(1) because the grid size is constant; space complexity is O(1) beyond the input storage.
