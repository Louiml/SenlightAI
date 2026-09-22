// Write a C++ function `bool canFormPlus(vector<vector<char>>& grid)` that takes a rectangular grid of characters containing only `'*'` and `'.'` and determines whether the grid contains exactly one "plus" shape made of `'*'` characters. A plus shape is defined as a central cell `(i, j)` such that all cells in the four straight lines extending upward, downward, left, and right from that center (including the center) are `'*'`, and the plus must be at least 3 cells wide and 3 cells tall (i.e., the center is not on the border, so each arm has at least one cell). The grid must contain exactly one such plus shape, and after removing that plus shape (converting all its `'*'` cells to `'.'`), the grid must contain no remaining `'*'` cells. The function should return `true` if the grid satisfies these conditions, and `false` otherwise. The grid dimensions are at least 3×3. You may modify the input grid (e.g., clearing the plus) during the check. Assume the grid is non-empty and rectangular.
#include <cassert>
#include <vector>

// (The solution function is assumed to be included above.)

int main() {
    // Test 1: Simple 3x3 plus
    std::vector<std::vector<char>> grid1 = {
        {'.', '*', '.'},
        {'*', '*', '*'},
        {'.', '*', '.'}
    };
    assert(canFormPlus(grid1) == true);

    // Test 2: No plus (single star)
    std::vector<std::vector<char>> grid2 = {
        {'.', '.', '.'},
        {'.', '*', '.'},
        {'.', '.', '.'}
    };
    assert(canFormPlus(grid2) == false);

    // Test 3: Two separate pluses
    std::vector<std::vector<char>> grid3 = {
        {'*', '.', '*', '.'},
        {'.', '*', '.', '*'},
        {'*', '.', '*', '.'},
        {'.', '*', '.', '*'}
    };
    assert(canFormPlus(grid3) == false);

    // Test 4: Plus with extra star outside
    std::vector<std::vector<char>> grid4 = {
        {'.', '*', '.', '.'},
        {'*', '*', '*', '*'},
        {'.', '*', '.', '.'},
        {'.', '.', '.', '.'}
    };
    assert(canFormPlus(grid4) == false);

    // Test 5: Larger plus, valid
    std::vector<std::vector<char>> grid5 = {
        {'.', '.', '*', '.', '.'},
        {'.', '.', '*', '.', '.'},
        {'*', '*', '*', '*', '*'},
        {'.', '.', '*', '.', '.'},
        {'.', '.', '*', '.', '.'}
    };
    assert(canFormPlus(grid5) == true);

    // Test 6: Plus touching border (center not interior) -> invalid
    std::vector<std::vector<char>> grid6 = {
        {'*', '*', '*'},
        {'.', '*', '.'},
        {'.', '*', '.'}
    };
    assert(canFormPlus(grid6) == false);

    // Test 7: Plus shape with a hole (missing one arm cell) -> no valid center
    std::vector<std::vector<char>> grid7 = {
        {'.', '*', '.'},
        {'*', '*', '.'},
        {'.', '*', '.'}
    };
    assert(canFormPlus(grid7) == false);

    // Test 8: All stars -> multiple centers
    std::vector<std::vector<char>> grid8 = {
        {'*', '*', '*'},
        {'*', '*', '*'},
        {'*', '*', '*'}
    };
    assert(canFormPlus(grid8) == false);

    return 0;
}
#include <vector>

// Determine if the grid contains exactly one plus-shaped cluster of '*' that covers all '*' cells.
bool canFormPlus(std::vector<std::vector<char>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    int centerCount = 0;
    int centerR = -1, centerC = -1;

    // Find all potential centers (inner cells with all four orthogonal neighbors being '*')
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            if (grid[i][j] == '*' &&
                grid[i-1][j] == '*' &&
                grid[i+1][j] == '*' &&
                grid[i][j-1] == '*' &&
                grid[i][j+1] == '*') {
                ++centerCount;
                centerR = i;
                centerC = j;
            }
        }
    }

    if (centerCount != 1) {
        return false;
    }

    // Delete the plus shape: traverse up, down, left, right from center
    // Up direction
    for (int i = centerR; i >= 0 && grid[i][centerC] == '*'; --i) {
        grid[i][centerC] = '.';
    }
    // Down direction
    for (int i = centerR; i < rows && grid[i][centerC] == '*'; ++i) {
        grid[i][centerC] = '.';
    }
    // Left direction
    for (int j = centerC; j >= 0 && grid[centerR][j] == '*'; --j) {
        grid[centerR][j] = '.';
    }
    // Right direction
    for (int j = centerC; j < cols && grid[centerR][j] == '*'; ++j) {
        grid[centerR][j] = '.';
    }

    // Check that no '*' remains
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == '*') {
                return false;
            }
        }
    }
    return true;
}
// The solution approach is to first scan all interior cells (excluding the border rows and columns) to find any cell that has all four orthogonal neighbors also equal to `'*'`. For each such candidate center, count how many such centers exist. If there is exactly one such center, then we simulate deleting that plus shape by traversing up, down, left, and right from the center while the cells are `'*'`, setting them to `'.'`. After deletion, we scan the entire grid to verify that no `'*'` remains. If a remaining `'*'` is found, or if the number of candidate centers is not exactly one, return `false`; otherwise return `true`. Edge cases: if there are zero candidate centers, the grid has no plus shape, so return `false`; if there is more than one candidate center, the plus shape is ambiguous or overlapping, return `false`; if the plus is perfectly formed but there are extra stars elsewhere, the deletion will leave them and return `false`. Time complexity is O(rows * cols) for scanning and deletion, and space complexity is O(1) auxiliary (modifying grid in place).
