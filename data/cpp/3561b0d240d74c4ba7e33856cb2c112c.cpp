/*
Write a C++ function `int countNeighbors(int H, int W, int R, int C)` that, given a grid with H rows and W columns (both at least 1), and a cell located at row R and column C (both 1-indexed), returns the number of cells adjacent (sharing an edge, not diagonal) to the given cell that are inside the grid. The function must handle boundary cells correctly: a cell at a corner has 2 neighbors, a cell on an edge (but not corner) has 3, and an interior cell has 4. The function should use at most a constant amount of extra space (excluding input parameters) and be robust for any positive H, W and valid R, C such that 1 ≤ R ≤ H and 1 ≤ C ≤ W.
*/

#include <cstddef>

// Count the number of grid-neighbors (sharing an edge) that are inside the grid.
// H = rows, W = columns, R = row (1-indexed), C = column (1-indexed).
// Returns 2, 3, or 4 depending on the cell's position.
int countNeighbors(int H, int W, int R, int C) {
    int count = 0;
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    for (std::size_t i = 0; i < 4; ++i) {
        int nx = R + dx[i];
        int ny = C + dy[i];
        if (nx >= 1 && nx <= H && ny >= 1 && ny <= W) {
            ++count;
        }
    }
    return count;
}

#include <cassert>

// Assume the solution function is available from the same file (or included).
int main() {
    // Interior cell of a large grid
    assert(countNeighbors(5, 5, 3, 3) == 4);
    // Corner cells
    assert(countNeighbors(5, 5, 1, 1) == 2);
    assert(countNeighbors(5, 5, 1, 5) == 2);
    assert(countNeighbors(5, 5, 5, 1) == 2);
    assert(countNeighbors(5, 5, 5, 5) == 2);
    // Edge cells (non-corner)
    assert(countNeighbors(5, 5, 1, 3) == 3);
    assert(countNeighbors(5, 5, 3, 1) == 3);
    assert(countNeighbors(5, 5, 3, 5) == 3);
    assert(countNeighbors(5, 5, 5, 3) == 3);
    // 1x1 grid: only cell has 0 neighbors
    assert(countNeighbors(1, 1, 1, 1) == 0);
    // 1xN grid: cells on the only row all have 2 neighbors except the ends
    assert(countNeighbors(1, 4, 1, 1) == 1);
    assert(countNeighbors(1, 4, 1, 2) == 2);
    assert(countNeighbors(1, 4, 1, 4) == 1);
    // Nx1 grid: cells on the only column have 2 neighbors except the ends
    assert(countNeighbors(4, 1, 1, 1) == 1);
    assert(countNeighbors(4, 1, 2, 1) == 2);
    assert(countNeighbors(4, 1, 4, 1) == 1);
    return 0;
}

// The solution iterates over the four possible directional offsets (up, down, left, right). For each offset, compute the candidate neighbor coordinates (nx, ny). Then check if 1 ≤ nx ≤ H and 1 ≤ ny ≤ W. If true, increment the count. This requires constant time, since we only check four fixed directions. The edge cases are naturally handled by the bounds check: corner cells have two valid neighbors, edge cells have three, and interior cells have all four. There are no tricky cases involving negative coordinates or out-of-range R, C because they are guaranteed to be valid by the problem statement. Time complexity: O(1). Space complexity: O(1) besides the input parameters.
