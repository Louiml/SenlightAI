/*
Write a C++ function `spiralMatrixIII(int rows, int cols, int rStart, int cStart)` that simulates walking a spiral path starting at cell (rStart, cStart) in a grid of size `rows` × `cols`. The walk moves in the order: right, down, left, up, repeating, with the step length increasing by 1 after every two direction changes. The function must return a vector of `rows * cols` coordinate pairs (as `vector<vector<int>>` where each inner vector is `{r, c}`) listing the cells in the exact order they are first visited during the spiral traversal, but only including cells that lie within the grid bounds (cells outside the grid are skipped but still count toward the step-length progression). The starting cell (rStart, cStart) is guaranteed to be inside the grid. The traversal continues until all grid cells have been collected exactly once. The order of the returned coordinates must match the traversal order, with the starting cell first.
*/
#include <vector>

// Return all cells of a rows x cols grid in spiral order starting from (rStart, cStart).
std::vector<std::vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart) {
    // Directions: right, down, left, up
    const std::vector<std::vector<int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    const int totalCells = rows * cols;
    std::vector<std::vector<int>> result(totalCells, std::vector<int>(2));
    
    int r = rStart;
    int c = cStart;
    int count = 0;
    int steps = 1;          // current step length
    int directionIndex = 0; // 0: right, 1: down, 2: left, 3: up
    
    // Start cell is always valid
    result[count][0] = r;
    result[count][1] = c;
    ++count;
    
    while (count < totalCells) {
        // Each direction is used for 'steps' moves, and we do two directions per step length
        for (int turn = 0; turn < 2 && count < totalCells; ++turn) {
            int dr = directions[directionIndex][0];
            int dc = directions[directionIndex][1];
            for (int i = 0; i < steps; ++i) {
                r += dr;
                c += dc;
                if (r >= 0 && r < rows && c >= 0 && c < cols) {
                    result[count][0] = r;
                    result[count][1] = c;
                    ++count;
                }
            }
            // Move to next direction (right->down->left->up->...)
            directionIndex = (directionIndex + 1) % 4;
        }
        // After completing two directions, increase the step length
        ++steps;
    }
    
    return result;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (provided by the solution)
std::vector<std::vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart);

int main() {
    // Test 1: 1x1 grid with start cell (0,0)
    assert(spiralMatrixIII(1, 1, 0, 0) == std::vector<std::vector<int>>({{0, 0}}));
    
    // Test 2: 1x5 grid starting at (0, 2) – should walk right then wrap around left
    std::vector<std::vector<int>> expected2 = {{0,2},{0,3},{0,4},{0,0},{0,1}};
    assert(spiralMatrixIII(1, 5, 0, 2) == expected2);
    
    // Test 3: 3x3 grid starting at center (1,1)
    std::vector<std::vector<int>> expected3 = {
        {1,1},{1,2},{2,2},{2,1},{2,0},
        {1,0},{0,0},{0,1},{0,2}
    };
    assert(spiralMatrixIII(3, 3, 1, 1) == expected3);
    
    // Test 4: 2x2 grid starting at (0,0)
    std::vector<std::vector<int>> expected4 = {{0,0},{0,1},{1,1},{1,0}};
    assert(spiralMatrixIII(2, 2, 0, 0) == expected4);
    
    // Test 5: 5x5 grid starting at (2,2) – spiral fully inside
    // We only check size and uniqueness of the first few cells (full check is complex)
    auto result5 = spiralMatrixIII(5, 5, 2, 2);
    assert(result5.size() == 25);
    assert(result5[0] == std::vector<int>({2,2}));
    assert(result5[1] == std::vector<int>({2,3}));
    assert(result5[2] == std::vector<int>({3,3}));
    
    // Test 6: Grid with start at corner (0,0) and rows > cols
    auto result6 = spiralMatrixIII(4, 2, 0, 0);
    assert(result6.size() == 8);
    assert(result6[0] == std::vector<int>({0,0}));
    assert(result6[1] == std::vector<int>({0,1}));
    // Check that all cells are unique (optional simple validation)
    for (size_t i = 0; i < result6.size(); ++i) {
        for (size_t j = i + 1; j < result6.size(); ++j) {
            assert(!(result6[i] == result6[j]));
        }
    }
    
    return 0;
}
// The solution simulates the spiral walk directly. We maintain a direction index cycling through four unit vectors: right (0, +1), down (+1, 0), left (0, -1), up (–1, 0). The step length `steps` starts at 1 and increases by 1 after every two direction changes (completing a "layer" of the spiral). For each direction, we move exactly `steps` cells, updating the current position each time. If the new cell is within bounds (0 ≤ r < rows and 0 ≤ c < cols), we append it to the result. We continue until we have collected all `rows * cols` cells. Edge cases: when the grid is small and the spiral immediately leaves bounds, many moves are skipped; the step count still increments correctly. Starting at the guaranteed in-bounds cell, the first appended cell is the start. Time complexity is O(rows * cols * constant) for the actual in-bounds cells, but the outer loop may run slightly more than that because it walks outside the grid; more precisely, the number of moves taken is O(rows * cols) in the worst case because the spiral expands and the number of total moves until all cells are visited is bounded by a constant factor of the grid area (since we only need to cover the grid, the spiral's radius is O(max(rows, cols)), and total steps are O((rows+cols)^2) which is O(rows*cols) when rows and cols are comparable; for skewed grids it's still O((rows+cols)^2), but that's acceptable). Space is O(rows*cols) for the output vector.
