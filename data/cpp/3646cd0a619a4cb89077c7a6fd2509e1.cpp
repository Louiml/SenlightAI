// Write a C++ function named `wordExistsInGrid` that takes a 2D vector of characters (`grid`) and a string (`word`) as input, and returns a boolean indicating whether the word can be constructed by sequentially moving to adjacent cells (up, down, left, or right) within the grid. Each cell may be used at most once per path, and the word must be formed by the exact sequence of letters in the grid. The function should work for grids of any dimensions (including empty or single-cell grids) and for empty words. The search must consider every possible starting position.

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.
// Test cases:
int main() {
    // Basic grid from the original snippet.
    std::vector<std::vector<char>> grid1 = {
        {'A','B','C','E'},
        {'S','F','C','S'},
        {'A','D','E','E'}
    };
    assert(wordExistsInGrid(grid1, "ABCCED") == true);
    assert(wordExistsInGrid(grid1, "SEE") == true);
    assert(wordExistsInGrid(grid1, "ABCB") == false);

    // Single-cell grid.
    std::vector<std::vector<char>> grid2 = {{'X'}};
    assert(wordExistsInGrid(grid2, "X") == true);
    assert(wordExistsInGrid(grid2, "Y") == false);
    assert(wordExistsInGrid(grid2, "XX") == false);

    // Empty word always exists.
    assert(wordExistsInGrid(grid2, "") == true);

    // Empty grid with non-empty word.
    std::vector<std::vector<char>> grid3;
    assert(wordExistsInGrid(grid3, "A") == false);

    // Grid with all same letters but word requires reuse (should be false).
    std::vector<std::vector<char>> grid4 = {
        {'A','A'},
        {'A','A'}
    };
    assert(wordExistsInGrid(grid4, "AAAA") == true); // can snake through
    assert(wordExistsInGrid(grid4, "AAAAA") == false); // would require reuse

    // Word longer than total cells.
    std::vector<std::vector<char>> grid5 = {{'a','b'}};
    assert(wordExistsInGrid(grid5, "abc") == false);

    // Path that goes up/left/down/right in a 3x1 column.
    std::vector<std::vector<char>> grid6 = {{'A'}, {'B'}, {'C'}};
    assert(wordExistsInGrid(grid6, "CBA") == true); // down, up, down? Actually C->B->A is valid (down then up then down? No: C is at row2, B at row1, A at row0, path down→up→down is not contiguous; but B->C->B is not allowed. Let's use "ABC" which is down from row0 to row2: A->B->C is valid.
    assert(wordExistsInGrid(grid6, "ABC") == true);
    assert(wordExistsInGrid(grid6, "CBA") == true); // C->B is up, B->A is up? Actually C(row2)->B(row1) is up, then B(row1)->A(row0) is up. That's a valid path of length 3.

    return 0;
}

#include <vector>
#include <string>

// Determine if 'word' can be formed by traversing adjacent cells in 'grid' without reusing cells.
bool wordExistsInGrid(std::vector<std::vector<char>>& grid, const std::string& word) {
    if (word.empty()) return true;
    if (grid.empty() || grid[0].empty()) return false;

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Depth-first search from a specific cell.
    const auto dfs = [&](int r, int c, int index) -> bool {
        if (index == static_cast<int>(word.size())) return true;
        if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
        if (grid[r][c] != word[index]) return false;

        char original = grid[r][c];
        grid[r][c] = '#'; // Mark as visited on the current path.

        bool found = dfs(r - 1, c, index + 1) ||
                     dfs(r + 1, c, index + 1) ||
                     dfs(r, c - 1, index + 1) ||
                     dfs(r, c + 1, index + 1);

        grid[r][c] = original; // Backtrack.
        return found;
    };

    // Try every cell as a starting point.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == word[0] && dfs(i, j, 0)) {
                return true;
            }
        }
    }
    return false;
}

// The solution uses a depth-first search (DFS) starting from every cell that matches the first character of the word. At each recursive step, we check if we’ve matched the entire word (base case), whether we are out of bounds, or whether the current cell’s character does not match the expected word character. To ensure each cell is used only once on the current path, we temporarily mark it as visited (e.g., change it to a sentinel like `'#'`), recurse into the four neighbors, and then restore the original character after backtracking. The base case for success occurs when the index reaches the word’s length. Important edge cases include an empty word (always true), an empty grid (false unless word is empty), and a word longer than the total number of cells (impossible to match). The time complexity is O(rows * cols * 4^L) in the worst case, where L is the length of the word, because from each starting cell we may branch into up to 4 directions at each of the L steps. The space complexity is O(L) due to the recursion stack, plus O(1) auxiliary for the visited marking (in-place modification).
