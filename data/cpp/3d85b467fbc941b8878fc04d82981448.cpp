// Given a 3x3 grid of distinct integers and a list of N called numbers, write a C++ function `bool hasBingo(const std::array<std::array<int,3>,3>& grid, const std::vector<int>& called)` that returns `true` if the called numbers mark an entire row, column, or diagonal in the grid, and `false` otherwise. Marking occurs when a called number exactly matches a grid value. The grid contains unique values, and called numbers may include duplicates and values not present in the grid. The function must handle any N (including zero called numbers) and must not modify the input grid or called list.

#include <array>
#include <vector>
#include <cassert>

// Function prototype.
bool hasBingo(const std::array<std::array<int, 3>, 3>& grid,
              const std::vector<int>& called);

int main() {
    std::array<std::array<int, 3>, 3> grid = {{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    }};

    // No numbers called → no bingo.
    assert(hasBingo(grid, {}) == false);

    // Complete first row.
    assert(hasBingo(grid, {1, 2, 3}) == true);

    // Complete second column.
    assert(hasBingo(grid, {2, 5, 8, 99, 2}) == true);

    // Complete main diagonal.
    assert(hasBingo(grid, {1, 5, 9}) == true);

    // Complete anti-diagonal.
    assert(hasBingo(grid, {3, 5, 7}) == true);

    // Partial row, no bingo.
    assert(hasBingo(grid, {1, 2}) == false);

    // Called numbers not in the grid → no bingo.
    assert(hasBingo(grid, {10, 20, 30}) == false);

    // Numbers in different rows/columns/diagonals, no line complete.
    assert(hasBingo(grid, {1, 5, 3, 7}) == false);

    // Duplicate numbers calling the same cell still counts once.
    assert(hasBingo(grid, {1, 1, 1, 2, 3}) == true);

    // All cells marked → bingo true.
    assert(hasBingo(grid, {1,2,3,4,5,6,7,8,9}) == true);

    return 0;
}

#include <array>
#include <vector>

// Check whether the called numbers complete any winning line in a 3x3 bingo grid.
bool hasBingo(const std::array<std::array<int, 3>, 3>& grid,
              const std::vector<int>& called) {
    bool marked[3][3] = {};

    for (int b : called) {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                if (b == grid[y][x]) {
                    marked[y][x] = true;
                }
            }
        }
    }

    // Check rows.
    for (int y = 0; y < 3; ++y) {
        if (marked[y][0] && marked[y][1] && marked[y][2]) {
            return true;
        }
    }

    // Check columns.
    for (int x = 0; x < 3; ++x) {
        if (marked[0][x] && marked[1][x] && marked[2][x]) {
            return true;
        }
    }

    // Check diagonals.
    if (marked[0][0] && marked[1][1] && marked[2][2]) {
        return true;
    }
    if (marked[0][2] && marked[1][1] && marked[2][0]) {
        return true;
    }

    return false;
}

// The core approach is to create a 3x3 boolean visibility array initialized to `false`. For each called number, perform a linear scan through all 9 grid cells; if a match is found, set the corresponding boolean cell to `true`. After processing all called numbers, check the eight possible winning lines: three rows, three columns, and two diagonals. If every cell in any line is `true`, return `true`; otherwise return `false`. Edge cases: N=0 returns `false` (since no lines can be complete); called numbers may contain duplicates but that's harmless because setting an already-true cell has no effect; numbers not in the grid are ignored. Complexity is O(N * 9 + 8) time = O(N) time and O(9) = O(1) auxiliary space for the boolean array.
