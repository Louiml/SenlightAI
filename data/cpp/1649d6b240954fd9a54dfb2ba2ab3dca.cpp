Write a C++ function `bool isPerfectBlackSquare(const std::vector<std::vector<char>>& grid)` that takes a square grid of characters (only `'.'` or `'#'`) and returns `true` if all `'#'` characters in the grid form a single perfect square submatrix (contiguous block) whose sides are exactly equal in length, and every cell inside that submatrix is `'#'` (no holes). All `'#'` cells must be part of that square, and no `'#'` can be outside it. The grid size `n` is at least 1. If there are no `'#'` characters, return `false`.

#include <cassert>
#include <vector>

// declaration of isPerfectBlackSquare (assume included above)

int main() {
    // Empty grid of 0x0? Not allowed by problem but test empty vector
    std::vector<std::vector<char>> empty;
    assert(isPerfectBlackSquare(empty) == false);

    // 1x1 with a single black
    std::vector<std::vector<char>> g1 = {{'#'}};
    assert(isPerfectBlackSquare(g1) == true);

    // 2x2 full black square
    std::vector<std::vector<char>> g2 = {{'#','#'},{'#','#'}};
    assert(isPerfectBlackSquare(g2) == true);

    // 2x2 with a hole
    std::vector<std::vector<char>> g3 = {{'#','#'},{'#','.'}};
    assert(isPerfectBlackSquare(g3) == false);

    // 3x3 with a 2x2 square in corner
    std::vector<std::vector<char>> g4 = {
        {'#','#','.'},
        {'#','#','.'},
        {'.','.','.'}
    };
    assert(isPerfectBlackSquare(g4) == true);

    // 3x3 with a non-square rectangle
    std::vector<std::vector<char>> g5 = {
        {'#','#','.'},
        {'#','#','.'},
        {'#','#','.'}
    };
    assert(isPerfectBlackSquare(g5) == false);

    // 3x3 with a single black
    std::vector<std::vector<char>> g6 = {
        {'.','.','.'},
        {'.','#','.'},
        {'.','.','.'}
    };
    assert(isPerfectBlackSquare(g6) == true);

    // 3x3 with two separate blacks
    std::vector<std::vector<char>> g7 = {
        {'#','.','.'},
        {'.','.','.'},
        {'.','.','#'}
    };
    assert(isPerfectBlackSquare(g7) == false);

    // 4x4 with a 3x3 square but extra black outside
    std::vector<std::vector<char>> g8 = {
        {'#','#','#','.'},
        {'#','#','#','.'},
        {'#','#','#','.'},
        {'.','.','.','#'}
    };
    assert(isPerfectBlackSquare(g8) == false);

    // 4x4 with a 2x2 square and no outside
    std::vector<std::vector<char>> g9 = {
        {'.','.','.','.'},
        {'.','#','#','.'},
        {'.','#','#','.'},
        {'.','.','.','.'}
    };
    assert(isPerfectBlackSquare(g9) == true);

    // 5x5 with a 1x1 square but other black adjacent (shape not square)
    std::vector<std::vector<char>> g10 = {
        {'.','.','.','.','.'},
        {'.','#','#','.','.'},
        {'.','#','#','.','.'},
        {'.','#','#','.','.'},
        {'.','.','.','.','.'}
    };
    assert(isPerfectBlackSquare(g10) == false);
}

#include <vector>
#include <algorithm>

// Returns true if all '#' cells form a single solid square submatrix.
bool isPerfectBlackSquare(const std::vector<std::vector<char>>& grid) {
    int n = grid.size();
    if (n == 0) return false;

    int min_i = n, max_i = -1, min_j = n, max_j = -1;
    int black_count = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == '#') {
                ++black_count;
                min_i = std::min(min_i, i);
                max_i = std::max(max_i, i);
                min_j = std::min(min_j, j);
                max_j = std::max(max_j, j);
            }
        }
    }

    if (black_count == 0) return false;

    int side = max_i - min_i + 1;
    if (side != (max_j - min_j + 1)) return false;
    if (side * side != black_count) return false;

    // Verify that all cells inside the square are '#'
    for (int i = min_i; i <= max_i; ++i) {
        for (int j = min_j; j <= max_j; ++j) {
            if (grid[i][j] != '#') return false;
        }
    }

    return true;
}

// The solution counts the total number of black cells (`black_count`). If `black_count == 0`, return `false` immediately. To check if the blacks form a perfect square, we need to find the bounding box of all `'#'` cells: the minimum and maximum row and column indices. Let `min_i`, `max_i`, `min_j`, `max_j`. For a perfect square, we need:
// 1. The number of rows in the bounding box `(max_i - min_i + 1)` equals the number of columns `(max_j - min_j + 1)`.
// 2. The area of that square `(side * side)` must equal `black_count`.
// 3. Every cell inside the bounding box must be `'#'` (no holes). To check this, we can either scan that submatrix or verify that the count of `'#'` inside equals `side*side` and that no `'#'` exists outside the bounding box (which is automatically true because the bounding box is defined by min/max of all blacks).
// Edge case: if `black_count == 0`, no square exists. If `black_count == 1`, a single cell forms a 1x1 square, so it’s fine.
// Time complexity: O(n^2) to scan the grid once. Space: O(1) auxiliary.
