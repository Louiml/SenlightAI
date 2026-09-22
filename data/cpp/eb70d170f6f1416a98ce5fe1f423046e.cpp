Write a C++ function `simulateGasSpread` that takes an initial 2D grid of integers representing gas concentrations (non-negative values, where 0 means empty), the number of simulation steps to run, and returns the final grid after applying the following rule at each step: each cell's new concentration is the average (rounded down to the nearest integer) of its current concentration and the maximum concentration among its four orthogonal neighbors (up, down, left, right); cells on the border only consider existing neighbors, and if a cell has no neighbors (when the grid is 1x1), its concentration stays unchanged. The function must handle grids of any size (including 1x1 and rectangular), preserve the original dimensions, and not modify the input grid. For example, if the initial grid is `{{5,3},{2,8}}` and one step is run, the result would be `{{4,5},{4,6}}` because the top-left cell averages 5 with max(3,2)=3 → (5+3)/2=4; top-right averages 3 with max(5,8)=8 → (3+8)/2=5 (integer division); bottom-left averages 2 with max(5,8)=8 → (2+8)/2=5, but wait—recheck: bottom-left's neighbors are 5 (up) and 8 (right), max=8, so (2+8)/2=5; bottom-right averages 8 with max(3,2)=3 → (8+3)/2=5.5 → floor 5. Actually the output should be `{{4,5},{5,5}}`. The function should perform all updates simultaneously (i.e., use the previous step's grid to compute all new values for the next step), and the number of steps is a non-negative integer (0 steps returns a copy of the input grid). Provide the function signature: `std::vector<std::vector<int>> simulateGasSpread(const std::vector<std::vector<int>>& initialGrid, int steps)`.
The solution requires a simultaneous update, meaning we must compute the next grid from the current grid without modifying the current grid during the iteration. The algorithm for each step: create a new grid of the same dimensions, and for each cell in the current grid, compute the maximum among its existing orthogonal neighbors (checking boundary conditions: if row > 0 consider up, row < rows-1 consider down, col > 0 consider left, col < cols-1 consider right). Then set the new cell value to `(currentValue + maxNeighbor) / 2` using integer division (which floors automatically for positive numbers since all values are non-negative). Handle the edge case of a 1x1 grid: no neighbors, so maxNeighbor is effectively 0? But the rule states "if a cell has no neighbors, its concentration stays unchanged", so we must check that: if the cell has at least one neighbor, apply the formula; if it has no neighbors (only possible when grid is 1x1), keep the same value. This is a straightforward O(rows * cols) per step, so for `steps` iterations, time complexity is O(steps * rows * cols). Space complexity is O(rows * cols) for the new grid copy each step. Edge cases: empty grid (zero rows or columns) should return an empty vector; negative steps are not specified but we can treat as 0. Also, values may become larger if maxNeighbor is larger than current, but that's fine; the data type `int` is sufficient given the problem constraints (no overflow concerns for reasonable inputs). The implementation should use `const` references for the input, and return by value.
#include <vector>
#include <algorithm>

// Simulate gas spread on a grid for the given number of steps.
// Each cell averages its current value with the maximum of its orthogonal neighbors (floor division).
// Cells with no neighbors (1x1 grid) remain unchanged.
std::vector<std::vector<int>> simulateGasSpread(const std::vector<std::vector<int>>& initialGrid, int steps) {
    if (initialGrid.empty() || initialGrid[0].empty()) {
        return initialGrid; // Empty grid
    }
    
    int rows = static_cast<int>(initialGrid.size());
    int cols = static_cast<int>(initialGrid[0].size());
    
    // Work on a copy of the initial grid
    std::vector<std::vector<int>> currentGrid = initialGrid;
    
    for (int step = 0; step < steps; ++step) {
        // Create the next grid based on currentGrid (simultaneous update)
        std::vector<std::vector<int>> nextGrid(rows, std::vector<int>(cols, 0));
        
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int maxNeighbor = -1; // sentinel to check if any neighbor exists
                
                // Check up
                if (r > 0) {
                    maxNeighbor = std::max(maxNeighbor, currentGrid[r-1][c]);
                }
                // Check down
                if (r < rows - 1) {
                    maxNeighbor = std::max(maxNeighbor, currentGrid[r+1][c]);
                }
                // Check left
                if (c > 0) {
                    maxNeighbor = std::max(maxNeighbor, currentGrid[r][c-1]);
                }
                // Check right
                if (c < cols - 1) {
                    maxNeighbor = std::max(maxNeighbor, currentGrid[r][c+1]);
                }
                
                if (maxNeighbor == -1) {
                    // No neighbors (1x1 grid) -> unchanged
                    nextGrid[r][c] = currentGrid[r][c];
                } else {
                    // Average with floor division (integer division works since non-negative)
                    nextGrid[r][c] = (currentGrid[r][c] + maxNeighbor) / 2;
                }
            }
        }
        
        currentGrid = std::move(nextGrid);
    }
    
    return currentGrid;
}
#include <cassert>
#include <vector>

