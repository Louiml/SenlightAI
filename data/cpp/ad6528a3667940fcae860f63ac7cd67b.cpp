// Write a C++ function `vector<vector<int>> fillSpiral(int n, int m)` that takes positive integers `n` and `m` and returns an `n` by `m` 2D vector (1-indexed conceptually, but stored as 0-indexed) where the cells are filled with numbers from `1` to `n * m` in a clockwise spiral order starting from the top-left cell (row 1, column 1) moving right first. The function must handle any positive `n` and `m` (including `1 x 1` and possibly very large values like `100x100`). The returned grid must have exactly `n` rows and `m` columns, and the spiral fill must never skip a cell or overwrite an existing value. The direction changes only when the next cell is out of bounds or already filled.

#include <cassert>
#include <vector>
#include <iostream>

// Declaration of the solution function (for linking)
std::vector<std::vector<int>> fillSpiral(int n, int m);

int main() {
    // Test 1: 1x1
    auto g1 = fillSpiral(1, 1);
    assert(g1.size() == 1 && g1[0].size() == 1);
    assert(g1[0][0] == 1);

    // Test 2: 1x5 (single row)
    auto g2 = fillSpiral(1, 5);
    assert(g2.size() == 1 && g2[0].size() == 5);
    for (int j = 0; j < 5; ++j) assert(g2[0][j] == j + 1);

    // Test 3: 5x1 (single column)
    auto g3 = fillSpiral(5, 1);
    assert(g3.size() == 5 && g3[0].size() == 1);
    for (int i = 0; i < 5; ++i) assert(g3[i][0] == i + 1);

    // Test 4: 2x2
    auto g4 = fillSpiral(2, 2);
    assert(g4[0][0] == 1 && g4[0][1] == 2);
    assert(g4[1][1] == 3 && g4[1][0] == 4);

    // Test 5: 3x3 standard
    auto g5 = fillSpiral(3, 3);
    assert(g5[0][0] == 1 && g5[0][1] == 2 && g5[0][2] == 3);
    assert(g5[1][2] == 4 && g5[2][2] == 5 && g5[2][1] == 6);
    assert(g5[2][0] == 7 && g5[1][0] == 8 && g5[1][1] == 9);

    // Test 6: 2x3 (rectangular)
    auto g6 = fillSpiral(2, 3);
    assert(g6[0][0] == 1 && g6[0][1] == 2 && g6[0][2] == 3);
    assert(g6[1][2] == 4 && g6[1][1] == 5 && g6[1][0] == 6);

    // Test 7: 3x2 (rectangular)
    auto g7 = fillSpiral(3, 2);
    assert(g7[0][0] == 1 && g7[0][1] == 2);
    assert(g7[1][1] == 3 && g7[2][1] == 4);
    assert(g7[2][0] == 5 && g7[1][0] == 6);

    // Test 8: 4x4 ensure all cells filled exactly once (check sum)
    auto g8 = fillSpiral(4, 4);
    int sum = 0;
    for (const auto& row : g8) for (int v : row) sum += v;
    assert(sum == (4 * 4) * (4 * 4 + 1) / 2); // sum 1..16 = 136

    // Test 9: 5x6 rectangle, verify last cell value is n*m and it's at center or expected position
    auto g9 = fillSpiral(5, 6);
    int maxVal = 0, count = 0;
    for (int i = 0; i < 5; ++i) for (int j = 0; j < 6; ++j) {
        int v = g9[i][j];
        assert(v >= 1 && v <= 30);
        count++;
    }
    assert(count == 30);

    // Test 10: 10x10 using uniqueness check (set of numbers 1..100)
    auto g10 = fillSpiral(10, 10);
    bool seen[101] = {false};
    for (int i = 0; i < 10; ++i) for (int j = 0; j < 10; ++j) {
        int v = g10[i][j];
        assert(v >= 1 && v <= 100);
        assert(!seen[v]);
        seen[v] = true;
    }

    std::cout << "All spiral tests passed.\n";
    return 0;
}

#include <vector>

// Fill an n x m grid with numbers 1..n*m in clockwise spiral order starting at top-left moving right.
std::vector<std::vector<int>> fillSpiral(int n, int m) {
    std::vector<std::vector<int>> grid(n, std::vector<int>(m, 0));
    if (n <= 0 || m <= 0) {
        return grid; // defensive: but spec says positive
    }

    // Direction deltas: right, down, left, up
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};

    int direction = 0;
    int row = 0, col = 0;
    grid[row][col] = 1;
    int current = 2;

    while (current <= n * m) {
        int nextRow = row + dr[direction];
        int nextCol = col + dc[direction];

        if (nextRow >= 0 && nextRow < n && nextCol >= 0 && nextCol < m && grid[nextRow][nextCol] == 0) {
            row = nextRow;
            col = nextCol;
            grid[row][col] = current++;
        } else {
            direction = (direction + 1) % 4;
        }
    }

    return grid;
}

// The algorithm simulates walking through the grid. Start at position `(0,0)` in 0-indexed coordinates, place `1` there, and set a direction index to `0` (meaning right). Maintain four directional deltas: right `(0,1)`, down `(1,0)`, left `(0,-1)`, up `(-1,0)`. For each value from `2` to `n * m`, compute the next cell by adding the current directional delta. If that next cell is inside the grid (row between `0` and `n-1`, column between `0` and `m-1`) and is still zero (unfilled), move to it and assign the next value. Otherwise, rotate the direction clockwise (increment direction index modulo 4) and recompute the next cell without moving yet; this ensures we try the new direction until a valid cell is found. The process terminates exactly after `n * m` placements. Edge cases: when `n=1` or `m=1`, the spiral simply goes straight along the only dimension; when both are 1, only the initial cell is filled. The algorithm is correct for all positive dimensions because the spiral never gets stuck; a valid cell always exists until all are filled. Time complexity is O(n * m) because each cell is visited exactly once for placement, and each failed boundary check costs constant time. Space complexity is O(n * m) for the output grid, plus O(1) auxiliary.
