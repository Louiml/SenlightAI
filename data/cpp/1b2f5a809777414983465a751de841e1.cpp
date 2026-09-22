/*
Given a rectangular grid of measured height values (stored as a 2D array of floats, where some entries may be `NaN` to indicate unmeasured points) and the grid dimensions, write a C++ function that fills all `NaN` entries using bilinear extrapolation from neighboring measured values. Specifically, for each `NaN` at position `(x, y)`, look at the four immediate neighbors in the cardinal directions (up, down, left, right) and the four diagonal neighbors; compute a linear extrapolation for each cardinal direction based on the next point beyond that neighbor (e.g., for the right neighbor, extrapolate using the right neighbor and the point two steps right); for diagonal neighbors, extrapolate similarly using the diagonal neighbor and the point two steps diagonally. Treat any far neighbor (two steps away) that is `NaN` as `0.0`, and if the near neighbor (one step away) is `NaN`, copy the value of the far neighbor (or 0.0 if that is also `NaN`). The final value for the cell is the average of the three extrapolated values (from horizontal, vertical, and diagonal directions). Points outside the grid boundaries are considered `NaN`. The function must modify the grid in place and return `void`. The grid is guaranteed to be at least 3x3 in size, and the corner cells are always valid (non-`NaN`), but other cells may be `NaN`.
*/
#include <cmath>
#include <cstdint>
#include <vector>

// Helper: return the value at (x,y) or NaN if out of bounds.
static float get(const std::vector<std::vector<float>>& grid, int x, int y) {
    if (x < 0 || y < 0 || x >= static_cast<int>(grid.size()) || y >= static_cast<int>(grid[0].size()))
        return std::nanf("");
    return grid[x][y];
}

// Helper: extrapolate from near (one step) and far (two steps) values.
static float extrapolate(float near_val, float far_val) {
    if (std::isnan(far_val)) far_val = 0.0f;
    if (std::isnan(near_val)) near_val = far_val;
    return 2.0f * near_val - far_val;
}

// Fill all NaN cells in the grid using bilinear extrapolation from neighbors.
void extrapolate_unprobed_grid(std::vector<std::vector<float>>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (!std::isnan(grid[x][y])) continue;

            // Horizontal: average of left and right extrapolations
            float left = extrapolate(get(grid, x-1, y), get(grid, x-2, y));
            float right = extrapolate(get(grid, x+1, y), get(grid, x+2, y));
            float horizontal = (left + right) * 0.5f;

            // Vertical: average of up and down extrapolations
            float up = extrapolate(get(grid, x, y-1), get(grid, x, y-2));
            float down = extrapolate(get(grid, x, y+1), get(grid, x, y+2));
            float vertical = (up + down) * 0.5f;

            // Diagonal: average of four diagonal extrapolations
            float diag1 = extrapolate(get(grid, x-1, y-1), get(grid, x-2, y-2));
            float diag2 = extrapolate(get(grid, x-1, y+1), get(grid, x-2, y+2));
            float diag3 = extrapolate(get(grid, x+1, y-1), get(grid, x+2, y-2));
            float diag4 = extrapolate(get(grid, x+1, y+1), get(grid, x+2, y+2));
            float diagonal = (diag1 + diag2 + diag3 + diag4) * 0.25f;

            grid[x][y] = (horizontal + vertical + diagonal) / 3.0f;
        }
    }
}
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution here or link appropriately
// (assuming the above function is declared before main)