// Function declaration (normally in a header, but here for testing)
std::vector<std::vector<int>> simulateGasSpread(const std::vector<std::vector<int>>& initialGrid, int steps);

int main() {
    // Test 1: Simple 2x2 grid, 1 step
    {
        std::vector<std::vector<int>> input = {{5,3}, {2,8}};
        std::vector<std::vector<int>> expected = {{4,5}, {5,5}};
        assert(simulateGasSpread(input, 1) == expected);
    }
    
    // Test 2: Zero steps returns copy
    {
        std::vector<std::vector<int>> input = {{1,2}, {3,4}};
        assert(simulateGasSpread(input, 0) == input);
    }
    
    // Test 3: 1x1 grid stays unchanged
    {
        std::vector<std::vector<int>> input = {{42}};
        assert(simulateGasSpread(input, 100) == input);
    }
    
    // Test 4: Single row (1x3 grid)
    {
        std::vector<std::vector<int>> input = {{10, 20, 30}};
        std::vector<std::vector<int>> expected = {{15, 25, 25}}; // step 1
        // neighbors: (0,0): max(20)=20 -> (10+20)/2=15; (0,1): max(10,30)=30 -> (20+30)/2=25; (0,2): max(20)=20 -> (30+20)/2=25
        assert(simulateGasSpread(input, 1) == expected);
    }
    
    // Test 5: Single column (3x1 grid)
    {
        std::vector<std::vector<int>> input = {{5}, {15}, {10}};
        std::vector<std::vector<int>> expected = {{10}, {10}, {12}}; // step 1
        // (0,0): max(15)=15 -> (5+15)/2=10; (1,0): max(5,10)=10 -> (15+10)/2=12; but wait check: (1,0) neighbors up=5, down=10 -> max=10 -> (15+10)/2=12; (2,0): max(15)=15 -> (10+15)/2=12
        // Actually expected should be {{10},{12},{12}}? Let's compute:
        // (0,0): neighbor (1,0)=15 -> (5+15)/2=10
        // (1,0): neighbors (0,0)=5 and (2,0)=10 -> max=10 -> (15+10)/2=12
        // (2,0): neighbor (1,0)=15 -> (10+15)/2=12
        // So expected = {{10},{12},{12}}
        std::vector<std::vector<int>> expected_correct = {{10}, {12}, {12}};
        assert(simulateGasSpread(input, 1) == expected_correct);
    }
    
    // Test 6: Larger grid with multiple steps
    {
        std::vector<std::vector<int>> input = {{1,2,3}, {4,5,6}, {7,8,9}};
        std::vector<std::vector<int>> step1 = simulateGasSpread(input, 1);
        // Manually compute step1:
        // (0,0): neighbors (0,1)=2,(1,0)=4 -> max=4 -> (1+4)/2=2
        // (0,1): neighbors (0,0)=1,(0,2)=3,(1,1)=5 -> max=5 -> (2+5)/2=3
        // (0,2): neighbors (0,1)=2,(1,2)=6 -> max=6 -> (3+6)/2=4
        // (1,0): neighbors (0,0)=1,(1,1)=5,(2,0)=7 -> max=7 -> (4+7)/2=5
        // (1,1): neighbors all -> max=9 -> (5+9)/2=7
        // (1,2): neighbors (0,2)=3,(1,1)=5,(2,2)=9 -> max=9 -> (6+9)/2=7
        // (2,0): neighbors (1,0)=4,(2,1)=8 -> max=8 -> (7+8)/2=7
        // (2,1): neighbors (2,0)=7,(1,1)=5,(2,2)=9 -> max=9 -> (8+9)/2=8
        // (2,2): neighbors (1,2)=6,(2,1)=8 -> max=8 -> (9+8)/2=8
        std::vector<std::vector<int>> expected_step1 = {{2,3,4},{5,7,7},{7,8,8}};
        assert(step1 == expected_step1);
        // Check step2 is different (no assertion on exact value, just that it changes)
        std::vector<std::vector<int>> step2 = simulateGasSpread(input, 2);
        assert(step2 != step1);
        assert(step2 != input);
    }
    
    // Test 7: Empty grid
    {
        std::vector<std::vector<int>> empty;
        assert(simulateGasSpread(empty, 5).empty());
    }
    
    // Test 8: Grid with zeros only
    {
        std::vector<std::vector<int>> input = {{0,0}, {0,0}};
        assert(simulateGasSpread(input, 10) == input);
    }
    
    // Test 9: Negative steps treated as zero (or no change)
    {
        std::vector<std::vector<int>> input = {{1,2}, {3,4}};
        assert(simulateGasSpread(input, -3) == input);
    }
    
    std::cout << "All tests passed!\n";
    return 0;
}
