// Create a C++ function `int floodFillClick(int grid[10][10], bool revealed[10][10], int row, int col)` that simulates a Minesweeper-style "safe click" on a 10×10 board. The `grid` contains integer values: `-1` denotes a mine, and non-negative integers denote the number of adjacent mines (0–8). The `revealed` boolean matrix tracks which cells have been revealed (`true`) or hidden (`false`). The function should simulate a click on `(row, col)` assuming that cell is not a mine. If the clicked cell has zero adjacent mines, implement a flood-fill algorithm to reveal all connected zero-cells and their neighboring non-zero cells (classic Minesweeper expansion). The function should update `revealed` accordingly and return the total number of newly revealed cells in that click. If the clicked cell is already revealed, it should return 0 without changing anything. You can assume `row` and `col` are within 0–9 and that `grid[row][col] != -1`. Do not handle mine clicks or game win conditions; focus solely on the flood-fill expansion logic.
// The solution uses a breadth-first search (BFS) style flood fill. Start by checking if `revealed[row][col]` is already true; if so, return 0. Otherwise, mark it revealed and increment a counter. If the cell’s grid value is 0, enqueue its position and process its 8 neighbors (including diagonals). For each neighbor, if it is within bounds and not yet revealed and not a mine (grid != -1), reveal it, increment the counter, and if the neighbor also has grid value 0, enqueue it for further expansion. The process continues until the queue is empty. Non-zero cells are revealed but not expanded further because only zero-cells trigger neighbor expansion. Edge cases include clicks near the border (need bounds checks), cells adjacent to mines (they stop expansion), and fully revealed boards (return 0). Time complexity is O(100) per call in the worst case because the grid is fixed at 10×10; each cell is visited at most once. Space complexity is O(100) for the queue and visited state (implicitly via `revealed`). The function only updates `revealed` and returns the count, so it is deterministic and testable.
#include <queue>
#include <utility>
#include <vector>

// Simulate a safe click on a 10x10 Minesweeper board.
// grid: -1 means mine, otherwise adjacent mine count (0-8)
// revealed: updated to show newly revealed cells
// row, col: clicked coordinates (0-indexed, within 0-9, not a mine)
// Returns number of newly revealed cells (0 if already revealed)
int floodFillClick(int grid[10][10], bool revealed[10][10], int row, int col) {
    if (revealed[row][col]) {
        return 0;
    }

    int newlyRevealed = 0;
    std::queue<std::pair<int, int>> toProcess;

    // Reveal the clicked cell
    revealed[row][col] = true;
    newlyRevealed++;

    if (grid[row][col] == 0) {
        toProcess.push({row, col});
    }

    const int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    while (!toProcess.empty()) {
        auto [r, c] = toProcess.front();
        toProcess.pop();

        for (int i = 0; i < 8; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Bounds check
            if (nr < 0 || nr >= 10 || nc < 0 || nc >= 10) continue;

            // Skip if already revealed or mine
            if (revealed[nr][nc] || grid[nr][nc] == -1) continue;

            // Reveal neighbor
            revealed[nr][nc] = true;
            newlyRevealed++;

            // If neighbor is zero, it triggers further expansion
            if (grid[nr][nc] == 0) {
                toProcess.push({nr, nc});
            }
        }
    }

    return newlyRevealed;
}
#include <cassert>
#include <cstring>

int main() {
    // Test 1: Simple board with one zero in corner, no expansion needed
    int grid1[10][10] = {0};
    bool rev1[10][10] = {false};
    grid1[0][0] = 1; // non-zero, so no flood fill
    int result1 = floodFillClick(grid1, rev1, 0, 1);
    assert(result1 == 1);
    assert(rev1[0][1] == true);
    assert(rev1[0][0] == false);

    // Test 2: Zero cell expands to all connected zeros and neighbors
    int grid2[10][10] = {0};
    bool rev2[10][10] = {false};
    grid2[0][0] = -1; // mine blocks expansion
    grid2[0][1] = 1;
    grid2[1][0] = 1;
    grid2[1][1] = 2;
    // Click on (1,1) which is value 2? No, we need zero cell. Click (2,2) which is zero.
    int result2 = floodFillClick(grid2, rev2, 2, 2);
    assert(result2 > 0);
    // Should have revealed large area including (2,2), (3,3), etc., but not (0,0) mine
    assert(rev2[2][2] == true);
    assert(rev2[3][3] == true);
    assert(rev2[0][0] == false);

    // Test 3: Already revealed cell returns 0
    int grid3[10][10] = {0};
    bool rev3[10][10] = {false};
    grid3[5][5] = 1;
    floodFillClick(grid3, rev3, 5, 5);
    int result3 = floodFillClick(grid3, rev3, 5, 5);
    assert(result3 == 0);

    // Test 4: Edge case at board boundary, zero expansion limited by bounds
    int grid4[10][10] = {0};
    bool rev4[10][10] = {false};
    grid4[0][0] = 0; // zero at corner
    grid4[0][1] = 1;
    grid4[1][0] = 1;
    grid4[1][1] = 2;
    int result4 = floodFillClick(grid4, rev4, 0, 0);
    // Should reveal (0,0), (0,1), (1,0) but not (1,1) because it's non-zero? Actually (1,1) is 2 and gets revealed as neighbor of zero? No, (1,1) is neighbor of (0,1) and (1,0) but those are non-zero, so they don't expand. But (0,0) is zero, so expands to all neighbors: (0,1)=1, (1,0)=1, (1,1)=2. So all four revealed.
    assert(result4 == 4);
    assert(rev4[1][1] == true);

    // Test 5: Zero cell surrounded by mines, only itself revealed
    int grid5[10][10] = {0};
    bool rev5[10][10] = {false};
    // Make (4,4) surrounded by mines except itself
    grid5[3][3] = -1; grid5[3][4] = -1; grid5[3][5] = -1;
    grid5[4][3] = -1;                    grid5[4][5] = -1;
    grid5[5][3] = -1; grid5[5][4] = -1; grid5[5][5] = -1;
    grid5[4][4] = 0;
    int result5 = floodFillClick(grid5, rev5, 4, 4);
    assert(result5 == 1);
    assert(rev5[4][4] == true);
    assert(rev5[3][3] == false);

    return 0;
}