int main() {
    // Test 1: Simple interior NaN with valid neighbors
    {
        std::vector<std::vector<float>> grid = {
            {1.0f, 2.0f, 3.0f},
            {4.0f, std::nanf(""), 6.0f},
            {7.0f, 8.0f, 9.0f}
        };
        extrapolate_unprobed_grid(grid);
        assert(!std::isnan(grid[1][1]));
        // Expected: horizontal: left (4) and right (6) -> (4+6)/2=5? Wait, extrapolate uses near and far.
        // Manual: left: near=4, far=1 (since x-2= -1 out of bounds -> NaN) -> 2*4-0=8? No, far NaN becomes 0, near 4 -> 2*4-0=8.
        // Right: near=6, far=3 -> 2*6-3=9. Horizontal average (8+9)/2=8.5
        // Vertical: up: near=2, far=1 (out of bounds? y-2=-1) -> 2*2-0=4; down: near=8, far=7 -> 2*8-7=9; average 6.5
        // Diagonal: diag1(-1,-1) NaN -> 0; diag2(-1,1) NaN -> 0; diag3(1,-1) NaN -> 0; diag4(1,1)=9, far(2,2)=9? Actually x+2=3 out, so far NaN -> 0, near=9 -> 2*9-0=18; average = 18/4=4.5
        // Final = (8.5+6.5+4.5)/3 = 19.5/3 = 6.5
        assert(std::abs(grid[1][1] - 6.5f) < 1e-5);
    }

    // Test 2: Edge cell (top-left corner already valid, but an edge NaN)
    {
        std::vector<std::vector<float>> grid = {
            {1.0f, std::nanf(""), 3.0f},
            {4.0f, 5.0f, 6.0f},
            {7.0f, 8.0f, 9.0f}
        };
        extrapolate_unprobed_grid(grid);
        assert(!std::isnan(grid[0][1]));
        // Manual: left: near=1 (x-1=0,y=1) far=out->0 -> 2*1-0=2
        // right: near=3 (x+1=2,y=1) far=out? x+2=3 out -> 2*3-0=6; horizontal avg=4
        // up: out -> 0; down: near=5, far=6? Wait y+1=2 => grid[0][2]=3? No, for up/down: y-1=-1 out -> 0; y+1=2 => grid[0][2]=3, far y+2=3 out -> 0 -> near=3 far=0 -> 2*3-0=6; vertical avg = (0+6)/2=3
        // diagonal: all out except maybe down-left? diag1(-1,-1) out; diag2(-1,1) out; diag3(1,-1) out; diag4(1,1)=5, far(2,2)=9? x+2=3 out, so far=NaN -> 0; near=5 -> 2*5-0=10; diag avg=10/4=2.5
        // Final = (4+3+2.5)/3 = 9.5/3 ≈ 3.1666667
        assert(std::abs(grid[0][1] - (9.5f/3.0f)) < 1e-5);
    }

    // Test 3: All NaN interior (3x3 with only corners valid)
    {
        std::vector<std::vector<float>> grid = {
            {1.0f, std::nanf(""), 2.0f},
            {std::nanf(""), std::nanf(""), std::nanf("")},
            {3.0f, std::nanf(""), 4.0f}
        };
        extrapolate_unprobed_grid(grid);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                assert(!std::isnan(grid[i][j]));
        // Verify the center value is the average of the four corners? Not exactly due to extrapolation but check it's finite.
        assert(std::isfinite(grid[1][1]));
    }

    // Test 4: Larger grid with pattern, ensure all filled
    {
        std::vector<std::vector<float>> grid(4, std::vector<float>(4, std::nanf("")));
        // Set a 2x2 block of valid values in the middle
        grid[1][1] = 10.0f; grid[1][2] = 20.0f;
        grid[2][1] = 30.0f; grid[2][2] = 40.0f;
        extrapolate_unprobed_grid(grid);
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                assert(!std::isnan(grid[i][j]));
    }

    // Test 5: Grid already fully valid, no change
    {
        std::vector<std::vector<float>> grid = {
            {1.0f, 2.0f},
            {3.0f, 4.0f}
        };
        extrapolate_unprobed_grid(grid);
        assert(grid[0][0] == 1.0f);
        assert(grid[1][1] == 4.0f);
    }

    return 0;
}
// The algorithm iterates over every cell in the grid. For a cell at `(x, y)` that is not `NaN`, skip it. For a `NaN` cell, compute three extrapolated values:
// 1. **Horizontal extrapolation**: Use the left and right neighbor pairs. For the right side: let `a1 = grid[x+1][y]` and `a2 = grid[x+2][y]`; if `a2` is `NaN`, set `a2 = 0.0`; if `a1` is `NaN`, set `a1 = a2`. The extrapolated value is `2*a1 - a2`. Do the same for the left side using `grid[x-1][y]` and `grid[x-2][y]`. Average the two horizontal values.
// 2. **Vertical extrapolation**: Same logic using `grid[x][y+1]`, `grid[x][y+2]` (down) and `grid[x][y-1]`, `grid[x][y-2]` (up). Average the two vertical values.
// 3. **Diagonal extrapolation**: Use the four diagonal directions. For each diagonal, consider the near diagonal neighbor (e.g., `grid[x+1][y+1]`) and the far diagonal neighbor (e.g., `grid[x+2][y+2]`). Apply the same `NaN` handling and compute `2*c1 - c2`. Average all four diagonal values.
//
// The final cell value is the average of the three averaged values (horizontal, vertical, diagonal). Edge cases include cells near the grid boundary where some neighbors are out of range—those neighbors are treated as `NaN` and handled by the same fallback logic (near neighbor becomes far neighbor, far neighbor becomes 0.0). The order of filling matters: the function fills cells in row-major order, but because we only read from neighbors that are at most 2 steps away, and we never write to a cell that is already non-`NaN`, the results are deterministic regardless of order; however, if a neighboring `NaN` cell is processed later, it does not affect the current cell's computation because we only read from the original grid values (but since we modify in place, we must be careful: when computing a cell, we use the current values of neighbors, which may have been filled earlier. This is acceptable and matches the extrapolation logic—treating already-filled neighbors as valid data). Time complexity is O(R*C) where R and C are grid dimensions, since each cell does constant work. Space complexity is O(1) beyond the input grid.
