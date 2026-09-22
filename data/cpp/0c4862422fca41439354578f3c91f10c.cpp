// Write a C++ function `bool canGhostMove(const std::vector<std::string>& grid, size_t row, size_t col, int direction, bool aggressive)`, where `grid` is a rectangular maze represented as a vector of strings (each character is either `'#'` for a wall, `'.'` for a point, `' '` for empty, `'P'` for Pac-Man, `'G'` for another ghost, or `'E'` for an energizer). The function should return `true` if a ghost at position `(row, col)` can move one step in the given `direction` (0=up, 1=right, 2=down, 3=left), subject to the following rules: (1) the target cell must be inside the grid and not a wall (`'#'`); (2) the target cell must not contain another ghost (`'G'`); (3) if `aggressive` is `true`, the ghost may move into a cell containing `'P'` (Pac-Man), but if `aggressive` is `false` (frightened), the ghost must NOT move into a cell containing `'P'` (because it would be eaten). For non-aggressive mode, moving into empty cells, points, energizers, or even out-of-bounds is not allowed—only valid in-bounds non-wall cells that are not Pac-Man. Do not modify the grid. Assume the input grid is non-empty, all rows have equal length, and coordinates are always valid (i.e., `(row, col)` is inside the grid and not a wall).

#include <cassert>
#include <string>
#include <vector>

// solution function declared above
bool canGhostMove(const std::vector<std::string>& grid, size_t row, size_t col, int direction, bool aggressive);

int main() {
    std::vector<std::string> maze = {
        "####",
        "#P #",
        "# G#",
        "####"
    };
    // Ghost at (2,2)
    // Up: (1,2) is empty, always allowed
    assert(canGhostMove(maze, 2, 2, 0, true) == true);
    assert(canGhostMove(maze, 2, 2, 0, false) == true);
    // Down: (3,2) is wall '#'
    assert(canGhostMove(maze, 2, 2, 2, true) == false);
    // Left: (2,1) is empty, allowed
    assert(canGhostMove(maze, 2, 2, 3, true) == true);
    // Right: (2,3) is wall '#'
    assert(canGhostMove(maze, 2, 2, 1, true) == false);

    // Ghost adjacent to Pac-Man: ghost at (1,2), Pac-Man at (1,1)
    // Left: target (1,1) contains 'P'
    assert(canGhostMove(maze, 1, 2, 3, true) == true);   // aggressive can enter
    assert(canGhostMove(maze, 1, 2, 3, false) == false); // frightened avoids

    // Another ghost blocking: ghost at (2,1), target (2,2) contains 'G'
    assert(canGhostMove(maze, 2, 1, 1, true) == false);
    assert(canGhostMove(maze, 2, 1, 1, false) == false);

    // Out-of-bounds: ghost at (0,1) moving up
    std::vector<std::string> single_row = {"#P#G"};
    assert(canGhostMove(single_row, 0, 2, 0, true) == false);
    // Right from ghost at (0,2) goes out of bounds
    assert(canGhostMove(single_row, 0, 2, 1, true) == false);
    return 0;
}

#include <string>
#include <vector>

// Determine if a ghost at (row, col) can move one step in the given direction.
// direction: 0=up, 1=right, 2=down, 3=left
// Returns true if the move is allowed according to the maze and ghost rules.
bool canGhostMove(const std::vector<std::string>& grid, size_t row, size_t col, int direction, bool aggressive) {
    size_t target_row = row;
    size_t target_col = col;
    switch (direction) {
        case 0: target_row = row - 1; break;  // up
        case 1: target_col = col + 1; break;  // right
        case 2: target_row = row + 1; break;  // down
        case 3: target_col = col - 1; break;  // left
        default: return false;
    }

    // Check boundaries
    if (target_row >= grid.size() || target_col >= grid[0].size()) {
        return false;
    }

    char cell = grid[target_row][target_col];
    if (cell == '#') {
        return false;  // wall
    }
    if (cell == 'G') {
        return false;  // another ghost
    }
    if (cell == 'P') {
        return aggressive;  // only move into Pac-Man if aggressive
    }
    return true;  // empty, point, or energizer
}

// The solution is straightforward: first, compute the target cell coordinates based on the given direction (up: `row-1`, right: `col+1`, down: `row+1`, left: `col-1`). Then check if the target is within the grid boundaries (row between 0 and `grid.size()-1`, col between 0 and `grid[0].size()-1`). If out of bounds, return `false` immediately. Next, inspect the character at the target cell. If it is `'#'`, return `false`. If it is `'G'`, return `false` (ghosts block each other). If it is `'P'`, return `true` if `aggressive` is `true`, and `false` otherwise. For any other character (`'.'`, `' '`, `'E'`), return `true`. The complexity is O(1) time and O(1) space because we only examine a single cell and no extra data structures are used. Edge cases: moving into a cell that already contains the same ghost (not possible because we input the ghost's own position, not the target), moving out of bounds (handled), and the condition that non-aggressive ghosts avoid Pac-Man. The function is `const`-correct as it does not modify the grid.
